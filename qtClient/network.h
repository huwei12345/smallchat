﻿#ifndef NETWORK_H
#define NETWORK_H
#include <QTcpSocket>
#include <QAbstractSocket>
#include "Protocol.h"
class ClientNetWork : public QObject
{
    Q_OBJECT
public:
    static ClientNetWork* GetInstance();
    int Server();
    int Client();
    int SendPacket(QByteArray &responsePacket);
    void socketError(QAbstractSocket::SocketError socketError);
    bool isClose();
    void close();
    bool process(QByteArray& array);

    bool confirmMessage(int sendId, int recvId, int start, int end);
    bool confirmgroupMessage(int groupId, int mUserId, int lastConfirmId, QVector<int> confirmVec);
signals:
    void loginSuccessful(UserInfo info);
    void loginFailure();
    void FindFriendSuccess(Response response);
    void ChangeUserPic(FileInfo info);
    void ChangeGroupUserPic(FileInfo info);
    void ChangeUserPicBySend(FileInfo info);
    void FindGroupSuccess(Response response);
    void findAllFriendSuccess(Response response);
    void findAllGroupSuccess(Response response);
    void AddFriendSuccess(int mCode);
    void ProcessFriendRequestResult(Response response);
    void getAllMessageSuccess(Response response);
    void getAllOfflineFileSuccess(Response response);
    void getAllFriendRequestSuccess(Response response);
    void getAllGroupRequestSuccess(Response response);
    void UpDateUserStateSuccess(Response response);
    void ReciveMessageSuccess(Response response);
    void PersonMessageArriveClient(Response response);
    void GroupMessageArriveClient(Response response);
    void createGroupSuccess(Response response);
    void applyJoinGroupSuccess(Response response);
    void processGroupApplySuccess(Response response);
    void storeFileResponse(Response response);
    void StartUpLoadFileSuccess(Response response);
    void UpLoadFileSuccess(Response response);
    void GetFileFirstSuccess(Response response);
    void GetFileSuccess(Response response);
    void NofifyFileComing(Response response);

    void findSpaceFileTreeSuccess(Response response);
    void findAllGroupMemberSuccess(Response response);
    void getAllGroupMessageSuccess(Response response);
    void ProcessGroupMessageSuccess(Response response);
private:
    ClientNetWork();
    QTcpSocket mSocket;
    static ClientNetWork* clientNetwork;
    QString mServerIp;
public slots:
    void ftpFileSendOver(FileInfo filename);
    void ftpFileGetOver(FileInfo filename);
};

#endif // NETWORK_H
