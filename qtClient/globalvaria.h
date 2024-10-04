#ifndef GLOBALVARIA_H
#define GLOBALVARIA_H
#include <QString>
#include "clientpersoninfo.h"
class GlobalVaria
{
public:
    static GlobalVaria *GetInstance();

    QString serverIp() const;
    void setServerIp(const QString &serverIp);

    QString ftpIp() const;
    void setFtpIp(const QString &ftpIp);

    QString user() const;
    void setUser(const QString &user);

    QString password() const;
    void setPassword(const QString &password);

    QString ftpType() const;
    void setFtpType(const QString &ftpType);

    unsigned short ftpPort() const;
    void setFtpPort(unsigned short ftpPort);

private:
    GlobalVaria();
    QString mServerIp;
    QString mFtpIp;
    unsigned short mFtpPort;
    QString mFtpUser;
    QString mFtpPassword;
    QString mFtpType;
    static GlobalVaria* globalVaria;
};

#endif // GLOBALVARIA_H
