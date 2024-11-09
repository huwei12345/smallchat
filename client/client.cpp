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
    int count = 10000;
    while (count--) {
        string data;
        MyProtocolStream stream(data);
        Request req(1, FunctionCode::TEST, 3, 4, 5, "", 0, 0);

        int r = Trans::send(clientSocket, req);
        if (r > 0) {
            printf("send success\n");
        }
        Response rsp;
        int x = Trans::receive(clientSocket, rsp);
        rsp.print();
    }
}

bool Login(int clientSocket, UserInfo& info, string username, string password) {
    //Login
    string data;
    MyProtocolStream stream(data);
    stream << username << password;
    Request req(1, FunctionCode::Login, 3, 4, 5, data, 0, 0);
    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
	}
	Response rsp;
	Trans::receive(clientSocket, rsp);
	rsp.print();
    string mdata = rsp.mData;
    
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

bool Register(int clientSocket, UserInfo& info, string username = "lixin", string password = "123456", string email = "email") {
    string data;
    MyProtocolStream stream(data);
	stream << username << password << email << "full_name" << "avatar_url"
      << "bio" << true << 20 << "shanxi";
    Request req(1, FunctionCode::Register, 3, 4, 5, data);

    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
	}
	Response rsp;
	int x = Trans::receive(clientSocket, rsp);
	rsp.print();
    string mdata = rsp.mData;

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
    string data;
    MyProtocolStream stream(data);
	stream << info.user_id;
    Request req(1, FunctionCode::SearchAllFriend, 3, 4, 5, data, info.user_id);
	cout << endl;

    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
	}
	Response rsp;
	int x = Trans::receive(clientSocket, rsp);
	rsp.print();
    string mdata = rsp.mData;

    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    for (int i = 0; i < size; i++) {
        UserInfo info;
        stream2 >> info.user_id >> info.username >> info.email;
        info.print();
    }
}


void FindFriend(int clientSocket, UserInfo& info, FriendList& friendList, string name) {
    string data;
    MyProtocolStream stream(data);
	stream << name;
    Request req(1, FunctionCode::FindFriend, 3, 4, 5, data, info.user_id);
	cout << endl;

    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
	}
	Response rsp;
	int x = Trans::receive(clientSocket, rsp);
	rsp.print();
    string mdata = rsp.mData;

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
    string data;
    MyProtocolStream stream(data);
	stream << user_id;
    Request req(1, FunctionCode::AddFriend, 3, 4, 5, data, info.user_id);
	cout << endl;
    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
	}
	Response rsp;
	int x = Trans::receive(clientSocket, rsp);
	rsp.print();
    if (rsp.mCode == 1)
        return true;
    return false;
}

bool SendMessage2(int clientSocket, UserInfo& info, MessageInfo& message) {
    string data;
    MyProtocolStream stream(data);
	stream << message.recv_id << message.flag<< message.message_text;
    Request req(1, FunctionCode::SendMessage, 3, 4, 5, data, info.user_id);

    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
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

bool SendFile(int clientSocket, UserInfo& info, FileInfo& fileInfo) {
    string data;
    MyProtocolStream stream(data);
    stream << fileInfo.ftpTaskId << fileInfo.id << fileInfo.serviceType << fileInfo.send_id << fileInfo.recv_id <<  fileInfo.serverPath << fileInfo.serverFileName << fileInfo.fileType << fileInfo.filesize << fileInfo.fileMode << fileInfo.md5sum;
    Request req(1, FunctionCode::StartUpLoadFile, 3, 4, 5, data, info.user_id);
    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
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

bool SendFileSuccess(int clientSocket, UserInfo& infox, FileInfo& info) {
    string data;
    MyProtocolStream stream(data);
    stream << info.ftpTaskId << info.id << info.serviceType << info.send_id << info.recv_id <<  info.serverPath << info.serverFileName << info.fileType << info.filesize << info.fileMode << info.md5sum;
    Request req(1, FunctionCode::UpLoadFileSuccess, 3, 4, 5, data, infox.user_id);
    int r = Trans::send(clientSocket, req);
	if (r > 0) {
		printf("send success\n");
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

std::string Client::sendAndRecv(const char *str, int code)
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
                        printf("do you want to accept %s's requestAdd \n", friendlist[i].username.c_str());
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
            case FunctionCode::SendMessage: {
                printf("please input user_id and Message\n");
                int user_id = 0;
                char Message[2000];
                scanf("%d %s", &user_id, Message);
                message.message_text = Message;
                message.recv_id = user_id;
                message.send_id = info.user_id;
                printf("message %d %d\n", message.send_id, message.recv_id);
                SendMessage2(clientSocket, info, message);
                break;
            }
            case FunctionCode::StartUpLoadFile: {
                FileInfo infox;
                infox.serverPath = "1/";
                infox.serverFileName = "a.txt";
                infox.filesize = 10;
                infox.recv_id = 13;
                infox.send_id = 1;
                infox.timestamp = "2024-09-09 07:08:09";
                SendFile(clientSocket, info, infox);

                sleep(1);

                SendFileSuccess(clientSocket, info, infox);
                break;
            }
            case 25: {
                printf("please input user_id and Message\n");
                
                MessageInfo* minfo = new TextMessageInfo;
                minfo->recv_id = 13;
                minfo->send_id = 13;
                minfo->message_text = "content";
                minfo->flag = MessageInfo::Group;
                printf("message %d %d\n", message.send_id, message.recv_id);
                SendMessage2(clientSocket, info, *minfo);
            }
        }
    }
	return "";
}

Client::~Client()
{
    // 关闭套接字
    close(clientSocket);
}

