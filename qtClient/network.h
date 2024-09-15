#ifndef NETWORK_H
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

    void confirmMessage(int uid, int start, int end);
signals:
    void loginSuccessful(UserInfo info);
    void loginFailure();
    void FindFriendSuccess(Response response);
    void findAllFriendSuccess(Response response);
    void AddFriendSuccess(int mCode);
    void getAllMessageSuccess(Response response);
    void getAllFriendRequestSuccess(Response response);
    void UpDateUserStateSuccess(Response response);
private:
    ClientNetWork();
    QTcpSocket mSocket;
    static ClientNetWork* clientNetwork;

};

#endif // NETWORK_H
