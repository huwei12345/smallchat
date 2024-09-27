#include "ftpsender.h"
#include "network.h"
FtpSender::FtpSender()
{
    connect(this, &FtpSender::ftpFileSendOver, ClientNetWork::GetInstance(), ftpFileSendOver);
    connect(this, &FtpSender::ftpFileGetOver, ClientNetWork::GetInstance(), ftpFileGetOver);
}

void FtpSender::ProcessList() {
    item = list.top();
    list.pop();
    if (1) {
        bool ret = ftpUtil->sendFile();
        if (ret) {
            emit ftpFileSendOver(fileName);
        }
    }
}

void FtpSender::GetProcessList() {
    item = list.top();
    list.pop();
    if (1) {
        bool ret = ftpUtil->getFile();
        if (ret) {
            emit ftpFileGetOver(fileName);
        }
    }
}
