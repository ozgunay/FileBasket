#ifndef CONNECTION_H
#define CONNECTION_H

#include <fcntl.h>
#include <boost/format.hpp>

#include <QString>
#include <QTime>
#include <QObject>
#include <QtQml/QQmlListProperty>

#include <libssh/libssh.h>
#include <libssh/sftp.h>

#include <fb-lib_global.h>
#include <logger/fb-logger.h>

#include <data/StringDecorator.h>
#include <data/IntDecorator.h>
#include <data/IpDecorator.h>
#include <data/Entity.h>

namespace fb {
namespace models {

typedef enum sshconerr {
    E_OK = 1, E_SESSION_ALOC = -1, E_SSH_CONNECT_ERR = -2, E_SFTP_ALLOC = -3, E_INIT_SFTP = -4, E_CREATE_DIR = -5, E_FILEOPEN_WRITE = -6, E_WRITE_ERR = -7
    , E_FILE_CLOSE = -8, E_FILE_OPEN_READ = -9, E_INVALID_PARAMS = -10, E_SFTP_ERR = -11, E_SFTP_READ_ERR = -12, E_SFTP_READBYTES_ERR = -13, E_GET_FILEINF = -14
    , E_LOCAL_FILE_NOTFOUND = -15, E_RENAME_ERR = -16, E_MEM_ALLOC = -17, E_LOCAL_FILE_READ = -18, E_LOCAL_FILE_RDWR = -19, E_REMOTEFILE_SEEK = -20
    , E_REMOTE_FILE_OPEN = -21, E_DELETE_ERR = -22, E_RENAME_LOCAL_FILE = -23, E_LOCAL_DELETE_FILE = -24, E_FILEOPEN_RDONLY = -25, E_SFTP_READ_EOF = -26
    , E_AUTHENTICATE = -27, E_UNKNOWN = -999
} ESSHERR;


class FBLIBSHARED_EXPORT Connection : public data::Entity
{
    Q_OBJECT
    Q_PROPERTY(fb::data::StringDecorator* ui_username MEMBER m_username
               CONSTANT)
    Q_PROPERTY(fb::data::StringDecorator* ui_password MEMBER m_password
               CONSTANT)
    Q_PROPERTY(fb::data::IpDecorator* ui_hostname MEMBER m_hostname
               CONSTANT)
    Q_PROPERTY(fb::data::IntDecorator* ui_port MEMBER m_port
               CONSTANT)

public:
    Connection(const QString& hostname,
                         const QString& username,
                         const QString& password,
                         const QString& basedir,
                         const int port);
    ~Connection();
    ESSHERR ConnectSession();
    ESSHERR InitSession();
    int ListFilesInDirectory(const QString& fileDirectoryServer);

    void setVerbosity(const int verbosity);
    int getVerbosity();

private:
    ssh_session  m_session;                                             // ssh session
    sftp_session m_sftp;                                                // sftp session
    sftp_file m_file;                                                   // structure for a remote file
    QTime m_transferstarttime;                                          // time of start of the transfer
    data::StringDecorator* m_username{nullptr};
    data::StringDecorator* m_password{nullptr};
    data::IpDecorator* m_hostname{nullptr};                                                 // hostname of the sftp server
    data::StringDecorator* m_basedir{nullptr};                                                  // this base dir is the directory of public and
                                                                        // private key structur ( NOT USED IN THIS VERSION)
    data::IntDecorator* m_port;                                                         // port of the server;
    int m_verbosity;                                                    // degree of verbosity of libssh
    unsigned long long m_rfilesize;                                              // remote file size
    unsigned long long transfered;                                               // number of bytes already transfered

    ESSHERR rwopen_existing_SFTPfile(char *fn);                         // open a file on remote ( server ) read/write   for upload
    ESSHERR rdopen_existing_SFTPfile(char *fn);                         // open a file on remote ( server ) read only    for download
    ESSHERR createSFTPfile(char *fn);                                   // create a file on server;
    ESSHERR writeSFTPfile(char *block, size_t blocksize);               // write a block of data to the  open remote file
    ESSHERR readSFTPfile(char *block, size_t len, DWORD *bytesread);
    ESSHERR closeSFTPfile();                                            // closes the remote file;
    ESSHERR openSFTPfile(char *fn);                                     // opens the remote file
    ESSHERR getSFTPfileinfo();                                          // gets information about the remote file

};

} //namespace models
} //namespace fb

#endif // CONNECTION_H
