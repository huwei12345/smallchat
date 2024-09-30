#include "ftpmanager.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QDirIterator>
#include <QTimer>
#include <QTime>
#include "globalvaria.h"
struct sFtpData
{
    int id = 0;
    QFile *downloadFile = nullptr;
    QFile *uploadFile = nullptr;
    QFtp *ftp = nullptr;
    QTimer *timer = nullptr;
    QFtp::State mState;
    //FileInfo* info;
};


FtpManager *FtpManager::mFtpManager = nullptr;

FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    d = new sFtpData;
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
    delete d;
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
    d->uploadFile = new QFile(localPath);

    if(d->uploadFile->open(QIODevice::ReadOnly))
    {
        //TODO:是否需要切换目录，还是直接带目录发送就可以
        d->ftp->put(d->uploadFile,QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName));
    }
}

void FtpManager::renameFile(const QString &oldName, const QString &newName)
{
    if(oldName.isEmpty() || newName.isEmpty())
        return;
    if(isConnected())
        d->ftp->rename(oldName,newName);
    else
        emit sigRename(false);
}

void FtpManager::deleteFile(const QString &fileName)
{
    if(fileName.isEmpty())
        return;
    if(isConnected())
        d->ftp->remove(fileName);
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
    return d->mState;
}

void FtpManager::ftpCommandFinished(int cmdId, bool error)
{
    if(!d->ftp)
        return;

    switch (d->ftp->currentCommand()) {
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
        if(d->downloadFile && d->downloadFile->isOpen())
        {
            emit FileGetOver(d->downloadFile->fileName());
            d->downloadFile->close();
            delete d->downloadFile;
            d->downloadFile = nullptr;
        }
    }break;
    case QFtp::Put:
    {
        if(error)
            emit sigUploaded(false);
        else
            emit sigUploaded(true);
        if(d->uploadFile && d->uploadFile->isOpen())
        {
            emit ftpFileSendOver(d->downloadFile->fileName());
            d->uploadFile->close();
            delete d->uploadFile;
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
    if(!d->ftp)
        return;
    d->mState = d->ftp->state();
    switch (d->ftp->state()) {
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
    if(!d->ftp)
    {
        d->ftp = new QFtp(this);
        QObject::connect(d->ftp, SIGNAL(commandFinished(int,bool)),this, SLOT(ftpCommandFinished(int,bool)));
        QObject::connect(d->ftp, SIGNAL(stateChanged(int)),this, SLOT(ftpStateChanged(int)));
        QObject::connect(d->ftp, SIGNAL(listInfo(QUrlInfo)),this, SLOT(addToList(QUrlInfo)));
        QObject::connect(d->ftp, SIGNAL(dataTransferProgress(qint64,qint64)),this, SLOT(updateDataTransferProgress(qint64,qint64)));
    }
    d->ftp->connectToHost(Ftp_Ip,Ftp_Port);
    d->ftp->login(Ftp_Username,Ftp_Password);
}

void FtpManager::disconnectFtp()
{
    if(d->ftp)
    {
        qDebug() << "Ftp Connected Close!";
        d->ftp->state();
        d->ftp->abort();
        d->ftp->deleteLater();
        d->ftp = nullptr;
        return;
    }
}


void FtpManager::downloadFile(FileInfo& info)
{

    QString localPath = QString::fromStdString(info.ClientPath);
    qDebug() << "localPath " << localPath;
    d->downloadFile = new QFile(localPath);
    bool ret = d->downloadFile->open(QIODevice::WriteOnly);
    if(ret)
    {
        d->ftp->get(QString::fromStdString(info.serverPath) + QString::fromStdString(info.serverFileName), d->downloadFile);
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
