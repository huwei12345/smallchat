#include "EventLoop.h"
#include "server.h"
#include <map>
#include "Protocol.h"
EventLoop::EventLoop()
{
    mIndex = -1;//-1是主线程
    mServer = NULL;
    mEpollFd = createEpoll();
    mRunning = true;
}
EventLoop::EventLoop(Server *server, int serverFd, int index)
{
    mServer = server;
    mIndex = index;
    mEpollFd = createEpoll();
    mServerSocket = serverFd;
}
int EventLoop::createEpoll()
{
    mEpollFd = epoll_create1(0);
    if (mEpollFd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    return mEpollFd;
}

void EventLoop::Run()
{
    while (mRunning) {
        //-1 不主动返回
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, 100);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == mWakeupSocket[1]) {
                printf("son waked %d\n", mIndex);
                //WakeUp应该单独用于addSocket，可以不用TaskQueue，但是要考虑多线程竞争accept
                int wakeup;
            	read(mWakeupSocket[1], (char*)&wakeup, sizeof(int));
                for (int j = 0; j < mTaskQueue.size(); j++) {
                    Task* task = mTaskQueue.front();
                    mTaskQueue.pop();
                    if (task->type == ADD) {
                        addSocket();//accept
                    }
                    else if (task->type == ERASE) {
                        eraseSocket(task->sockFd);
                    }
                    else if (task->type == CHANGE) {
                        changeSocket(task);
                    }
                    else if (task->type == ALTRM) {
                        doAltrmTask(task);
                    }
                    delete task;
                }
            }
            else if (events[i].data.fd != mWakeupSocket[1]) {
                int client_fd = events[i].data.fd;
                char buffer[4096];
                mServer->mConnectionMap[client_fd]->processRead();
            }
        }
        doOtherThing();
    }
}

void EventLoop::RunMain()
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    //TODO:ServerSocket设置成EPOLLET需要一些适配
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = mServerSocket;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mServerSocket, &ev) < 0) {
        printf("xxxxxxxxxxx %d %d\n", mEpollFd, mServerSocket);
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    // 当有事件发生时，处理事件并将socket重新添加到epoll中
    while (mRunning) {
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t client_len = 0;
        // 当有事件发生时，处理事件并将socket重新添加到epoll中
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == mServerSocket) {
                int wakeupId = mServer->selectAlgorithm();
                printf("wake up %d\n", wakeupId);
                mServer->mEvLoopList[wakeupId]->wakeup();
            }
        }
    }
    close(mServerSocket);
}


bool EventLoop::addWakeupSocket() {
    struct epoll_event ev;
    if (int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, mWakeupSocket) != 0) {
        return false;
    }
    ev.data.fd = mWakeupSocket[1];
    ev.events = EPOLLIN/* | EPOLLET*/;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mWakeupSocket[1], &ev) < 0) {
        printf("xxxxxxxxxxx222\n");
        perror("epoll_ctl");
        return false;
    }
    return true;
}

void EventLoop::addSocket()
{
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = 0;
    int client_fd = ::accept(mServerSocket, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        } else {
            printf("%d %d %d\n", mServerSocket, client_fd, errno);
            perror("accept");
            exit(EXIT_FAILURE);
        }
    }
    std::cout << "EventLoop" << mIndex << " : get connect " << client_fd << std::endl;
    // 将client socket设置为非阻塞模式
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    struct epoll_event ev;
    ev.events = EPOLLIN/* | EPOLLET*/;
    ev.data.fd = client_fd;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
    //TODO：每个EventLoop分别管理连接和session还是总体一起管理？
    Connection* conn = new Connection(client_fd, this);
    mServer->mConnectionMap[client_fd] = conn;
}

bool EventLoop::eraseSocket(int fd)
{
    printf("epoll del %d success\n", fd);
    if (epoll_ctl(mEpollFd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        printf("epoll del error %d\n", fd);
        perror("epoll_ctl: EPOLL_CTL_DEL");
        return false;
    }
    return true;
}

void EventLoop::changeSocket(Task *task)
{
}

void EventLoop::doAltrmTask(Task *task)
{
}

bool EventLoop::doWrite(Task *task)
{
    //TODO:一次性全部写出，不太好，有缓冲区后修改
    int fd = task->sockFd;
    const char* data = task->mData->c_str();
    int len = task->mData->size();
    int pos = 0;
    while (pos != len) {
        int ret = ::send(fd, data + pos, len - pos, 0);
        if (ret != -1) { 
            pos += ret;
        }
        else if (ret == 0) {
            if (task->mData) {
                delete task->mData;
            }
            return ret;
        }
        else {
            if (errno = EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            if (task->mData) {
                delete task->mData;
            }
            return ret;
        }
    }
    if (task->mData) {
        delete task->mData;
    }
    return len;
}

void EventLoop::wakeup()
{
    int wakeup = 1;
    Task *task = new Task;
    task->type = ADD;
    mTaskQueue.push(task);
	write(mWakeupSocket[0], &wakeup, sizeof(int));
}

void EventLoop::doOtherThing()
{
    for (int j = 0; j < mTaskQueue.size(); j++) {
        Task* task = mTaskQueue.front();
        mTaskQueue.pop();
        if (task->type == ADD) {
            addSocket();//accept
        }
        else if (task->type == ERASE) {
            eraseSocket(task->sockFd);
        }
        else if (task->type == CHANGE) {
            changeSocket(task);
        }
        else if (task->type == ALTRM) {
            doAltrmTask(task);
        }
        else if (task->type == WRITE) {
            doWrite(task);
        }
        delete task;
    }
}

bool EventLoop::sendDataAll(int fd, const char* data, int len)
{
    std::string str(data, data + len);
    return sendDataAll(fd, str);
}

bool EventLoop::sendDataAll(int fd, const std::string& data)
{
    //方式1：操作写缓冲区，并触发写事件
    //方式2：添加Task在do_otherThing中进行将写操作作为任务，写出。
    //目前没有写缓冲区，所以采用方式2
    Task *task = new Task;
    task->sockFd = fd;
    task->mData = new std::string(data);
    task->type = WRITE;
    mTaskQueue.push(task);
    return false;
}