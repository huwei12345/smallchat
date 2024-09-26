#include "ftpsender.h"

FtpSender::FtpSender()
{

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
    GetFileSuccess;
}
