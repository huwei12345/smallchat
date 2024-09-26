#ifndef FTPSENDER_H
#define FTPSENDER_H
#include <string>

class FtpSender
{
public:
    FtpSender();

public slots:
    void ftpFileSendOver(std::string name);

};

#endif // FTPSENDER_H
