#ifndef FTPSENDER_H
#define FTPSENDER_H
#include <string>

class FtpSender
{
public:
    FtpSender();

signals:
    void ftpFileSendOver(std::string name);
    void ftpFileGetOver(std::string name);
};

#endif // FTPSENDER_H
