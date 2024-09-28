#include "ftpsender.h"
#include "ftp/FtpManager.h"
#include "network.h"
#include "globalvaria.h"
FtpSender::FtpSender()
{
    connect(this, &FtpSender::ftpFileSendOver, ClientNetWork::GetInstance(), &ClientNetWork::ftpFileSendOver);
    ftpUtil = new FtpManager(GlobalVaria::GetInstance()->ftpIp(),
                             GlobalVaria::GetInstance()->ftpPort(),
                             GlobalVaria::GetInstance()->user(),
                             GlobalVaria::GetInstance()->password(),
                             this);
    ftpUtil->connectFtp();
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
        ftpUtil->uploadFile(QString::fromStdString(info.path) + QString::fromStdString(info.filename));
        if (ret) {
            emit ftpFileSendOver(QString::fromStdString(info.filename));
        }
    }
}

void FtpSender::ProcessGetList(FileInfo& info) {
    if (1) {
        ftpUtil->downloadFile(QString::fromStdString(info.path) + QString::fromStdString(info.filename));
    }
}

void FtpSender::Run() {
    while (1) {
        if (!mFtpSendList.empty()) {
            FileInfo info = mFtpSendList.front();
            mFtpSendList.pop();
            if (ftpUtil->ftpState() != QFtp::Connected) {
                //ftpUtil->connect();
                ftpUtil->connectFtp();
                QThread::sleep(1);
            }
            if (ftpUtil->ftpState() == QFtp::Connected) {
                ProcessSendList(info);
            }
        }
        if (!mFtpGetList.empty()) {
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
