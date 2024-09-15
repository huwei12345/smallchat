#ifndef MYSQL_POOL_H
#define MYSQL_POOL_H
#include <vector>
#include <queue>
#include <string>
#include <mysql-cppconn/jdbc/mysql_connection.h>
#include <mysql-cppconn/jdbc/mysql_driver.h>
#include <mysql-cppconn/jdbc/mysql_error.h>
#include <mysql-cppconn/jdbc/cppconn/statement.h>
#include <mysql-cppconn/jdbc/cppconn/prepared_statement.h>
#include <mysql-cppconn/jdbc/cppconn/resultset.h>
#include "mutex.h"
class MysqlPool {
public:
    static MysqlPool* GetInstance(); 
    MysqlPool(int handleNumber = 4, int flag = 0);
    ~MysqlPool();
    bool init(std::string host = "tcp://127.00.1:3306/mIM", std::string user = "root", std::string passwd = "123456");

    sql::Connection *getConnection();

    void releaseConncetion(sql::Connection *conn);

private:
    static MysqlPool* mPool;
    bool mStoping;
    int mFlag;
    int mCapacity;
    int mIdleSize;
    sql::Driver* mDriver;
    std::string mHost;
    std::string mUser;
    std::string mPasswd;
    std::vector<sql::Connection*> mConnectionPool;
    std::queue<sql::Connection*> mIdleConnectionQue;
    Mutex mMutex;

};


#endif