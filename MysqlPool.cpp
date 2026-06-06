#include "MysqlPool.h"
#include "Logger.h"

MysqlPool *MysqlPool::GetInstance()
{
    static MysqlPool pool;
    return &pool;
}

MysqlPool::MysqlPool(int handleNumber, int flag) : mCapacity(handleNumber), mFlag(flag), mIdleSize(0), mStoping(false), mWaitTimeoutMs(5000)
{
    mDriver = sql::mysql::get_driver_instance();
    pthread_cond_init(&mCond, NULL);
}

MysqlPool::~MysqlPool()
{
    mStoping = true;
    pthread_cond_broadcast(&mCond);
    for (int i = 0; i < mCapacity; i++) {
        mConnectionPool[i]->close();
    }
    pthread_cond_destroy(&mCond);
}

bool MysqlPool::init(std::string host, std::string user, std::string passwd)
{
    for (int i = 0; i < mCapacity; i++) {
        sql::Connection* conn = mDriver->connect(host, user, passwd);
        if (!conn->isValid()) {
            printf("sql init conn %d failure\n", i);
            LOG_ERROR("mysql connection {} init failed", i);
            return false;
        }
        else {
            printf("sql init conn %d success\n", i);
            LOG_INFO("mysql connection {} initialized", i);
        }
        mConnectionPool.push_back(conn);
        mIdleConnectionQue.push(conn);
        mIdleSize++;
    }
    mStoping = false;
    return true;
}

sql::Connection* MysqlPool::getConnection() {
    if (mStoping) {
        return NULL;
    }
    mMutex.Lock();
    sql::Connection *conn = NULL;
    if (!mIdleConnectionQue.empty()) {
        conn = mIdleConnectionQue.front();
        mIdleConnectionQue.pop();
        mIdleSize--;
        mMutex.Unlock();
        return conn;
    }

    // 等待连接归还，带超时
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += mWaitTimeoutMs / 1000;
    ts.tv_nsec += (mWaitTimeoutMs % 1000) * 1000000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }

    int ret = pthread_cond_timedwait(&mCond, mMutex.nativeHandle(), &ts);
    if (ret == 0 && !mIdleConnectionQue.empty() && !mStoping) {
        conn = mIdleConnectionQue.front();
        mIdleConnectionQue.pop();
        mIdleSize--;
    }
    mMutex.Unlock();
    if (!conn) {
        LOG_WARN("getConnection timeout, no idle connection available");
    }
    return conn;
}

void MysqlPool::releaseConncetion(sql::Connection* conn) {
    mMutex.Lock();
    if (!mStoping) {
        mIdleConnectionQue.push(conn);
        mIdleSize++;
        pthread_cond_signal(&mCond);
    }
    mMutex.Unlock();
}