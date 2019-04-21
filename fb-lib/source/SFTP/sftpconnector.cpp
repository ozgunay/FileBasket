#include "sftpconnector.h"

namespace fb {
namespace sftp {

sftpstat SFTPConnector::getTransferStatus() {
    return  transferstatus;
}

void SFTPConnector::setBlockTransferDelay(int miliseconds) {
    blocktransferdelay = miliseconds;
}

bool SFTPConnector::IsConnected() {
    if (ssh_is_connected(session) == 1) return true;
    return false;
}

int SFTPConnector::GetSessionStatus() {
    return ssh_get_status(session);
}

void SFTPConnector::CloseLocalFile() {
    CloseHandle(localfilehandle);
}

void SFTPConnector::CloseRemoteFile() {
    sftp_close(file);
}

void SFTPConnector::setLogFile(FILE *logf) {
    logfile = logf;
}

void SFTPConnector::CancelTransfer() {
    transferstatus = ES_CANCELLED;
}
void SFTPConnector::PauseTransfer() {
    transferstatus = ES_PAUSED;
    pause = true;
}

//----------------------------------------

// SFTPreger starts or resumes a download.
ESSHERR SFTPConnector::SFTPreget(char *lfn, char *rfn, size_t blocksize) {
    ESSHERR result;
    int rc;
    BOOL  bresult;
    DWORD bytesread;
    DWORD byteswritten;
    filesize = 0;
    transfered = 0;
    initially_was_transferred = 0;
    lfilesize = rfilesize = 0;
    pause = false;
    transferstatus = ES_NONE;
    char *block;
    struct stat st;
    wchar_t temp[SHORT_BUFF_LEN];
    size_t tempsize;
    std::wstring wlfn;
    int loopcounter = 0;

    localfilename = lfn;
    filename = rfn;

    tempfilename = std::string(lfn) + ".sftp_temp";
    mbstowcs_s(&tempsize, temp, tempfilename.c_str(), SHORT_BUFF_LEN);

    localfilehandle = CreateFile(temp, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (localfilehandle == INVALID_HANDLE_VALUE)
    {
        transferstatus = ES_FAILED;
        errstring = L"Could not open local file:" + std::wstring(temp) + L" for read and write";
        Err = E_LOCAL_FILE_RDWR;
        return E_LOCAL_FILE_RDWR;
    }
    local_file_size_lowDWORD = GetFileSize(localfilehandle, &local_file_size_hiDWORD);
    lfilesize = (local_file_size_hiDWORD * 0x100000000) + local_file_size_lowDWORD;

    if (lfilesize < 0) {
        transferstatus = ES_FAILED;
        Err = E_LOCAL_FILE_NOTFOUND;
        errstring = L"Could not get size info for file:" + std::wstring(temp);
        CloseLocalFile();
        return E_LOCAL_FILE_NOTFOUND;
    }

    transfered = lfilesize;
    initially_was_transferred = lfilesize;

    uint64_t tempi64 = transfered & 0x00000000FFFFFFFF;
    long dwlow = tempi64;
    tempi64 = (transfered & 0x7FFFFFFF00000000);
    tempi64 = tempi64 >> 32;
    long  dwhi = tempi64;
    DWORD dwResult = SetFilePointer(localfilehandle, dwlow, &dwhi, FILE_BEGIN);
    if (dwResult == INVALID_SET_FILE_POINTER)
    {
        errstring = L"could not set file pointer for localfile" + std::wstring(temp);
        transferstatus = ES_FAILED;
        result = E_LOCAL_FILE_NOTFOUND;
        Err = result;
        CloseLocalFile();
        return result;
    }

    block = (char*)malloc(blocksize + 1);
    if (block == nullptr) {
        Err = E_MEM_ALLOC;
        transferstatus = ES_FAILED;
        errstring = L"Could not allocate memory for file block size";
        CloseLocalFile();
        return E_MEM_ALLOC;
    }

    result = rdopen_existing_SFTPfile((char *)rfn);

    if (result == E_OK) {
        getSFTPfileinfo();
        filesize = rfilesize;
    }
    else
    {
        Err = E_REMOTE_FILE_OPEN;
        transferstatus = ES_FAILED;
        errstring = L"Could not open remote file";
        CloseLocalFile();
        delete block;
        return E_REMOTE_FILE_OPEN;
    }

    rc = sftp_seek64(file, transfered);
    if (rc != SSH_OK)
    {
        Err = E_REMOTEFILE_SEEK;
        transferstatus = ES_FAILED;
        errstring = L"Could not set pointer for remote file";
        CloseRemoteFile();
        CloseLocalFile();
        delete block;
        return E_REMOTEFILE_SEEK;
    }

    transferstatus = ES_RESUMING;

    sftp_file_set_blocking(file);

    transferstarttime = time(nullptr);
    transferstatus = ES_INPROGRESS;

    while (transferstatus != ES_FAILED && transferstatus != ES_PAUSED &&  transferstatus != ES_CANCELLED  &&transferstatus != ES_DONE)
    {
        loopcounter++;

        result = readSFTPfile(block, blocksize, (DWORD *)&bytesread);
        if (result != E_OK && result != E_SFTP_READ_EOF)
        {
            errstring = L"Error reading from remote sftp server file.";
            Err = result;
            transferstatus = ES_FAILED;
            CloseRemoteFile();
            CloseLocalFile();
            delete block;
            return result;
        }
        if (result == E_SFTP_READ_EOF)  transferstatus = ES_DONE;
        fprintf(logfile, "Read %d bytes from input file. Number of packets: %d , %llu from %llu bytes\n", bytesread, loopcounter, transfered, filesize);

        bresult = WriteFile(localfilehandle, (LPVOID)block, bytesread, &byteswritten, nullptr);
        if (byteswritten < bytesread)
        {
            if (bresult == FALSE)
            {
                errstring = L"Error writing to local file.";
                Err = E_LOCAL_FILE_RDWR;
                transferstatus = ES_FAILED;
                CloseRemoteFile();
                CloseLocalFile();
                delete block;
                return E_LOCAL_FILE_RDWR;
            }
            else if (bytesread == 0)
            {
                errstring = L"Transfer done.";
                Err = E_OK;
                transferstatus = ES_DONE;
                continue;
            }
        }

        Err = E_OK;

        if (pause == true) transferstatus = ES_PAUSED;
        if (bresult == TRUE &&  bytesread == 0)
        {
            // at the end of the file
            transferstatus = ES_DONE;
        }
        Sleep(blocktransferdelay);
        if (loopcounter % 71 == 0)Sleep(7 * blocktransferdelay);
        if (loopcounter % 331 == 0)Sleep(77 * blocktransferdelay);
        if (loopcounter % 3331 == 0)Sleep(777 * blocktransferdelay);
    }
    result = closeSFTPfile();
    CloseHandle(localfilehandle);
    Sleep(1000);

    if (transferstatus == ES_DONE)
    {
        wchar_t temp2[SHORT_BUFF_LEN];
        mbstowcs_s(&tempsize, temp2, lfn, SHORT_BUFF_LEN);
        bresult = MoveFile(temp, temp2);
        if (bresult != TRUE)
        {
            Err = E_RENAME_LOCAL_FILE;
            errstring = L"Could not rename local file: " + std::wstring(temp);
            transferstatus = ES_FAILED;
            delete block;
            return E_RENAME_LOCAL_FILE;
        }
    }

    if (transferstatus == ES_CANCELLED)
    {
        wchar_t temp2[SHORT_BUFF_LEN];
        mbstowcs_s(&tempsize, temp2, lfn, SHORT_BUFF_LEN);
        bresult = DeleteFile(temp);
        if (bresult != TRUE)
        {
            Err = E_LOCAL_DELETE_FILE;
            errstring = L"Could not rename local file: " + std::wstring(temp);
            transferstatus = ES_FAILED;
            delete block;
            return E_LOCAL_DELETE_FILE;
        }
    }

    delete block;
    return result;
}

//---------------------------------------

ESSHERR SFTPConnector::SFTPrename(char *newname, char *oldname)
{
    int rc = sftp_rename(sftp, oldname, newname);
    if (rc != SSH_OK) {
        return E_RENAME_ERR;
    }

    return E_OK;
}

ESSHERR SFTPConnector::SFTPdelete(char *remfile)
{
    int rc = sftp_unlink(sftp, remfile);
    if (rc != SSH_OK) {
        return E_DELETE_ERR;
    }

    return E_OK;
}

// SFTPreput
ESSHERR SFTPConnector::SFTPreput(char *lfn, char *rfn, size_t blocksize)
{
    ESSHERR     result;
    BOOL  bresult;
    DWORD bytesread;
    filesize = 0;
    transfered = 0;

    pause = false;
    transferstatus = ES_NONE;
    char *block;
    struct stat st;
    wchar_t temp[SHORT_BUFF_LEN];
    size_t tempsize;
    std::wstring wlfn;
    int loopcounter = 0;

    localfilename = lfn;
    //wlfn = wstring(lfn);
    //localfile = fopen(lfn, L"r");
    filename = rfn;
    mbstowcs_s(&tempsize, temp, lfn, SHORT_BUFF_LEN);

    //filesize = getFileSize(localfilename);

    /*if(filesize < 0) {
    transferstatus = ES_FAILED;
    Err = E_LOCAL_FILE_NOTFOUND;
    return E_LOCAL_FILE_NOTFOUND;
    }*/

    localfilehandle = CreateFile(temp, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (localfilehandle == INVALID_HANDLE_VALUE)
    {
        transferstatus = ES_FAILED;
        Err = E_LOCAL_FILE_NOTFOUND;
        return E_LOCAL_FILE_NOTFOUND;
    }
    local_file_size_lowDWORD = GetFileSize(localfilehandle, &local_file_size_hiDWORD);
    filesize = (local_file_size_hiDWORD * 0x100000000) + local_file_size_lowDWORD;

    if (filesize < 0) {
        transferstatus = ES_FAILED;
        Err = E_LOCAL_FILE_NOTFOUND;
        CloseLocalFile();
        return E_LOCAL_FILE_NOTFOUND;
    }

    block = (char*)malloc(blocksize + 1);
    if (block == nullptr) {
        Err = E_MEM_ALLOC;
        transferstatus = ES_FAILED;
        errstring = L"Could not allocate memory for file block size";
        CloseLocalFile();
        return E_MEM_ALLOC;
    }

    tempfilename = std::string(rfn) + ".sftp_temp";

    result = rwopen_existing_SFTPfile((char *)tempfilename.c_str());
    if (result == E_OK) {
        getSFTPfileinfo();
        sftp_seek64(file, rfilesize);
        uint64_t tempi64 = rfilesize & 0x00000000FFFFFFFF;
        DWORD dwlow = tempi64;
        tempi64 = (rfilesize & 0x7FFFFFFF00000000);
        tempi64 = tempi64 >> 32;
        long  dwhi = tempi64;
        DWORD dwResult = SetFilePointer(localfilehandle, dwlow, &dwhi, FILE_BEGIN);
        if (dwResult == INVALID_SET_FILE_POINTER)
        {
            transferstatus = ES_FAILED; Err = result;  return result;
        }
        transferstatus = ES_RESUMING;
        transfered = rfilesize;
        initially_was_transferred = rfilesize;
    }
    else {
        result = createSFTPfile((char *)tempfilename.c_str());
        transferstatus = ES_STARTING;
        rfilesize = 0;
        initially_was_transferred = 0;
        if (result != E_OK) {
            transferstatus = ES_FAILED;
            Err = result;
            CloseLocalFile();
            return result;
        }
    }
    sftp_file_set_blocking(file);
    transferstarttime = time(nullptr);
    transferstatus = ES_INPROGRESS;

    while (transferstatus != ES_FAILED && transferstatus != ES_PAUSED && transferstatus != ES_DONE)
    {
        loopcounter++;
        bresult = ReadFile(localfilehandle, (LPVOID)block, blocksize, &bytesread, nullptr);
        fprintf(logfile, "Read %d bytes from input file. Number of packets: %d , %llu from %llu bytes\n", bytesread, loopcounter, transfered, filesize);
        if (bytesread < blocksize)
        {
            if (bresult == FALSE)
            {
                errstring = L"Error reading from local file.";
                Err = E_LOCAL_FILE_READ;
                transferstatus = ES_FAILED;
                CloseRemoteFile();
                CloseLocalFile();
                return E_LOCAL_FILE_READ;
            }
            else if (bytesread == 0)
            {
                errstring = L"Transfer done.";
                Err = E_OK;
                transferstatus = ES_DONE;
                continue;
            }
        }

        result = writeSFTPfile(block, bytesread);
        if (result != E_OK && bytesread > 0)
        {
            errstring = L"Error transmitting to remote sftp server file.";
            Err = result;
            transferstatus = ES_FAILED;
            CloseRemoteFile();
            CloseLocalFile();
            return result;
        }

        Err = E_OK;
        //transfered = transfered + bytesread;
        if (pause == true) transferstatus = ES_PAUSED;
        if (bresult == TRUE &&  bytesread == 0)
        {
            // at the end of the file
            transferstatus = ES_DONE;
        }
        Sleep(blocktransferdelay);
        if (loopcounter % 71 == 0)Sleep(7 * blocktransferdelay);
        if (loopcounter % 331 == 0)Sleep(77 * blocktransferdelay);
        if (loopcounter % 3331 == 0)Sleep(777 * blocktransferdelay);
    }

    CloseRemoteFile();
    CloseLocalFile();
    Sleep(1000);

    if (transferstatus == ES_CANCELLED)
    {
        result = SFTPdelete((char *)tempfilename.c_str());
        if (bresult != E_OK)
        {
            Err = E_DELETE_ERR;
            errstring = L"Could not delete remote file.";
            transferstatus = ES_FAILED;
            return E_DELETE_ERR;
        }
    }
    if (transferstatus == ES_DONE) result = SFTPrename(rfn, (char *)tempfilename.c_str());
    delete block;
    return result;
}

ESSHERR SFTPConnector::getSFTPfileinfo()
{
    sftp_attributes fileinf = sftp_fstat(file);

    if (fileinf == nullptr) {
        return E_GET_FILEINF;
    }

    rfilesize = fileinf->size;

    sftp_attributes_free(fileinf);
    return E_OK;
}

ESSHERR SFTPConnector::closeSFTPfile()
{
    int rc = sftp_close(file);
    if (rc != SSH_OK)
    {
        fprintf(logfile, "Can't close the written file: %s\n",
            ssh_get_error(session));
        return E_FILE_CLOSE;
    }
    return E_OK;
}

ESSHERR SFTPConnector::writeSFTPfile(char *block, size_t blocksize)
{
    size_t nwritten = sftp_write(file, block, blocksize);
    if (nwritten != blocksize)
    {
        fprintf(logfile, "Can't write data to file: %s\n",
            ssh_get_error(session));
        //sftp_close(file);
        transfered = transfered + nwritten;
        return E_WRITE_ERR;
    }

    transfered = transfered + nwritten;
    return E_OK;
}
ESSHERR SFTPConnector::readSFTPfile(char *block, uint64_t len, DWORD *bytesread)
{
    DWORD readbytes;
    *bytesread = 0;
    if (len <= 0) return E_INVALID_PARAMS;
    if (bytesread == nullptr || block == nullptr) return E_INVALID_PARAMS;

    readbytes = sftp_read(file, block, len);
    if (readbytes < 0)
    {
        fprintf(logfile, "Can't read  from  remote file: %s  %s\n", filename.c_str(), ssh_get_error(session));
        *bytesread = 0;
        return E_SFTP_READ_ERR;
    }

    if (readbytes < len)
    {
        *bytesread = readbytes;
        return E_SFTP_READ_EOF;
    }

    *bytesread = readbytes;
    transfered = transfered + readbytes;

    return E_OK;
}

ESSHERR SFTPConnector::createSFTPfile(char *fn)
{
    int access_type = O_CREAT | O_RDWR | O_APPEND;
    int rc, nwritten;

    filename = std::string(fn);
    file = sftp_open(sftp, fn, access_type, S_IREAD | S_IWRITE);
    if (file == nullptr)
    {
        fprintf(logfile, "Can't open file for writing: %s\n",
            ssh_get_error(session));
        return E_FILEOPEN_WRITE;
    }
    return E_OK;
}

ESSHERR SFTPConnector::rdopen_existing_SFTPfile(char *fn)
{
    int access_type = O_RDONLY;
    int rc, nwritten;

    filename = std::string(fn);
    file = sftp_open(sftp, fn,
        access_type, S_IREAD);
    if (file == nullptr)
    {
        fprintf(logfile, "Can't open file for writing: %s\n",
            ssh_get_error(session));
        return E_FILEOPEN_RDONLY;
    }
    return E_OK;
}

ESSHERR SFTPConnector::rwopen_existing_SFTPfile(char *fn)
{
    int access_type = O_RDWR;
    int rc, nwritten;

    filename = std::string(fn);
    file = sftp_open(sftp, fn,
        access_type, S_IREAD);
    if (file == nullptr)
    {
        fprintf(logfile, "Can't open file for writing: %s\n",
            ssh_get_error(session));
        return E_FILEOPEN_RDONLY;
    }
    return E_OK;
}

ESSHERR SFTPConnector::openSFTPfile(char *fn)
{
    int access_type = O_RDONLY;
    int rc, nwritten;

    filename = std::string(fn);
    file = sftp_open(sftp, fn,
        access_type, S_IWRITE);
    if (file == nullptr)
    {
        fprintf(logfile, "Can't open file for writing: %s\n",
            ssh_get_error(session));
        return E_FILE_OPEN_READ;
    }
    return E_OK;
}

ESSHERR SFTPConnector::Makedir(char *newdir)
{
    int rc;
    rc = sftp_mkdir(sftp, newdir, S_IFDIR);
    if (rc != SSH_OK)
    {
        if (sftp_get_error(sftp) != SSH_FX_FILE_ALREADY_EXISTS)
        {
            fprintf(logfile, "Can't create directory: %s\n",
                ssh_get_error(session));
            return E_CREATE_DIR;
        }
    }

    return E_OK;
}

SFTPConnector::SFTPConnector()
{
    //libssh2_init(0);
    session = ssh_new();
    if (session == nullptr)
    {
        Err = E_SESSION_ALOC;
        errstring = L"Could not allocate a session.";
    }
    wcscpy(hostname, L"localhost");
    wcscpy(username, L"User");
    wcscpy(password, L"Password");
    wcscpy(basedir, L".\\");
    port = 22;
    verbosity = SSH_LOG_RARE;
    filesize = 0;
    transfered = 0;

    pause = false;
    transferstatus = ES_NONE;
    logfile = stderr;
    blocktransferdelay = INITIALBLOCKTRANSDELAY;
}

SFTPConnector::SFTPConnector(wchar_t *dir, wchar_t *hn, int hostport, wchar_t *un, wchar_t *pass)
{
    //libssh2_init(0);
    session = ssh_new();

    if (session == nullptr)
    {
        Err = E_SESSION_ALOC;
        errstring = L"Could not allocate a session.";
    }
    wcscpy(hostname, hn);
    wcscpy(username, un);
    wcscpy(password, pass);
    wcscpy(basedir, dir);
    port = hostport;
    verbosity = SSH_LOG_RARE;
    filesize = 0;
    transfered = 0;

    pause = false;
    transferstatus = ES_NONE;
    logfile = stderr;
    blocktransferdelay = INITIALBLOCKTRANSDELAY;
}

ESSHERR  SFTPConnector::InitSFTP()
{
    int rc;
    sftp = sftp_new(session);
    if (session == nullptr)
    {
        Err = E_SFTP_ALLOC;
        errstring = L"Could not allocate a sftp session.";
    }

    rc = sftp_init(sftp);
    if (rc != SSH_OK)
    {
        fprintf(logfile, "Error initializing SFTP session: %s.\n",
            sftp_get_error(sftp));
        sftp_free(sftp);
        return E_INIT_SFTP;
    }

    return E_OK;
}

ESSHERR  SFTPConnector::ConnectSession()
{
    char temp[SHORT_BUFF_LEN];
    size_t n_of_chars;
    wcstombs_s(&n_of_chars, temp, SHORT_BUFF_LEN, (const wchar_t *)password, SHORT_BUFF_LEN);
    int ir;

    ir = ssh_connect(session);
    if (ir != SSH_OK) {
        errstring = L"Could not connect the ssh session.";
        return E_SSH_CONNECT_ERR;
    }

    ir = ssh_userauth_password(session, nullptr, temp);
    if (ir != SSH_OK) {
        errstring = L"Could not authenticate with the ssh server.\r\n";
        return E_AUTHENTICATE;
    }
    return E_OK;
}

ESSHERR  SFTPConnector::InitSession()
{
    char temp[SHORT_BUFF_LEN];
    size_t n_of_chars;
    wcstombs_s(&n_of_chars, temp, SHORT_BUFF_LEN, (const wchar_t *)hostname, SHORT_BUFF_LEN);
    ssh_options_set(session, SSH_OPTIONS_HOST, temp);
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    wcstombs_s(&n_of_chars, temp, SHORT_BUFF_LEN, (const wchar_t *)username, SHORT_BUFF_LEN);
    ssh_options_set(session, SSH_OPTIONS_USER, temp);
    wcstombs_s(&n_of_chars, temp, SHORT_BUFF_LEN, (const wchar_t *)basedir, SHORT_BUFF_LEN);
    ssh_options_set(session, SSH_OPTIONS_SSH_DIR, temp);

    return E_OK;
}

void  SFTPConnector::setVerbosity(int v)
{
    verbosity = v;
}

int  SFTPConnector::getVerbosity()
{
    return verbosity;
}

TTransStat* SFTPConnector::getStatus()
{
    if (transferstatus == ES_INPROGRESS)
    {
        try
        {
            stats.seconds_elapsed = time(nullptr);
            stats.seconds_elapsed -= transferstarttime;
            if (stats.seconds_elapsed != 0)
                stats.averagebps = ((transfered - initially_was_transferred) * 8) / stats.seconds_elapsed;
        }
        catch (const std::exception&)
        {
            stats.averagebps = 0;
        }
    }
    if (filesize > 0 && stats.averagebps != 0) {
        stats.percent = (transfered * 100) / filesize;
        stats.seconds_remained = ((filesize - transfered) * 8) / stats.averagebps;
    }
    else
    {
        stats.percent = -1;
        stats.seconds_remained = -1;
    }
    stats.total_size = filesize;
    stats.transferstate = transferstatus;
    stats.remote_file_name = filename;
    stats.local_file_name = localfilename;
    stats.transferred = transfered;
    stats.initially_transferred = initially_was_transferred;

    return &stats;
}

int SFTPConnector::sftp_list_dir(const std::string file_directory_server)
{
    sftp_dir dir;
    sftp_attributes attributes;
    int rc;

    dir = sftp_opendir(sftp, file_directory_server.c_str());

    if (!dir)
    {
        fprintf(stderr, "Directory not opened: %s\n",
            ssh_get_error(session));
        return SSH_ERROR;
    }
    printf("Name                       Size Perms    Owner\tGroup\n");
    file_name_size.clear();
    while ((attributes = sftp_readdir(sftp, dir)) != nullptr)
    {
        std::string file_name;
        int file_size;
        std::tuple<std::string, int> data_pair;

        file_name = std::string(attributes->name);
        file_size = ((long long unsigned int(attributes->size)) / 1024);
        data_pair = make_tuple(file_name, file_size);

        file_name_size.push_back(data_pair);
        printf("%-20s %10llu %.8o %s(%d)\t%s(%d)\n",
            attributes->name,
            (long long unsigned int) attributes->size,
            attributes->permissions,
            attributes->owner,
            attributes->uid,
            attributes->group,
            attributes->gid);
        sftp_attributes_free(attributes);
    }
    if (!sftp_dir_eof(dir))
    {
        fprintf(stderr, "Can't list directory: %s\n",
            ssh_get_error(session));
        sftp_closedir(dir);
        return SSH_ERROR;
    }
    rc = sftp_closedir(dir);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Can't close directory: %s\n",
            ssh_get_error(session));
        return rc;
    }
}

SFTPConnector::~SFTPConnector()
{
    sftp_free(sftp);
    ssh_disconnect(session);
    ssh_free(session);

    return;
}

} // namespace sftp
} // namespace fb
