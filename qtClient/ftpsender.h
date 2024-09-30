#ifndef FTPSENDER_H
#define FTPSENDER_H
#include <QObject>
#include <string>
#include <queue>
#include <QThread>
#include "Protocol.h"
class FtpManager;
class ClientNetWork;

class FtpSender : public QThread
{
    Q_OBJECT
public:
    static FtpSender* GetInstance();
    void Run();
    void ProcessSendList(FileInfo &info);
    void ProcessGetList(FileInfo &info);

    void SendFile(FileInfo &info);
    void GetFile(FileInfo &info);

    void addFile(FileInfo& info);
    void removeFile(FileInfo& info);
    FileInfo file(int id);
signals:

private:
    FtpSender();
    static FtpSender* ftpSender;
    std::queue<FileInfo> mFtpSendList;
    std::queue<FileInfo> mFtpGetList;
    FtpManager* ftpUtil;
    std::map<int, FileInfo> mCurrentFileMap;
protected:
    // 在这里定义线程运行时需要执行的任务
    void run() override {
        Run();
    }
};

#endif // FTPSENDER_H
