#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include "qftp.h"
#include "qurlinfo.h"
#include <QString>
#include "Protocol.h"
#include <QFile>
#include <QTimer>

struct sFtpData
{
    int id = 0;
    QFile *downloadFile = nullptr;
    FileInfo mInfo;
    QFile *uploadFile = nullptr;
    //FileInfo* info;
};

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);
    explicit FtpManager(QString serverIp, unsigned short port, QString user, QString password, QObject *parent);
    ~FtpManager();

private:

private:
    //sFtpData *d;
    static FtpManager *mFtpManager;

public:
    static FtpManager *getInstance();
    void connectFtp();
    void disconnectFtp();

    void uploadFile(FileInfo &info);
    void downloadFile(FileInfo &info);

    void renameFile(const QString &oldName, const QString &newName);
    void deleteFile(const QString &fileName);
    bool isConnected() const;
    QFtp::State ftpState() const;

signals:
    void sigUpdateDataTransferProgress(qint64,qint64);
    void sigUploaded(bool);
    void sigDelete(bool);
    void sigRename(bool);

    void FileGetOver(FileInfo info);
    void ftpFileSendOver(FileInfo info);

private slots:
    void ftpCommandFinished(int cmdId, bool error);
    void ftpStateChanged(int state);
    void addToList(const QUrlInfo &urlInfo);
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

private:
    QString Ftp_Ip;
    unsigned short Ftp_Port;
    QString Ftp_Username;
    QString Ftp_Password;
    QFtp *mFtp = nullptr;
    QTimer *mTimer = nullptr;

    std::map<int, sFtpData> mTaskMap;

    QFtp::State mFtpState;
};

#endif // FTPMANAGER_H
