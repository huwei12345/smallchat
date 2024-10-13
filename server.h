#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <map>
#include <string>
#include "Protocol.h"
class Connection;
class Session;
class Request;
class Response;
class EventLoop;
//bool readRequest(int fd, char* buf, std::string& requestData);
Request* parseRequest(std::string& RequestData);
//Response* processRequest(Request* request);
// bool sendDataAll(int fd, const char* data, int len);
// bool sendDataAll(int fd, std::string data);
// bool sendResponse(int fd, Response* response);
std::string serialResponse(Response* response); 
//bool process(int fd, char* buf);
using CallBack = void(void*);

class Server {
public:
    int run();
    int createListener();
    int selectAlgorithm();
    static Server* GetInstance(); 
    std::map<int, Connection*> mConnectionMap;//连接层 fd - conn
    std::map<int, Session*> mUserSessionMap;//会话层 userId - conn
    vector<EventLoop*> mEvLoopList;
    int mEpollFd;
    int mServerSocket;
    EventLoop* mMainEventLoop;
private:
    Server(const char* ip = NULL, unsigned int port = 8080);
    int client_fd[1024]; 
    int n, i;
    struct sockaddr_in server_addr;
    struct epoll_event events[MAX_EVENTS];
    char buf[BUF_SIZE];
    static Server* mInstance;
    std::string mServerIP;
    unsigned short mServerPort;
};

#endif