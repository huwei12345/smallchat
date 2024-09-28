#include "processor.h"
#include <QMessageBox>
#include <string>
#include "network.h"
#include "MyProtocolStream.h"
#include "Protocol.h"
#include "ftpsender.h"
int user_id;

Processor::Processor()
{

}


bool Processor::Login(string& username, string& password) {
    //Login
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << username << password;
    Request req(1, FunctionCode::Login, 3, 4, 5, data, 0, 0);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();

        return true;
    }
    return false;
}

bool Processor::Register(string& username, string& password, string& email) {
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << username << password << email << "full_name" << "avatar_url"
      << "bio" << true << 20 << "shanxi";
    Request req(1, FunctionCode::Register, 3, 4, 5, data);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::SendMessage(int reciveId, string& content) {
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    MessageInfo message;
    message.receiver_id = reciveId;
    message.message_text = content;
    std::string data;
    MyProtocolStream stream(data);
    stream << message.receiver_id << message.message_text;
    Request req(1, FunctionCode::SendMessage, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::FindFriend(int friendId) {
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << QString::number(friendId).toStdString();
    Request req(1, FunctionCode::FindFriend, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;




//    Response rsp;
//    int x = Trans::receive(clientSocket, rsp);
//    rsp.print();
//    std::string mdata = rsp.mData;

//    MyProtocolStream stream2(mdata);
//    int size = 0;
//    stream2 >> size;
//    for (int i = 0; i < size; i++) {
//        UserInfo info;
//        stream2 >> info.user_id >> info.username >> info.email;
//        info.print();
//    }
}

bool Processor::FindFriendByName(string& friendName) {
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << friendName;
    Request req(1, FunctionCode::FindFriend, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::AddFriend(int friendId) {
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << friendId;
    Request req(1, FunctionCode::AddFriend, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}


bool Processor::findAllFriend(int userId)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << userId;
    Request req(1, FunctionCode::SearchAllFriend, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::getAllMessage(int userId)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << userId;
    Request req(1, FunctionCode::GetAllMessage, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::getAllFriendRequest(int userId)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << userId;
    Request req(1, FunctionCode::GetAllFriendRequest, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::ChangeUserState(int state)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << state;
    Request req(1, FunctionCode::UpdateUserState, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::processFriendRequest(FriendRequest friendRequest)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << friendRequest.sender_id << friendRequest.reciver_id << friendRequest.mAccept;
    Request req(1, FunctionCode::ProcessFriendRequest, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::processMessageRead(std::vector<int> messageList)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    //多个一起确认
    stream << (int)messageList.size();
    for (int i = 0; i < (int)messageList.size(); i++) {
        stream << messageList[i];
    }
    Request req(1, FunctionCode::ProcessMessageRead, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::CreateGroup(GroupInfo &info)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << info.group_name << info.gtype << info.admin_id << info.description << info.tips;
    Request req(1, FunctionCode::CreateGroup, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

//还要先有FindGroup
bool Processor::JoinGroup(int groupId)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << groupId;
    Request req(1, FunctionCode::JoinGroup, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::FindGroup(int groupId)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << groupId;
    Request req(1, FunctionCode::FindGroup, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}


bool Processor::SendFile(string path, string filename)
{
    int filesize = 100;
    int fileMode = 777;
    int fileType = 1;
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << path << filename << fileType << filesize << fileMode;
    Request req(1, FunctionCode::StartUpLoadFile, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::SendMessageSuccess(string filename)
{
    int md5sum = 0xfffe1111;
    std::string data;
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    MyProtocolStream stream(data);
    stream << filename << md5sum;
    Request req(1, FunctionCode::UpLoadFileSuccess, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

//告诉服务器接收到了文件
bool Processor::GetMessageSuccess(string filename)
{
    int md5sum = 0xfffe1111;
    std::string data;
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    MyProtocolStream stream(data);
    stream << filename << md5sum;
    Request req(1, FunctionCode::GetFileSuccess, 3, 4, 5, data, user_id);

    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::GetFile(string path, string filename)
{
    int filesizeLimit = 10000;
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    stream << path << filename << filesizeLimit;
    Request req(1, FunctionCode::GetFile, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    return false;
}

bool Processor::AgreeRecvFile(bool agree, FileInfo info)
{
    ClientNetWork* clientSocket = ClientNetWork::GetInstance();
    std::string data;
    MyProtocolStream stream(data);
    //或许需要一个Id,便于服务器标识
    stream << agree << info.path << info.filename;
    Request req(1, FunctionCode::NofifyFileComing, 3, 4, 5, data, user_id);
    string str = req.serial();
    QByteArray array(str.c_str(),str.size());
    int r = clientSocket->SendPacket(array);
    if (r > 0) {
        req.print();
        return true;
    }
    if (agree) {
        FtpSender::GetInstance()->GetFile(info);
    }
    else {

    }
    return false;
}


/*
#include "client.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <iostream>
#include "../Trans.h"
#include "../MyProtocolStream.h"

Client::Client(const char *ip, unsigned short port)
{
    struct sockaddr_in serverAddr;
    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cout << "Failed to create socket" << std::endl;
        return;
    }
    int opt = 1;
    if (setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error: setsockopt failed SO_REUSEADDR ");
        exit(EXIT_FAILURE);
    }
    // 配置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // 设置服务器端口
    serverAddr.sin_addr.s_addr = inet_addr(ip); // 设置服务器IP（本地回环地址）

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cout << "Failed to connect to server" << std::endl;
        close(clientSocket);
        return;
    }
}

void Client::run()
{

}

void Client::send(const char *str)
{
    const char* message = "Hello from client!";
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // 发送数据给服务器
    if (::send(clientSocket, message, strlen(message), 0) == -1) {
        std::cout << "Failed to send data to server" << std::endl;
        close(clientSocket);
        return;
    }
    std::cout << "Data sent to server: " << message << std::endl;
}

void Test(int clientSocket) {
    //Login
    std::string data;
    MyProtocolStream stream(data);
    Request req(1, FunctionCode::TEST, 3, 4, 5, "", 0, 0);

    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
}

bool Login(int clientSocket, UserInfo& info, std::string username, std::string password) {
    //Login
    std::string data;
    MyProtocolStream stream(data);
    stream << username << password;
    Request req(1, FunctionCode::Login, 3, 4, 5, data, 0, 0);
    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    Trans::receive(clientSocket, rsp);
    rsp.print();
    std::string mdata = rsp.mData;

    MyProtocolStream stream2(mdata);
    stream2 >> info.user_id >> info.username;
    if (rsp.mCode == 1) {
        cout << "Login Success id " << info.user_id << " username " << info.username << std::endl;
        return true;
    }
    else if (rsp.mCode == 0) {
        printf("login Error\n");
        return false;
    }
    return false;
}

bool Register(int clientSocket, UserInfo& info, std::string username = "lixin", std::string password = "123456", std::string email = "email") {
    std::string data;
    MyProtocolStream stream(data);
    stream << username << password << email << "full_name" << "avatar_url"
      << "bio" << true << 20 << "shanxi";
    Request req(1, FunctionCode::Register, 3, 4, 5, data);

    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
    std::string mdata = rsp.mData;

    for (int i = 0; i < mdata.size(); i++) {
        cout << (int)mdata[i] << " ";
    }
    cout << std::endl;
    if (rsp.mhasData) {
        MyProtocolStream stream2(mdata);
        stream2 >> info.user_id >> info.username >> info.email
        >> info.full_name >> info.avatar_url >> info.bio
        >> info.sex >> info.age >> info.address;
        info.print();
    }
    if (rsp.mCode == 1)
        return true;
    if (rsp.mCode == 2) {
        printf("user has been Register\n");
        return false;
    }
    return false;
}

void SearchAllFriend(int clientSocket, UserInfo& info, FriendList& friendList) {
    std::string data;
    MyProtocolStream stream(data);
    stream << info.user_id;
    Request req(1, FunctionCode::SearchAllFriend, 3, 4, 5, data, info.user_id);
    cout << endl;

    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
    std::string mdata = rsp.mData;

    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    for (int i = 0; i < size; i++) {
        UserInfo info;
        stream2 >> info.user_id >> info.username >> info.email;
        info.print();
    }
}


void FindFriend(int clientSocket, UserInfo& info, FriendList& friendList, std::string name) {
    std::string data;
    MyProtocolStream stream(data);
    stream << name;
    Request req(1, FunctionCode::FindFriend, 3, 4, 5, data, info.user_id);
    cout << endl;

    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
    std::string mdata = rsp.mData;

    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    for (int i = 0; i < size; i++) {
        UserInfo info;
        stream2 >> info.user_id >> info.username >> info.email;
        info.print();
    }
}

bool AddFriend(int clientSocket, UserInfo& info, int user_id) {
    std::string data;
    MyProtocolStream stream(data);
    stream << user_id;
    Request req(1, FunctionCode::AddFriend, 3, 4, 5, data, info.user_id);
    cout << endl;
    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
    if (rsp.mCode == 1)
        return true;
    return false;
}

bool SendMessage(int clientSocket, UserInfo& info, MessageInfo& message) {
    std::string data;
    MyProtocolStream stream(data);
    stream << message.receiver_id << message.message_text;
    Request req(1, FunctionCode::SendMessage, 3, 4, 5, data, info.user_id);

    int r = Trans::send(clientSocket, req);
    if (r > 0) {
        req.print();
    }
    Response rsp;
    int x = Trans::receive(clientSocket, rsp);
    rsp.print();
    if (rsp.mCode == 1) {
        printf("send Message Success\n");
        return true;
    }
    printf("send Message Failure\n");
    return false;
}

void ReciveMessageAll() {

}

std::std::string Client::sendAndRecv(const char *str, int code)
{
    UserInfo info;
    FriendList friendlist;
    MessageInfo message;
    code = 0;
    while (1) {
        // printf("---------------mIM-Test-------------------\n");
        // printf("---------------0 Test---------------------\n");
        // printf("---------------1 Register-----------------\n");
        // printf("---------------2 Login--------------------\n");
        // printf("---------------3 SearchAllFriend----------\n");
        // printf("---------------4 FindFriend---------------\n");
        // printf("---------------5 AddFriend----------------\n");
        // printf("---------------6 SendMessage--------------\n");
        printf("please input your test code:\n");
        scanf("%d", &code);
        printf("now test %d %s\n", code, FunctionCode::FunctionCodeString[code].c_str());
        switch (code) {
            case FunctionCode::TEST:
                Test(clientSocket);
                break;
            case FunctionCode::Login: {
                printf("Login please Input your username password: \n");
                char username[200];
                char password[200];
                scanf("%s %s", username, password);
                Login(clientSocket, info, username, password);

                SearchAllFriend(clientSocket, info, friendlist);
                for (int i = 0; i < friendlist.size(); i++) {
                    if (friendlist[i].friendStatus == "accepted")
                        printf("friend %d: %d %s %s\n", i, friendlist[i].user_id, friendlist[i].username.c_str(), friendlist[i].email.c_str());
                }
                for (int i = 0; i < friendlist.size(); i++) {
                    if (friendlist[i].friendStatus == "pending") {
                        printf("do you want to accept %s's requestAdd \n", friendlist[i].username);
                        int x = 0;
                        scanf("%d", &x);
                        if (x == 1) {
                            AddFriend(clientSocket, info, friendlist[i].user_id);
                        }
                    }
                }
                ReciveMessageAll();

                break;
            }
            case FunctionCode::Register: {
                printf("Register please Input your username password email: \n");
                char username[200];
                char password[200];
                char email[200];
                scanf("%s %s %s", username, password, email);
                Register(clientSocket, info, username, password, email);
                break;
            }
            case FunctionCode::SearchAllFriend:
                SearchAllFriend(clientSocket, info, friendlist);
                break;
            case FunctionCode::FindFriend:
                char username[200];
                printf("FindFriend please Input username to Find: \n");
                scanf("%s", username);
                FindFriend(clientSocket, info, friendlist, username);
                break;
            case FunctionCode::AddFriend: {
                int id = 0;
                printf("AddFriend please Input user_id to Find: \n");
                scanf("%d", &id);
                AddFriend(clientSocket, info, id);
                break;
            }
            case FunctionCode::SendMessage:
                printf("please input user_id and Message\n");
                int user_id = 0;
                char Message[2000];
                scanf("%d %s", &user_id, Message);
                message.message_text = Message;
                message.receiver_id = user_id;
                message.sender_id = info.user_id;
                SendMessage(clientSocket, info, message);
                break;
        }
    }
    return "";
}

Client::~Client()
{
    // 关闭套接字
    close(clientSocket);
}




class Client {
public:
    Client(const char* ip = "127.0.0.1", unsigned short port = 8080);
    void run();
    void send(const char* str);
    std::std::string sendAndRecv(const char* str, int code = 0);
    ~Client();
private:
    int clientSocket;





#include "client.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[]) {
    Client client;
    int code = 0;
    if (argc < 2) {
        printf("usage ./client code\n");
        code = 0;
    }
    else {
        code = atoi(argv[1]);
    }
    std::std::string str = client.sendAndRecv("12345\n", code);
    std::cout << str << std::endl;
    sleep(3);
    return 0;
}

};




*/
