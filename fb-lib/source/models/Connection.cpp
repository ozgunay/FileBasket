#include "Connection.h"

using namespace fb::data;

namespace fb {
namespace models {
Connection::Connection(QObject* parent)
    : Entity(parent, "client") {
    m_hostname = static_cast<IpDecorator*>(addDataItem(new
        IpDecorator(this, "ip", "Connection IP")));
    m_username = static_cast<StringDecorator*>(addDataItem(new
        StringDecorator(this, "username", "Username")));
    m_password = static_cast<StringDecorator*>(addDataItem(new
        StringDecorator(this, "password", "Password")));
    m_basedir = static_cast<StringDecorator*>(addDataItem(new
        StringDecorator(this, "basedir", "Base Directory")));
    m_port = static_cast<IntDecorator*>(addDataItem(new
      IntDecorator(this, "port", "Port", 22)));
}
Connection::Connection(QObject* parent, const QJsonObject& json)
    : Connection(parent) {
    fromJson(json);
    m_verbosity = SSH_LOG_RARE;
}

Connection::~Connection() {
    if(m_sftp != nullptr)
        sftp_free(m_sftp);
    if(ssh_is_connected(m_session))
        ssh_disconnect(m_session);
    if(m_session != nullptr)
        ssh_free(m_session);
}

ESSHERR Connection::InitSession() {
    m_session = ssh_new();
    if (m_session == nullptr) {
        LOG_ERROR << "Could not allocate a session.";
    }

    ssh_options_set(m_session, SSH_OPTIONS_HOST, m_hostname->value().toLatin1().data());
    ssh_options_set(m_session, SSH_OPTIONS_LOG_VERBOSITY, &m_verbosity);
    ssh_options_set(m_session, SSH_OPTIONS_PORT, &m_port);
    ssh_options_set(m_session, SSH_OPTIONS_USER, m_hostname->value().toLatin1().data());
    ssh_options_set(m_session, SSH_OPTIONS_SSH_DIR, m_hostname->value().toLatin1().data());

    return E_OK;
}

ESSHERR Connection::ConnectSession() {
    int connectionResult;

    connectionResult = ssh_connect(m_session);
    if (connectionResult != SSH_OK) {
        LOG_ERROR << "Could not connect the ssh session.";
        return E_SSH_CONNECT_ERR;
    }

    connectionResult = ssh_userauth_password(m_session, nullptr, m_password->value().toLatin1().data());
    if (connectionResult != SSH_OK) {
        LOG_ERROR << "Could not authenticate with the ssh server.";
        return E_AUTHENTICATE;
    }
    return E_OK;
}

int Connection::ListFilesInDirectory(const QString& fileDirectoryServer) {
    sftp_dir dir;
    sftp_attributes attributes;
    int connectionResult;

    dir = sftp_opendir(m_sftp, fileDirectoryServer.toLatin1().data());

    if (!dir) {

        LOG_WARNING << boost::format("Directory not opened: %s") %ssh_get_error(m_session);
        return SSH_ERROR;
    }
    printf("Name                       Size Perms    Owner\tGroup\n");
    while ((attributes = sftp_readdir(m_sftp, dir)) != nullptr)
    {
        QString fileName = attributes->name;
        long long unsigned int fileSize = ((static_cast<long long unsigned int>(attributes->size)) / 1024);
        printf("%-20s %10llu %.8o %s(%d)\t%s(%d)\n",
            attributes->name,
            static_cast<long long unsigned int>(attributes->size),
            attributes->permissions,
            attributes->owner,
            attributes->uid,
            attributes->group,
            attributes->gid);
        sftp_attributes_free(attributes);
    }
    if (!sftp_dir_eof(dir)) {
        LOG_WARNING << boost::format("Can't list directory: %s") %ssh_get_error(m_session);
        sftp_closedir(dir);
        return SSH_ERROR;
    }
    connectionResult = sftp_closedir(dir);
    if (connectionResult != SSH_OK) {
        LOG_WARNING << boost::format("Can't close directory: %s") %ssh_get_error(m_session);
        return connectionResult;
    }
    return SSH_OK;
}

void Connection::setVerbosity(const int verbosity) {
    m_verbosity = verbosity;
}

int Connection::getVerbosity() {
    return m_verbosity;
}

ESSHERR Connection::getSFTPfileinfo() {
    sftp_attributes fileinf = sftp_fstat(m_file);

    if (fileinf == nullptr) {
        return E_GET_FILEINF;
    }

    m_rfilesize = fileinf->size;

    sftp_attributes_free(fileinf);
    return E_OK;
}

ESSHERR Connection::closeSFTPfile() {
    int rc = sftp_close(m_file);
    if (rc != SSH_OK) {
        LOG_ERROR << boost::format("Can't close the written m_file: %s") %ssh_get_error(m_session);
        return E_FILE_CLOSE;
    }
    return E_OK;
}

ESSHERR Connection::writeSFTPfile(char *block, size_t blocksize) {
    size_t nwritten = static_cast<size_t>(sftp_write(m_file, block, blocksize));
    if (nwritten != blocksize) {
        LOG_ERROR << boost::format("Can't write data to m_file: %s") %ssh_get_error(m_session);
        //sftp_close(m_file);
        transfered = transfered + nwritten;
        return E_WRITE_ERR;
    }

    transfered = transfered + nwritten;
    return E_OK;
}

ESSHERR Connection::readSFTPfile(char *block, size_t len, unsigned long *bytesread) {
    long long readbytes;
    *bytesread = 0;
    if (len <= 0) return E_INVALID_PARAMS;
    if (bytesread == nullptr || block == nullptr) return E_INVALID_PARAMS;

    readbytes = sftp_read(m_file, block, len);
    if (readbytes < 0) {
        LOG_ERROR << boost::format("Can't read  from  remote m_file: %s  %s")
                     %m_file %ssh_get_error(m_session);
        *bytesread = 0;
        return E_SFTP_READ_ERR;
    }
    // BURADA KALDIM!!!!!
    if (readbytes < static_cast<long long>(len)) {
        *bytesread = readbytes;
        return E_SFTP_READ_EOF;
    }

    *bytesread = readbytes;
    transfered = transfered + readbytes;

    return E_OK;
}

ESSHERR Connection::createSFTPfile(char *fn) {
    int access_type = O_CREAT | O_RDWR | O_APPEND;
    int rc, nwritten;

    std::string filename(fn);
    m_file = sftp_open(m_sftp, fn, access_type, S_IREAD | S_IWRITE);
    if (m_file == nullptr)
    {
        LOG_ERROR << boost::format("Can't open m_file for writing: %s")
                     %ssh_get_error(m_session);
        return E_FILEOPEN_WRITE;
    }
    return E_OK;
}

ESSHERR Connection::rdopen_existing_SFTPfile(char *fn) {
    int access_type = O_RDONLY;
    int rc, nwritten;

    std::string filename(fn);
    m_file = sftp_open(m_sftp, fn,
        access_type, S_IREAD);
    if (m_file == nullptr)
    {
        LOG_ERROR << boost::format("Can't open m_file for writing: %s")
                     %ssh_get_error(m_session);
        return E_FILEOPEN_RDONLY;
    }
    return E_OK;
}

ESSHERR Connection::rwopen_existing_SFTPfile(char *fn) {
    int access_type = O_RDWR;
    int rc, nwritten;

    std::string filename(fn);
    m_file = sftp_open(m_sftp, fn,
        access_type, S_IREAD);
    if (m_file == nullptr)
    {
        LOG_ERROR << boost::format("Can't open m_file for writing: %s")
                     %ssh_get_error(m_session);
        return E_FILEOPEN_RDONLY;
    }
    return E_OK;
}

ESSHERR Connection::openSFTPfile(char *fn) {
    int access_type = O_RDONLY;
    int rc, nwritten;

    std::string filename(fn);
    m_file = sftp_open(m_sftp, fn,
        access_type, S_IWRITE);
    if (m_file == NULL)
    {
        LOG_ERROR << boost::format("Can't open m_file for writing: %s")
                     %ssh_get_error(m_session);
        return E_FILE_OPEN_READ;
    }
    return E_OK;
}


} //namespace models
} //namespace fb
