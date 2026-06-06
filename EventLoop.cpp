#include "EventLoop.h"
#include "server.h"
#include <map>
#include <mutex>
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
    if (mEpollFd < 0) {
        printf("EventLoop: createEpoll failed\n");
    }
    mServerSocket = serverFd;
}
int EventLoop::createEpoll()
{
    mEpollFd = epoll_create1(0);
    if (mEpollFd < 0) {
        perror("epoll_create1");
        return -1;
    }
    return mEpollFd;
}

void EventLoop::Run()
{
    if (mEpollFd < 0) {
        printf("EventLoop::Run: invalid epoll fd\n");
        return;
    }
    while (mRunning) {
        //-1 不主动返回
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, 100);
        if (nfds < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == mWakeupSocket[1]) {
                printf("son waked %d\n", mIndex);
                //WakeUp应该单独用于addSocket，可以不用TaskQueue，但是要考虑多线程竞争accept
                int wakeup;
            	read(mWakeupSocket[1], (char*)&wakeup, sizeof(int));
                std::queue<Task*> localQueue;
                {
                    std::lock_guard<std::mutex> lock(mTaskQueueMutex);
                    localQueue.swap(mTaskQueue);
                }
                while (!localQueue.empty()) {
                    Task* task = localQueue.front();
                    localQueue.pop();
                    if (task->type == ADD) {
                        addSocket();
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
            else if (events[i].data.fd != mWakeupSocket[1]) {
                int client_fd = events[i].data.fd;
                Connection* conn = nullptr;
                {
                    std::lock_guard<std::mutex> lock(mServer->mConnectionMapMutex);
                    auto it = mServer->mConnectionMap.find(client_fd);
                    if (it != mServer->mConnectionMap.end()) {
                        conn = it->second;
                    }
                }
                if (conn) {
                    conn->processRead();
                }
            }
        }
        doOtherThing();
    }
}

void EventLoop::RunMain()
{
    if (mEpollFd < 0) {
        printf("EventLoop::RunMain: invalid epoll fd\n");
        return;
    }
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    //TODO:ServerSocket设置成EPOLLET需要一些适配
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = mServerSocket;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mServerSocket, &ev) < 0) {
        printf("xxxxxxxxxxx %d %d\n", mEpollFd, mServerSocket);
        perror("epoll_ctl");
        return;
    }

    // 当有事件发生时，处理事件并将socket重新添加到epoll中
    while (mRunning) {
        int nfds = epoll_wait(mEpollFd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
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
        }
        printf("accept error: fd=%d errno=%d\n", mServerSocket, errno);
        perror("accept");
        return;
    }
    std::cout << "EventLoop" << mIndex << " : get connect " << client_fd << std::endl;
    // 将client socket设置为非阻塞模式
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        close(client_fd);
        return;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN/* | EPOLLET*/;
    ev.data.fd = client_fd;
    if (epoll_ctl(mEpollFd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        perror("epoll_ctl");
        close(client_fd);
        return;
    }
    Connection* conn = new Connection(client_fd, this);
    {
        std::lock_guard<std::mutex> lock(mServer->mConnectionMapMutex);
        mServer->mConnectionMap[client_fd] = conn;
    }
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
    if (task->mData == nullptr)
        return true;
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
                task->mData = nullptr;
            }
            return ret;
        }
        else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            printf("write error %d\n", errno);
            if (task->mData) {
                delete task->mData;
                task->mData = nullptr;
            }
            return ret;
        }
    }
    if (task->mData) {
        delete task->mData;
        task->mData = nullptr;
    }
    return len;
}

void EventLoop::wakeup()
{
    int wakeup = 1;
    Task *task = new Task;
    task->type = ADD;
    {
        std::lock_guard<std::mutex> lock(mTaskQueueMutex);
        mTaskQueue.push(task);
    }
	write(mWakeupSocket[0], &wakeup, sizeof(int));
}

void EventLoop::doOtherThing()
{
    std::queue<Task*> localQueue;
    {
        std::lock_guard<std::mutex> lock(mTaskQueueMutex);
        localQueue.swap(mTaskQueue);
    }
    while (!localQueue.empty()) {
        Task* task = localQueue.front();
        localQueue.pop();
        if (task->type == ADD) {
            addSocket();
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
    Task *task = new Task;
    task->sockFd = fd;
    task->mData = new std::string(data);
    task->type = WRITE;
    {
        std::lock_guard<std::mutex> lock(mTaskQueueMutex);
        mTaskQueue.push(task);
    }
    return false;
}