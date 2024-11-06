#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <vector>

//其实不是线程池，只是一个线程管理类，因为不需要线程池等待执行任务，IO线程是一直阻塞的
class ThreadPool {
public:
    ThreadPool(int number = 8);
    void getThread(int index);
private:
    bool isRunning;
    int mThreadNum;
    int mIOThreadNum;
    int mDataBaseThread;

    int mProcessThreadNum;
    int mHttpThreadNum;
    std::vector<std::thread*> threadList;
};

#endif