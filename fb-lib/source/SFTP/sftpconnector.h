#ifndef SFTPCONNECTOR_H
#define SFTPCONNECTOR_H

#include <string>
#include <vector>
#include <tuple>
#include <time.h>
#include <fcntl.h>
#include <libssh\libssh.h>
#include <libssh\sftp.h>

//  constant to limit length of character strings
const unsigned int SHORT_BUFF_LEN = 1024;
// milliseconds, this is the delay that we put after
// each block transfer to make it posssible for network to absorb the data.
const unsigned int INITIALBLOCKTRANSDELAY = 10;

namespace fb {
namespace sftp {

typedef enum sshconerr {
    E_OK = 1, E_SESSION_ALOC = -1, E_SSH_CONNECT_ERR = -2, E_SFTP_ALLOC = -3, E_INIT_SFTP = -4, E_CREATE_DIR = -5, E_FILEOPEN_WRITE = -6, E_WRITE_ERR = -7
    , E_FILE_CLOSE = -8, E_FILE_OPEN_READ = -9, E_INVALID_PARAMS = -10, E_SFTP_ERR = -11, E_SFTP_READ_ERR = -12, E_SFTP_READBYTES_ERR = -13, E_GET_FILEINF = -14
    , E_LOCAL_FILE_NOTFOUND = -15, E_RENAME_ERR = -16, E_MEM_ALLOC = -17, E_LOCAL_FILE_READ = -18, E_LOCAL_FILE_RDWR = -19, E_REMOTEFILE_SEEK = -20
    , E_REMOTE_FILE_OPEN = -21, E_DELETE_ERR = -22, E_RENAME_LOCAL_FILE = -23, E_LOCAL_DELETE_FILE = -24, E_FILEOPEN_RDONLY = -25, E_SFTP_READ_EOF = -26
    , E_AUTHENTICATE = -27, E_UNKNOWN = -999
} ESSHERR;

// status of transfers;
typedef enum sftpstat { ES_DONE = 0, ES_INPROGRESS, ES_FAILED, ES_STARTING, ES_PAUSED, ES_RESUMING, ES_CANCELLED, ES_NONE } ESFTPSTAT;

// statistics about the transfer;
typedef struct transferstatstruct {
    std::string remote_file_name;
    std::string local_file_name;
    unsigned int initially_transferred;
    uint64_t total_size;
    uint64_t transferred;
    uint64_t averagebps;
    uint64_t seconds_elapsed;
    uint64_t seconds_remained;
    int percent;
    ESFTPSTAT transferstate;
} TTransStat;

class SFTPConnector
{
private:
    ssh_session  session;                 // ssh session
    sftp_session sftp;                    // sftp session
    sftp_file file;                       // structure for a remote file
    FILE *localfile;                      // not used in windows but could be local file pointer in UNIX
    FILE *logfile;                        // the file for writing logs, default is set to stderr
    std::string filename;                 // file name of the transfer;
    std::string localfilename;            // file name of local file;
    std::string tempfilename;             // a temporaty file name will be used during the transfer which is renamed when transfer is completed.
    ESFTPSTAT transferstatus;             // state of the transfer which has one of the above values (ESFTPSTAT)
    std::vector<std::tuple<std::string, int>> file_name_size;
    std::vector< std::tuple<int>>transfer_file_size;

    uint64_t transferstarttime;              // time of start of the transfer
    wchar_t username[SHORT_BUFF_LEN];
    wchar_t password[SHORT_BUFF_LEN];
    wchar_t hostname[SHORT_BUFF_LEN];       // hostname of the sftp server
    wchar_t basedir[SHORT_BUFF_LEN];        // this base dir is the directory of public and private key structur ( NOT USED IN THIS VERSION)
    int port;                               //  port of the server;
    int verbosity;                          //  degree of verbosity of libssh
    uint64_t  filesize;                      //  total number of bytes to be transfered;
    DWORD  local_file_size_hiDWORD;         //  Bill Gates cannot accept the file size without twisting the programmers, so he accepts them in 2 separate words like this
    DWORD  local_file_size_lowDWORD;        //  these 2 DWORDs when connected together comprise a 64 bit file size.
    uint64_t lfilesize;                          // local file size
    uint64_t rfilesize;                          // remote file size
    uint64_t transfered;                         // number of bytes already transfered
    uint64_t initially_was_transferred;          // this is the number of bytes which was transferred before pause or interrupt of a transfer and used when resuming a transfer.
    bool pause;                                 // pause flag
    TTransStat stats;                           // statistics of the transfer
    HANDLE  localfilehandle;                    // windows uses handles to manipulate files. this is the handle to local file.
    int blocktransferdelay;

    ESSHERR rwopen_existing_SFTPfile(char *fn);         // open a file on remote ( server ) read/write   for upload
    ESSHERR rdopen_existing_SFTPfile(char *fn);         // open a file on remote ( server ) read only    for download
    ESSHERR createSFTPfile(char *fn);                                   // create a file on server;
    ESSHERR writeSFTPfile(char *block, size_t blocksize);               // write a block of data to the  open remote file
    //ESSHERR readSFTPfile(char *block, size_t len, size_t *bytesread);   // read a block of data from the  open remote file
    ESSHERR readSFTPfile(char *block, uint64_t len, DWORD *bytesread);
    ESSHERR closeSFTPfile();        // closes the remote file;
    ESSHERR openSFTPfile(char *fn);  // opens the remote file
    ESSHERR getSFTPfileinfo();          // gets information about the remote file

public:
    std::wstring errstring;      // the string describing last error
    ESSHERR Err;                // error code of last error
    SFTPConnector();           // default constructor;
    ~SFTPConnector();           // default constructor;
    SFTPConnector(wchar_t *dir, wchar_t *hn, int hostport, wchar_t *un, wchar_t *pass);  // constructor
    void setVerbosity(int v);
    int  getVerbosity();
    ESSHERR InitSession();          // must be called befor doing any transfer
    ESSHERR ConnectSession();       // connnects to the  ssh server
    ESSHERR InitSFTP();     // must be called befor doing any transfer
    ESSHERR Makedir(char *newdir);
    //ESSHERR testUploadFile(char *fn, char *block);          // do not use this , only for test purposes for myself
    //ESSHERR SFTPput(char *lfn, char *rfn, size_t blocksize);            // Upload a file from start
    ESSHERR SFTPreput(char *lfn, char *rfn, size_t blocksize);          // checks for previouse interrupted transfer, then either continues the previouse transfer ( if there was any) or starts a new one (UPLOAD)
    ESSHERR SFTPrename(char *newname, char *oldname);                   // renames a remote file( must be closed)
    ESSHERR SFTPdelete(char *remfile);                  // deletes a remote file
    TTransStat *getStatus();                                                // gets statistics of the transfer
    //ESSHERR CSFTPConnector::SFTPget(char *lfn, char *rfn, size_t blocksize);    // Downloads a file from  sftp server
    ESSHERR SFTPreget(char *lfn, char *rfn, size_t blocksize);  // checks for previouse interrupted transfer, then either continues the previouse transfer ( if there was any) or starts a new one (DOWNLOAD)
    void CancelTransfer();
    void PauseTransfer();
    void setLogFile(FILE *logf);         // sets the log file, if not set stderr will be used. by default.
    void CloseLocalFile();
    void CloseRemoteFile();
    int GetSessionStatus();
    bool  IsConnected();
    void setBlockTransferDelay(int miliseconds);
    int sftp_list_dir(const std::string file_directory_server);
    ESFTPSTAT getTransferStatus();
};

} // namespace sftp
} // namespace fb


#endif // SFTPCONNECTOR_H
