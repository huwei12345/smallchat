#ifndef MUTEX_H
#define MUTEX_H

#define POSIX 1
#include <assert.h>
//#include "version.h"

#include <malloc.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
class Mutex
{
public:

    Mutex(const char *name = NULL) {
#ifdef _MSC_VER
        mHandle = CreateMutex(0, 0, name);
        assert(mHandle);
#else
#if POSIX
        int err = pthread_mutex_init(&mHandle, NULL);
        if(err < 0) {
            printf("Thread Mutex Init failed\n");
        }
        assert(err == 0);
#else
        int err = rt_mutex_create(&mHandle, name);
        if(err < 0) {
            printf("Thread Mutex Init failed\n");
        }
        assert(err == 0);
#endif
#endif
    }

    ~Mutex() {
#ifdef _MSC_VER
        CloseHandle(mHandle);
#else
#if POSIX
        pthread_mutex_destroy(&mHandle);
#else
        rt_mutex_delete(&mHandle);
#endif
#endif
    }

    void Lock() {
#ifdef _MSC_VER
        DWORD dwWaitResult;
        dwWaitResult = WaitForSingleObject(mHandle, INFINITE);
        assert(dwWaitResult == WAIT_OBJECT_0);
#else
#if POSIX
        int err = pthread_mutex_lock(&mHandle);
        if(err < 0) {
            printf("Thread Mutex lock failed\n");
        }
        assert(err == 0);
#else
        int err = rt_mutex_acquire(&mHandle, TM_INFINITE);
        if(err < 0) {
            printf("Thread Mutex lock  failed\n");
        }
        assert(err == 0);
#endif
#endif
    }


    void Unlock() {
#ifdef _MSC_VER
        int err = ReleaseMutex(mHandle);
        if(err < 0) {
            printf("Thread Mutex unlock failed\n");
        }
        assert(err);
#else
#if POSIX
        int err = pthread_mutex_unlock(&mHandle);
        if(err < 0) {
            printf("Thread Mutex unlock  failed\n");
        }
        assert(err == 0);
#else
        int err = rt_mutex_release(&mHandle);
        if(err < 0) {
            printf("Thread Mutex unlock failed\n");
        }
        assert(err == 0);
#endif
#endif
    }
private:
#ifdef _MSC_VER
    HANDLE mHandle;
#else
#if POSIX
    pthread_mutex_t mHandle;
#else
    RT_MUTEX mHandle;
#endif
#endif
};

#endif
