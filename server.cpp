#include "server.h"
#include "Protocol.h"
#include "RequestProcessor.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <functional>
#include "Trans.h"
#include "./cache/friendCache.h"
#include "EventLoop.h"
using namespace std;
#define MAX_REQUEST_SIZE 4096
#define IO_THREAD_NUM 2

RequestProcessor* requestProcessor[100];

// int readline(int sockfd, char* buffer, int MaxSize) {
//     int n;
//     char c;
//     int i = 0;
//     while (i < MaxSize - 1) {
//         n = recv(sockfd, &c, 1, 0);
//         if (n > 0) {
//             buffer[i++] = c;
//             if (c == '\n')
//                 break;
//         } else if (n == 0) {
//             break;
//         } else {
//             if (errno == EAGAIN || EWOULDBLOCK) {
//                 continue;
//             }
//             perror("recv");
//             break;
//         }
//     }
//     buffer[i] = '\0';
//     return i;
// }

// bool readRequest(int fd, char* buf, std::string& requestData) {
//     int ret = recv(fd, buf, 4, MSG_PEEK);
//     if (ret <= 0) {
//         if (ret == 0) {
//             printf("connect %d close\n", fd);
//             close(fd);
//         }
//         // Handle recv error
//         return false;
//     } else if (ret < 4) {
//         // Handle incomplete length field
//         return false;
//     }
//     uint32_t len = 0;
//     memcpy(&len, buf, 4);
//     len = htonl(len);
    
//     if (len < 4 || len > MAX_REQUEST_SIZE) {
//         // Handle invalid length
//         return false;
//     }
    
//     ret = recv(fd, buf, len, MSG_WAITALL);
//     if (ret <= 0) {
//         if (ret == 0) {
//             printf("connect %d close\n", fd);
//             close(fd);
//         }
//         // Handle recv error
//         return false;
//     } else if (ret != len) {
//         // Handle incomplete request data
//         return false;
//     }
//     requestData.assign(buf, buf + len);
//     return true;
// }



Request* parseRequest(std::string& RequestData) {
    Request* request = new Request;
    request->deserial(RequestData);
    request->print();
    return request;
}

// Response* processRequest(Request* request) {
//     Response* response = new Response;
//     requestProcessor[request->mFunctionCode]->Exec(NULL, *request, *response);
//     response->print();
//     return response;
// }

// bool sendDataAll(int fd, const char* data, int len) {
//     int pos = 0;
//     while (pos != len) {
//         int ret = ::send(fd, data + pos, len - pos, 0);
//         if (ret != -1) { 
//             pos += ret;
//         }
//         else if (ret == 0) {
//             return ret;
//         }
//         else {
//             if (errno = EAGAIN || errno == EWOULDBLOCK) {
//                 continue;
//             }
//             return ret;
//         }
//     }
//     return len;
// }

// bool sendDataAll(int fd, std::string data) {
//     return sendDataAll(fd, data.c_str(), data.size());
// }

// bool sendResponse(int fd, Response* response) {
//     std::string responseData = serialResponse(response);
//     if (responseData == "") {
//         return false;
//     }
//     return sendDataAll(fd, responseData);
// }

std::string serialResponse(Response* response) {
    return response->serial();
}

/*
错误处理改进：

当 readRequest()、parseRequest() 或 processRequest() 返回 NULL 或 false 时，函数直接返回 false。这种方式可以通过错误码或日志记录来提供更详细的错误信息，帮助调试和排查问题。

内存管理：
确保在函数的任何退出路径上，释放 request 和 response 对象的内存，避免内存泄漏。可以使用智能指针或者手动释放资源。
参数验证：

对于函数参数如 buf 和 len，确保在使用之前进行有效性验证，以避免潜在的缓冲区溢出或无效访问。
返回类型：

考虑是否需要将 process() 函数的返回类型改为 void，并通过传递指针或引用来返回处理结果，以提高代码的清晰度和可读性。
错误处理策略：

考虑实现更复杂的错误处理策略，例如回退、重试或报告错误给调用者，以提高系统的鲁棒性和可靠性。
性能优化：

如果可能，评估每个步骤的性能并进行优化，特别是在处理大量请求时，如使用更高效的数据结构或算法。
*/

// bool process(int fd, char* buf) {
//     std::string requestData;
//     Request* request = nullptr;
//     Response* response = nullptr;

//     if (!readRequest(fd, buf, requestData)) {
//         return false;
//     }

//     request = parseRequest(requestData);
//     if (!request) {
//         // Clean up requestData if necessary
//         return false;
//     }

//     response = processRequest(request);
//     delete request; // Clean up request after use
//     if (!response) {
//         // Clean up response if necessary
//         return false;
//     }
//     bool success = true;
//     if (response->returnFlag)
//         success = sendResponse(fd, response);
//     delete response; // Clean up response after use
//     return success;
// }


int sendline(int sockfd, char* buffer, int Size) {
    int pos = 0;
    while (pos != Size) {
        int ret = ::send(sockfd, buffer + pos, Size - pos, 0);
        if (ret != -1) { 
            pos += ret;
        }
        else if (ret == 0) {
            return ret;
        }
        else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            return ret;
        }
    }
    return Size;
}

Server::Server(const char *ip, unsigned int port)
{
    if (ip == NULL) {
        mServerIP = "";
    }
    else {
        mServerIP = ip;
    }
    mServerPort = port;

    requestProcessor[FunctionCode::TEST] = new RequestProcessor;
    requestProcessor[FunctionCode::Register] = new RegisterProcessor;
    requestProcessor[FunctionCode::Login] = new LoginProcessor;
    requestProcessor[FunctionCode::SearchAllFriend] = new SearchAllFriendProcessor;
    requestProcessor[FunctionCode::FindFriend] = new FindFriendProcessor;
    requestProcessor[FunctionCode::AddFriend] = new AddFriendProcessor;
    requestProcessor[FunctionCode::SendMessage] = new SendMessageProcessor;
    
    requestProcessor[FunctionCode::GetAllFriendRequest] = new GetAllFriendReqProcessor;
    requestProcessor[FunctionCode::GetAllMessage] = new GetAllMessageProcessor;
    requestProcessor[FunctionCode::ProcessFriendRequest] = new ProcessFriendRequestProcessor;
    requestProcessor[FunctionCode::GetAllGroupRequest] = new GetAllGroupReqProcessor;
    requestProcessor[FunctionCode::ProcessGroupJoinReq] = new ProcessGroupJoinReqProcessor;

    requestProcessor[FunctionCode::UpdateUserState] = new UpdateUserStateProcessor;
    requestProcessor[FunctionCode::ProcessMessageRead] = new ProcessMessageReadProcessor;

    requestProcessor[FunctionCode::CreateGroup] = new CreateGroupProcessor;
    requestProcessor[FunctionCode::JoinGroup] = new JoinGroupProcessor;
    requestProcessor[FunctionCode::ResponseJoinGroup] = new ResponseJoinGroupProcessor;
    requestProcessor[FunctionCode::StoreFile] = new StoreFileProcessor;

    
    requestProcessor[FunctionCode::StartUpLoadFile] = new ProcessStartUpLoadFileProcessor;
    requestProcessor[FunctionCode::UpLoadFileSuccess] = new ProcessUpLoadFileSuccessProcessor;

    requestProcessor[FunctionCode::GetFile] = new ProcessGetFileProcessor;
    requestProcessor[FunctionCode::GetFileSuccess] = new ProcessGetFileSuccessProcessor;
    
    requestProcessor[FunctionCode::NofifyFileComing] = new ProcessNofifyFileComingProcessor;
    requestProcessor[FunctionCode::TransFileOver] = new ProcessTransFileOverProcessor;
    requestProcessor[FunctionCode::AgreeRecvFile] = new RequestProcessor;

    requestProcessor[FunctionCode::GetAllOfflineFile] = new ProcessGetAllOfflineFileProcessor;
    
    requestProcessor[FunctionCode::GetOfflineFile] = new ProcessGetOfflineFileProcessor;

    requestProcessor[FunctionCode::SearchAllGroup] = new SearchAllGroupProcessor;
    requestProcessor[FunctionCode::FindGroup] = new FindGroupProcessor;
    requestProcessor[FunctionCode::FindSpaceFileTree] = new ProcessFindSpaceFileTreeProcessor;
    requestProcessor[FunctionCode::FindAllGroupMember] = new ProcessFindAllGroupMemberProcessor;
    
    requestProcessor[FunctionCode::DELETESTOREFILE] = new ProcessEraseFileProcessor;
    requestProcessor[FunctionCode::EDITSTOREFILE] = new RequestProcessor;
    requestProcessor[FunctionCode::GETSTOREFILE] = new RequestProcessor;
    requestProcessor[FunctionCode::RENAMESTOREFILE] = new ProcessMoveFileProcessor;
    requestProcessor[FunctionCode::GetAllGroupMessage] = new GetAllGroupMessageProcessor;
    requestProcessor[FunctionCode::ProcessGroupMessageRead] = new ProcessGroupMessageReadProcessor;
    requestProcessor[FunctionCode::Heartbeat] = new HeartbeatProcessor;
    requestProcessor[FunctionCode::LOGOUT] = new LogoutProcessor;
    for (int i = 50; i < 100; i++) {
        requestProcessor[i] = new RequestProcessor;
    }
}


void runx(Server* server) {

}

int Server::selectAlgorithm() {
    static int loop = 0;
    return loop++ % IO_THREAD_NUM;
}

static const int HEARTBEAT_CHECK_INTERVAL = 60; // 检查间隔(秒)
static const int HEARTBEAT_TIMEOUT = 90;         // 超时时间(秒)

static void heartbeatCheckThread(Server* server) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_CHECK_INTERVAL));
        std::time_t now = std::time(nullptr);
        std::vector<int> timeoutFds;

        {
            std::lock_guard<std::mutex> lock(server->mConnectionMapMutex);
            for (auto& pair : server->mConnectionMap) {
                if (pair.second && (now - pair.second->lastActiveTime) > HEARTBEAT_TIMEOUT) {
                    timeoutFds.push_back(pair.first);
                }
            }
        }

        for (int fd : timeoutFds) {
            printf("heartbeat timeout, closing connection fd=%d\n", fd);
            Connection* conn = nullptr;
            {
                std::lock_guard<std::mutex> lock(server->mConnectionMapMutex);
                auto it = server->mConnectionMap.find(fd);
                if (it != server->mConnectionMap.end()) {
                    conn = it->second;
                }
            }
            if (conn) {
                conn->closeConnection();
            }
        }
    }
}

int Server::run()
{
    mServerSocket = createListener();
    if (mServerSocket <= 0) {
        return -1;
    }
    for (int i = 0; i < IO_THREAD_NUM; i++) {
        EventLoop* loop = new EventLoop(this, mServerSocket, i);
        loop->addWakeupSocket();
        mEvLoopList.push_back(loop);
    }
    for (int i = 0; i < IO_THREAD_NUM; i++) {
        std::thread *iothread =  new std::thread(std::bind(&EventLoop::Run, mEvLoopList[i]));
        mEvLoopList[i]->mThread = iothread;
        if (iothread == nullptr) {
            printf("ioThread %d create error\n", i);
            return -3;
        }
    }
    // 启动心跳检测线程
    std::thread heartbeatThread(heartbeatCheckThread, this);
    heartbeatThread.detach();
    printf("heartbeat check thread started (interval=%ds, timeout=%ds)\n",
           HEARTBEAT_CHECK_INTERVAL, HEARTBEAT_TIMEOUT);

    mMainEventLoop = new EventLoop(this, mServerSocket);
    mMainEventLoop->RunMain();
    return 0;
}

int Server::createListener()
{
        // 创建socket并绑定端口
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0) {
        perror("socket");
        ::exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (mServerIP == "")
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        server_addr.sin_addr.s_addr = inet_addr(mServerIP.c_str());
    server_addr.sin_port = htons(mServerPort);
    int opt = 1;
    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error: setsockopt failed SO_REUSEADDR ");
        exit(EXIT_FAILURE);
    }
    // 将socket设置为非阻塞模式
    if (fcntl(mServerSocket, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (bind(mServerSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        ::exit(EXIT_FAILURE);
    }
    if (listen(mServerSocket, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return mServerSocket;
}


Server *Server::GetInstance()
{
    static Server instance;
    return &instance;
}

bool Connection::readRequest(std::string &requestData)
{
    int ret = recv(clientSocket, buffer, 4, MSG_PEEK);
    if (ret <= 0) {
        if (ret == 0) {
            printf("connect %d close\n", clientSocket);
            closeConnection();
        }
        // Handle recv error
        return false;
    } else if (ret < 4) {
        // Handle incomplete length field
        return false;
    }
    uint32_t len = 0;
    memcpy(&len, buffer, 4);
    len = htonl(len);
    
    if (len < 4 || len > MAX_REQUEST_SIZE) {
        // Handle invalid length
        return false;
    }
    
    ret = recv(clientSocket, buffer, len, MSG_WAITALL);
    if (ret <= 0) {
        if (ret == 0) {
            printf("connect %d close\n", clientSocket);
            closeConnection();
        }
        // Handle recv error
        return false;
    } else if (ret != len) {
        // Handle incomplete request data
        return false;
    }
    requestData.assign(buffer, buffer + len);
    return true;
}

bool Connection::processRead()
{
    std::string requestData;
    Request* request = nullptr;
    Response* response = nullptr;
    if (!readRequest(requestData)) {
        return false;
    }
    updateActiveTime();
    request = parseRequest(requestData);
    if (!request) {
        // Clean up requestData if necessary
        return false;
    }

    int functionCode = request->mFunctionCode;
    response = new Response;
    requestProcessor[functionCode]->Exec(this, *request, *response);
    delete request;

    if (functionCode == FunctionCode::ProcessMessageRead) {
        delete response;
        return true;
    }
    bool success = true;
    if (response->returnFlag) {
        response->print();
        success = sendResponse(clientSocket, response);
    }
    delete response; // Clean up response after use
    return success;
}

bool Connection::sendResponse(int clientSocket, Response* response)
{
    std::string responseData = serialResponse(response);
    if (responseData == "") {
        return false;
    }
    return mEvLoop->sendDataAll(clientSocket, responseData);
}

//flag = 0,默认关闭  flag = 1,强行关闭
bool Connection::closeConnection(int flag)
{
    mEvLoop->eraseSocket(clientSocket);

    Connection* conn = nullptr;
    Session* session = nullptr;
    int userId = -1;

    // 从 map 中移除并获取 session 信息
    {
        std::lock_guard<std::mutex> lock(Server::GetInstance()->mConnectionMapMutex);
        auto it = Server::GetInstance()->mConnectionMap.find(clientSocket);
        if (it == Server::GetInstance()->mConnectionMap.end()) {
            return true;
        }
        conn = it->second;
        Server::GetInstance()->mConnectionMap.erase(it);
    }

    if (conn == nullptr) {
        return true;
    }

    session = conn->session;
    if (session != NULL) {
        userId = session->mUserId;
        std::lock_guard<std::mutex> lock(Server::GetInstance()->mSessionMapMutex);
        Server::GetInstance()->mUserSessionMap.erase(userId);
    }

    // 通知好友下线（在关闭 socket 之前）
    if (userId != -1) {
        ProcessNotifyStateProcessor processor;
        vector<int> friendList = FriendCache::GetInstance()->getFriendList(userId);
        processor.Notify(conn, friendList, userId, OFFLINE);
    }

    // 关闭 socket 并释放资源
    close(clientSocket);
    clientSocket = -1;
    if (session != NULL) {
        delete session;
    }
    delete conn;
    return true;
}

/*

2024年9月15日

1.缺少在线发送消息和朋友请求逻辑(已完成，待优化)

2.缺少消息处理后的数据库read=1操作，目前方便调试不用改（已完成，通过注释Qt端发送逻辑取消该功能）
如果使用，最好在Qt端本地持久化历史聊天数据，或者添加一个逻辑，在本地没有聊天数据时，请求30天内已确认聊天数据到本地

3.界面端对于New Message显示取消（已完成）

4.Connection的断开连接 delete操作 Session（已完成，待优化）,
如何防止野指针

5.通过userId搜索朋友(完成，未测试)

6.修改当前朋友状态，以及在页面的显示(未完成)
朋友上线后的提示，下线提示

7.Group
朋友列表分组和 群都是Group，还需要一个表

8.头像存储

9.表情显示与存储

10.图片、文件

11.抖动消息

12.远程存储文件
*/
