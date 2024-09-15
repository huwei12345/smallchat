#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string>
#define BUF_SIZE 1024
#define MAX_EVENTS 1024

class Client {
public:
    Client(const char* ip = "127.0.0.1", unsigned short port = 8080);
    void run();
    void send(const char* str);
    std::string sendAndRecv(const char* str, int code = 0);
    ~Client();
private:
    int clientSocket;
};

#endif
