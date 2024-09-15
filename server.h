#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <map>
#define BUF_SIZE 1024
#define MAX_EVENTS 1024
class Connection;
class Session;

class Server {
public:
    void run();
    int recieve(int clientfd, char* str, int len);
    int send(int clientfd, char* str, int len);
    static Server* GetInstance(); 
    std::map<int, Connection*> mConnectionMap;//连接层 fd - conn
    std::map<int, Session*> mUserSessionMap;//会话层 userId - conn
private:
    Server(const char* ip = NULL, unsigned int port = 8080);
    int epoll_fd;
    int server_fd;
    int client_fd[1024]; 
    int n, i;
    struct sockaddr_in server_addr;
    struct epoll_event events[MAX_EVENTS];
    char buf[BUF_SIZE];
    static Server* mInstance;
};

#endif