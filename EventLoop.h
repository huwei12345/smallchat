#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <thread>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include <queue>
#include "Protocol.h"
class Server;

class Task {
public:
    int sockFd;
    int type;
    std::string* mData;
    Task() : mData(nullptr) { }
    ~Task() { if (mData != nullptr) delete mData; }
};

class EventLoop {
public:
    EventLoop();
    EventLoop(Server *server, int serverFd, int index = -1);
    enum TaskType {
        ADD,
        ERASE,
        CHANGE,
        ALTRM,
        BASE,//基础事件
        OTHER,
        WRITE,
    };
    void start() {
        mRunning = true;
        //loopThread = std::thread([this]() { this->loop(); });
    }
    int createEpoll();
    void Run();
    void RunMain();

    bool addWakeupSocket();
    void addSocket(); // 主动accept
    bool eraseSocket(int fd);
    void changeSocket(Task* task);
    void doAltrmTask(Task* task);
    bool doWrite(Task* task);
    void wakeup();
    void doOtherThing();
    std::thread* mThread;
    bool sendDataAll(int fd, const char *data, int len);
    bool sendDataAll(int fd, const std::string &data);

private:
    bool mRunning;
    int mServerSocket;
    int mEpollFd;
    int mWakeupSocket[2];
    int mIndex;
    Server* mServer;
    struct epoll_event events[MAX_EVENTS];
    std::queue<Task*> mTaskQueue;
};

#endif