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
#include "Trans.h"
using namespace std;
#define MAX_REQUEST_SIZE 4096

RequestProcessor* requestProcessor[100];

int readline(int sockfd, char* buffer, int MaxSize) {
    int n;
    char c;
    int i = 0;
    while (i < MaxSize - 1) {
        n = recv(sockfd, &c, 1, 0);
        if (n > 0) {
            buffer[i++] = c;
            if (c == '\n')
                break;
        } else if (n == 0) {
            break;
        } else {
            if (errno == EAGAIN || EWOULDBLOCK) {
                continue;
            }
            perror("recv");
            break;
        }
    }
    buffer[i] = '\0';
    return i;
}

bool readRequest(int fd, char* buf, std::string& requestData) {
    int ret = recv(fd, buf, 4, MSG_PEEK);
    if (ret <= 0) {
        if (ret == 0) {
            printf("connect %d close\n", fd);
            close(fd);
        }
        // Handle recv error
        return false;
    } else if (ret < 4) {
        // Handle incomplete length field
        return false;
    }
    uint32_t len = 0;
    memcpy(&len, buf, 4);
    len = htonl(len);
    
    if (len < 4 || len > MAX_REQUEST_SIZE) {
        // Handle invalid length
        return false;
    }
    
    ret = recv(fd, buf, len, MSG_WAITALL);
    if (ret <= 0) {
        if (ret == 0) {
            printf("connect %d close\n", fd);
            close(fd);
        }
        // Handle recv error
        return false;
    } else if (ret != len) {
        // Handle incomplete request data
        return false;
    }
    requestData.assign(buf, buf + len);
    return true;
}



Request* parseRequest(std::string& RequestData) {
    Request* request = new Request;
    request->deserial(RequestData);
    request->print();
    return request;
}

Response* processRequest(Request* request) {
    Response* response = new Response;
    requestProcessor[request->mFunctionCode]->Exec(NULL, *request, *response);
    response->print();
    return response;
}

bool sendDataAll(int fd, const char* data, int len) {
    int pos = 0;
    while (pos != len) {
        int ret = ::send(fd, data + pos, len - pos, 0);
        if (ret != -1) { 
            pos += ret;
        }
        else if (ret == 0) {
            return ret;
        }
        else {
            if (errno = EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            return ret;
        }
    }
    return len;
}

bool sendDataAll(int fd, std::string data) {
    return sendDataAll(fd, data.c_str(), data.size());
}

bool sendResponse(int fd, Response* response) {
    std::string responseData = serialResponse(response);
    if (responseData == "") {
        return false;
    }
    return sendDataAll(fd, responseData);
}

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

bool process(int fd, char* buf) {
    std::string requestData;
    Request* request = nullptr;
    Response* response = nullptr;

    if (!readRequest(fd, buf, requestData)) {
        return false;
    }

    request = parseRequest(requestData);
    if (!request) {
        // Clean up requestData if necessary
        return false;
    }

    response = processRequest(request);
    delete request; // Clean up request after use
    if (!response) {
        // Clean up response if necessary
        return false;
    }

    bool success = sendResponse(fd, response);
    delete response; // Clean up response after use
    return success;
}


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
            if (errno = EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            return ret;
        }
    }
    return Size;
}

Server::Server(const char *ip, unsigned int port)
{
    // 创建socket并绑定端口
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        ::exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (ip == NULL)
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error: setsockopt failed SO_REUSEADDR ");
        exit(EXIT_FAILURE);
    }
    // 将socket设置为非阻塞模式
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        ::exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // 创建epoll实例并将socket添加到epoll中
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    requestProcessor[FunctionCode::TEST] = new RequestProcessor;
    requestProcessor[FunctionCode::Register] = new RegisterProcessor;
    requestProcessor[FunctionCode::Login] = new LoginProcessor;
    requestProcessor[FunctionCode::SearchAllFriend] = new SearchAllFriendProcessor;
    requestProcessor[FunctionCode::FindFriend] = new FindFriendProcessor;
    requestProcessor[FunctionCode::AddFriend] = new AddFriendProcessor;
    requestProcessor[FunctionCode::SendMessage] = new SendMessageProcessor;
    
    requestProcessor[FunctionCode::GetAllFriendRequest] = new GetAllFriendReqProcessor;
    requestProcessor[FunctionCode::GetAllMessage] = new GetAllMessageProcessor;

    requestProcessor[FunctionCode::UpdateUserState] = new UpdateUserStateProcessor;
    requestProcessor[FunctionCode::ProcessFriendRequest] = new ProcessFriendRequestProcessor;
    requestProcessor[FunctionCode::ProcessMessageRead] = new ProcessMessageReadProcessor;

    for (int i = 30; i < 100; i++) {
        requestProcessor[i] = new RequestProcessor;
    }
}

void Server::run()
{
    // 进入循环，等待事件发生
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t client_len = 0;
        // 当有事件发生时，处理事件并将socket重新添加到epoll中
        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                while (1) {
                    int client_fd = ::accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        } else {
                            printf("%d %d %d\n", server_fd, client_fd, errno);
                            perror("accept");
                            exit(EXIT_FAILURE);
                        }
                    }
                    std::cout << "get connect in" << client_fd << std::endl;
                    // 将client socket设置为非阻塞模式
                    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
                        perror("fcntl");
                        exit(EXIT_FAILURE);
                    }
                    struct epoll_event ev;
                    ev.events = EPOLLIN/* | EPOLLET*/;
                    ev.data.fd = client_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                        perror("epoll_ctl");
                        exit(EXIT_FAILURE);
                    }
                    Connection* conn = new Connection;
                    conn->clientSocket = client_fd;
                    mConnectionMap[client_fd] = conn;
                }
            } else {
                int client_fd = events[i].data.fd;
                char buffer[4096];
                //process(client_fd, buffer);
                
                mConnectionMap[client_fd]->processRead();
                
                // Request req;
                // int len = Trans::receive(client_fd, req);
				// req.print();
				// string x = req.serial();
                // if (len > 0) {
                //     std::cout << "get data:" << std::endl;
				// 	Response rsp(1, 2, 3, 4, 1234567890, 200, true, "example data");
                //     int ret = Trans::send(client_fd, rsp);
                //     cout << "return data" << buf;
                // }
            }
        }
    }
    close(server_fd);
}

//循环直到发送完一行内容
int Server::send(int clientfd, char *str, int len)
{
    n = sendline(clientfd, buf, len);
    return n;
}

Server* Server::mInstance = NULL;

Server *Server::GetInstance()
{
    if (mInstance == NULL) {
        mInstance = new Server;
    }
    return mInstance;
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
    request = parseRequest(requestData);
    if (!request) {
        // Clean up requestData if necessary
        return false;
    }

    response = new Response;
    requestProcessor[request->mFunctionCode]->Exec(this, *request, *response);
    response->print();
    delete request; // Clean up request after use
    if (!response) {
        // Clean up response if necessary
        return false;
    }

    if (request->mFunctionCode == FunctionCode::ProcessMessageRead) {
        //某些请求不返回相应的
        delete response;
        return true;
    }
    bool success = sendResponse(clientSocket, response);
    delete response; // Clean up response after use
    return success;
}

//flag = 0,默认关闭  flag = 1,强行关闭
bool Connection::closeConnection(int flag)
{
    //删除顺序有待确认，这是IO线程，某些对象在业务线程可能还在用
    if (epoll_ctl(Server::GetInstance()->epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL) == -1) {
        printf("epoll del error %d\n", clientSocket);
        perror("epoll_ctl: EPOLL_CTL_DEL");
        return false;
    }
    close(clientSocket);
    Connection* conn = Server::GetInstance()->mConnectionMap[clientSocket];
    if (conn == NULL) {
        return true;
    }
    Session* session = conn->session;
    if (session != NULL) {
        int userId = session->mUserId;
        Server::GetInstance()->mUserSessionMap.erase(userId);
        delete session;
    }
    Server::GetInstance()->mConnectionMap.erase(clientSocket);
    clientSocket = -1;
    delete conn;
    printf("size : %d %d\n", (int)Server::GetInstance()->mUserSessionMap.size(), (int)Server::GetInstance()->mConnectionMap.size());
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
