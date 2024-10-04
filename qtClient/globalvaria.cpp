#include "globalvaria.h"
#include "Protocol.h"
#include <QSettings>
#include <QDebug>
#ifndef SERVER
int FileInfo::GenerateId = 0;
QMutex FileInfo::genMutex;
#endif

GlobalVaria::GlobalVaria()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    // 读取全局字符串
    QString stringValue = settings.value("global/string").toString();
    // 读取全局整数
    int intValue = settings.value("global/integer").toInt();
    // 读取全局布尔值
    bool boolValue = settings.value("global/boolean").toBool();
    // 读取section1下的字符串
    QString sectionStringValue = settings.value("section1/string").toString();
    // 打印读取的值
    mServerIp = settings.value("global/SERVERIP").toString();
    mFtpIp = settings.value("global/FTPIP").toString();
    mFtpPort = settings.value("global/FTPPORT").toUInt();
    mFtpUser = settings.value("global/FTPUSER").toString();
    mFtpPassword = settings.value("global/FTPPASSWORD").toString();
    mFtpType = settings.value("global/FTPTYPE").toString();
    qDebug() << "String Value: " << stringValue;
    qDebug() << "Integer Value: " << intValue;
    qDebug() << "Boolean Value: " << boolValue;
    qDebug() << "Section String Value: " << sectionStringValue;
    qDebug() << "mServerIp: "
              << mServerIp
              << "\n mFtpIp: "
              << mFtpIp
              << "\n mFtpPort: "
              << mFtpPort
              << "\n mFtpUser: "
              << mFtpUser
              << "\n mFtpPassword: "
              << mFtpPassword
              << "\n mFtpType: "
              << mFtpType;
}

unsigned short GlobalVaria::ftpPort() const
{
    return mFtpPort;
}

void GlobalVaria::setFtpPort(unsigned short ftpPort)
{
    mFtpPort = ftpPort;
}

GlobalVaria* GlobalVaria::globalVaria = NULL;

GlobalVaria *GlobalVaria::GetInstance()
{
    if (globalVaria == NULL) {
        globalVaria = new GlobalVaria;
    }
    return globalVaria;
}

QString GlobalVaria::serverIp() const
{
    return mServerIp;
}

void GlobalVaria::setServerIp(const QString &serverIp)
{
    mServerIp = serverIp;
}

QString GlobalVaria::ftpIp() const
{
    return mFtpIp;
}

void GlobalVaria::setFtpIp(const QString &ftpIp)
{
    mFtpIp = ftpIp;
}

QString GlobalVaria::user() const
{
    return mFtpUser;
}

void GlobalVaria::setUser(const QString &user)
{
    mFtpUser = user;
}

QString GlobalVaria::password() const
{
    return mFtpPassword;
}

void GlobalVaria::setPassword(const QString &password)
{
    mFtpPassword = password;
}

QString GlobalVaria::ftpType() const
{
    return mFtpType;
}

void GlobalVaria::setFtpType(const QString &ftpType)
{
    mFtpType = ftpType;
}
