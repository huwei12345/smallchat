#include "MysqlPool.h"
#include "Logger.h"

MysqlPool *MysqlPool::GetInstance()
{
    static MysqlPool pool;
    return &pool;
}

MysqlPool::MysqlPool(int handleNumber, int flag) : mCapacity(handleNumber), mFlag(flag), mIdleSize(0), mStoping(false)
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
    }
    mMutex.Unlock();
    return conn;
}

void MysqlPool::releaseConncetion(sql::Connection* conn) {
    mMutex.Lock();
    if (!mStoping) {
        mIdleConnectionQue.push(conn);
        mIdleSize++;
    }
    mMutex.Unlock();
}