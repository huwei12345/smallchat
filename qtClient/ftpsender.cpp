﻿#include "ftpsender.h"
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

void FtpSender::addFile(FileInfo &info)
{
    if (mCurrentFileMap.count(info.id) == 0) {
        mCurrentFileMap[info.id] = info;
    }
    else {
        qDebug() << "Error Push Ftp File Id : " << info.id;
    }
}

void FtpSender::removeFile(FileInfo &info)
{
    if (mCurrentFileMap.count(info.id) == 0) {
        mCurrentFileMap.erase(info.id);
    }
    else {
        qDebug() << "Error Remove Ftp File Id : " << info.id;
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
