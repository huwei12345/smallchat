#include "MysqlPool.h"

MysqlPool* MysqlPool::mPool = NULL;

MysqlPool *MysqlPool::GetInstance()
{
    if (mPool == NULL) {
        mPool = new MysqlPool();
    }
    return mPool;
}

MysqlPool::MysqlPool(int handleNumber, int flag) : mCapacity(handleNumber), mFlag(flag)
{
    mDriver = sql::mysql::get_driver_instance();
}

MysqlPool::~MysqlPool()
{
    mStoping = true;
    for (int i = 0; i < mCapacity; i++) {
        mConnectionPool[i]->close();
    }
}

bool MysqlPool::init(std::string host, std::string user, std::string passwd)
{
    for (int i = 0; i < mCapacity; i++) {
        sql::Connection* conn = mDriver->connect(host, user, passwd);
        if (!conn->isValid()) {
            printf("sql init conn %d failure\n", i);
            return false;
        }
        else {
            printf("sql init conn %d success\n", i);
        }
        mConnectionPool.push_back(conn);
        mIdleConnectionQue.push(conn);
        mIdleSize++;
    }
    mStoping = false;
    return true;
}

sql::Connection* MysqlPool::getConnection() {
    if (!mIdleConnectionQue.empty() && !mStoping) {
        mMutex.Lock();
        sql::Connection *conn = NULL;
        if (!mIdleConnectionQue.empty()) {
            conn = mIdleConnectionQue.front();
            mIdleConnectionQue.pop();
            mIdleSize--;
        }
        mMutex.Unlock();
        return conn;
    }
    return NULL;
}

void MysqlPool::releaseConncetion(sql::Connection* conn) {
    mMutex.Lock();
    if (!mStoping) {
        mIdleConnectionQue.push(conn);
        mIdleSize++;
    }
    mMutex.Unlock();
}