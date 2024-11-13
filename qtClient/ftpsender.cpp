#include "ftpsender.h"
#include "ftp/FtpManager.h"
#include "network.h"
#include "globalvaria.h"
FtpSender::FtpSender()
{
    ftpUtil = new FtpManager(GlobalVaria::GetInstance()->ftpIp(),
                             GlobalVaria::GetInstance()->ftpPort(),
                             GlobalVaria::GetInstance()->user(),
                             GlobalVaria::GetInstance()->password(),
                             this);
    ftpUtil->connectFtp();

    //FTP文件上传下载的成功，不是服务器确认文件操作的成功，后续可能服务器还需要确认响应和数据库操作
    connect(ftpUtil, &FtpManager::ftpFileSendOver, ClientNetWork::GetInstance(), &ClientNetWork::ftpFileSendOver);
    connect(ftpUtil, &FtpManager::FileGetOver, ClientNetWork::GetInstance(), &ClientNetWork::ftpFileGetOver);
}

FtpSender* FtpSender::ftpSender = NULL;

FtpSender *FtpSender::GetInstance()
{
    if (ftpSender == NULL) {
        ftpSender = new FtpSender;
    }
    return ftpSender;
}

void FtpSender::ProcessSendList(FileInfo& info) {
    if (1) {
        bool ret = true;
        qDebug() << "ProcessSendList";
        //QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName)
        ftpUtil->uploadFile(info);
    }
}

void FtpSender::ProcessGetList(FileInfo& info) {
    if (1) {
        //QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName)
        ftpUtil->downloadFile(info);
    }
}

void FtpSender::Run() {
    while (1) {
        if (!mFtpSendList.empty()) {
            FileInfo info = mFtpSendList.front();
            mFtpSendList.pop();
            if (ftpUtil->ftpState() != QFtp::Connected) {
                //ftpUtil->connect();
//                ftpUtil->connectFtp();
//                QThread::sleep(1);
            }
            ProcessSendList(info);
        }
        if (!mFtpGetList.empty()) {
            qDebug() << "mFtpGetList" << mFtpGetList.size();
            FileInfo info = mFtpGetList.front();
            mFtpGetList.pop();
            qDebug() << "Will Get Ftp File .......................";
            ProcessGetList(info);
        }
        QThread::usleep(1000);
    }
}

void FtpSender::SendFile(FileInfo& info) {
    mFtpSendList.push(info);
}

void FtpSender::GetFile(FileInfo& info) {
    mFtpGetList.push(info);
}

void FtpSender::close()
{
    ftpUtil->disconnectFtp();
}

void FtpSender::start()
{
    ftpUtil->connectFtp();
}

void FtpSender::addFile(FileInfo &info)
{
    if (mCurrentFileMap.count(info.ftpTaskId) == 0) {
        mCurrentFileMap[info.ftpTaskId] = info;
    }
    else {
        qDebug() << "Error Push Ftp File Id : " << info.ftpTaskId;
    }
}



void FtpSender::removeFile(FileInfo &info)
{
    if (mCurrentFileMap.count(info.ftpTaskId) == 0) {
        mCurrentFileMap.erase(info.ftpTaskId);
    }
    else {
        qDebug() << "Error Remove Ftp File Id : " << info.ftpTaskId;
    }
}

FileInfo FtpSender::file(int id)
{
    if (mCurrentFileMap.count(id) != 0) {
        return mCurrentFileMap[id];
    }
    else {
        qDebug() << "Error Get Ftp File Id : " << id;
    }
    return FileInfo();
}
