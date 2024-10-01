#include "ftpmanager.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QDirIterator>
#include <QTimer>
#include <QTime>
#include "globalvaria.h"

FtpManager *FtpManager::mFtpManager = nullptr;

FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
}

FtpManager::FtpManager(QString serverIp, unsigned short port, QString user, QString password, QObject *parent) :
    FtpManager(parent)
{
    Ftp_Ip = serverIp;
    Ftp_Port = port;
    Ftp_Username = user;
    Ftp_Password = password;
}

FtpManager::~FtpManager()
{
    disconnectFtp();
}

FtpManager *FtpManager::getInstance()
{
    if(mFtpManager == nullptr)
    {
        static QMutex mutex;
        mutex.lock();
        mFtpManager = new FtpManager;
        mutex.unlock();
    }

    return mFtpManager;
}

//TODO:使用id串联整个文件传输流程

void FtpManager::uploadFile(FileInfo& info)
{
    if(!isConnected()) {
        emit sigUploaded(false);
        return;
    }
    QString localPath = QString::fromStdString(info.ClientPath);
    qDebug() << "localPath: " << localPath;
    sFtpData data;
    data.uploadFile = new QFile(localPath);
    data.mInfo = info;
    if(data.uploadFile->open(QIODevice::ReadOnly))
    {
        //TODO:是否需要切换目录，还是直接带目录发送就可以
        int cmdId = mFtp->put(data.uploadFile,QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName));
        mTaskMap[cmdId] = data;
    }
}


void FtpManager::downloadFile(FileInfo& info)
{

    QString localPath = QString::fromStdString(info.ClientPath);
    qDebug() << "localPath " << localPath;
    sFtpData data;
    data.downloadFile = new QFile(localPath);
    data.mInfo = info;
    bool ret = data.downloadFile->open(QIODevice::WriteOnly);
    if(ret)
    {
        int cmdId = mFtp->get(QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName), data.downloadFile);
        mTaskMap[cmdId] = data;
    }
}


void FtpManager::renameFile(const QString &oldName, const QString &newName)
{
    if(oldName.isEmpty() || newName.isEmpty())
        return;
    if(isConnected())
        mFtp->rename(oldName,newName);
    else
        emit sigRename(false);
}

void FtpManager::deleteFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;
    if(isConnected())
        mFtp->remove(fileName);
    else
        emit sigDelete(false);
}

bool FtpManager::isConnected() const
{
    if(ftpState() == QFtp::Connected || ftpState() == QFtp::LoggedIn)
        return true;
    else
        return false;
}

QFtp::State FtpManager::ftpState() const
{
    return mFtp->state();
}

void FtpManager::ftpCommandFinished(int cmdId, bool error)
{
    sFtpData data = mTaskMap[cmdId];
    if(!mFtp)
        return;
    switch (mFtp->currentCommand()) {
    case QFtp::ConnectToHost:
    {
        if(error)
            disconnectFtp();
    }break;
    case QFtp::Login:
    {
        if(error)
            return;
    }break;
    case QFtp::Cd:
    {
        if(error)
            return;
    }break;
    case QFtp::List:
    {
        if(error)
            return;
    }break;
    case QFtp::Get:
    {
        if(data.downloadFile && data.downloadFile->isOpen())
        {
            emit FileGetOver(mTaskMap[cmdId].mInfo);
            data.downloadFile->close();
            delete data.downloadFile;
            data.downloadFile = nullptr;
            mTaskMap.erase(cmdId);
        }
    }break;
    case QFtp::Put:
    {
        if(error)
            emit sigUploaded(false);
        else
            emit sigUploaded(true);
        if(data.uploadFile && data.uploadFile->isOpen())
        {
            emit ftpFileSendOver(mTaskMap[cmdId].mInfo);
            data.uploadFile->close();
            delete data.uploadFile;
            mTaskMap.erase(cmdId);
        }
    }break;
    case QFtp::Rename:
    {
        if(error)
            emit sigRename(true);
        else
            emit sigRename(false);

    }break;
    case QFtp::Remove:
    {
        if(error)
            emit sigDelete(true);
        else
            emit sigDelete(false);
    }break;
    default:
        break;
    }
}

void FtpManager::ftpStateChanged(int state)
{
    if(!mFtp)
        return;
    mFtpState = mFtp->state();
    switch (mFtp->state()) {
    case QFtp::Connecting:
    {
        qDebug() << "ftp connecting";
    }break;
    case QFtp::Connected:
    {
        qDebug() << "ftp connected";
    }break;
    case QFtp::Unconnected:
    {
        qDebug() << "ftp Unconnected";
    }break;
    default:
        break;
    }
}

void FtpManager::addToList(const QUrlInfo &urlInfo)
{
    if(urlInfo.isFile())
    {
        // 文件名称通过urlInfo.name()获取;
    }
}

void FtpManager::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    emit sigUpdateDataTransferProgress(readBytes,totalBytes);
}

void FtpManager::connectFtp()
{
    if(!mFtp)
    {
        mFtp = new QFtp(this);
        QObject::connect(mFtp, SIGNAL(commandFinished(int,bool)),this, SLOT(ftpCommandFinished(int,bool)));
        QObject::connect(mFtp, SIGNAL(stateChanged(int)),this, SLOT(ftpStateChanged(int)));
        QObject::connect(mFtp, SIGNAL(listInfo(QUrlInfo)),this, SLOT(addToList(QUrlInfo)));

        QObject::connect(mFtp, SIGNAL(dataTransferProgress(qint64,qint64)),this, SLOT(updateDataTransferProgress(qint64,qint64)));
    }
    mFtp->connectToHost(Ftp_Ip,Ftp_Port);
    mFtp->login(Ftp_Username,Ftp_Password);
}

void FtpManager::disconnectFtp()
{
    if(mFtp)
    {
        qDebug() << "Ftp Connected Close!";
        mFtp->state();
        mFtp->abort();
        mFtp->deleteLater();
        mFtp = nullptr;
        return;
    }
}

/*
1、保持与FTP服务器的长连接，需要进行一次FTP操作，我这里使用定时器隔20S获取一次timer.txt文件，文件内容为空。

2、QFtp重连的方式，必须先进行disconnect，调用abort()和deletelater()，然后将指针赋值为nullptr。

3、在断开QFtp连接之后，若剩余的操作未结束，返回ftpCommandFinished或ftpStateChanged，注意！！！此时QFtp指针已经被delete，需要对Qftp对象进行判空处理，若为nullptr则return，否则访问空指针导致崩溃。

4、QFtp的文件操作，是异步处理，若需要断开连接，请在创建QFtp的线程进行断开。

5、ftpCommandFinished和ftpStateChanged槽函数触发时，不要使用槽函数的形参，应使用QFtp对象的函数command()和state()来获取当前命令与当前状态。
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。

原文链接：https://blog.csdn.net/qq_45413837/article/details/135079870
*/
