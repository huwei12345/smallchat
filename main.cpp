#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "server.h"
#include "MysqlPool.h"
using namespace std;

int main() {
    MysqlPool* mysqlPool = MysqlPool::GetInstance();
    mysqlPool->init();
    Server* server = Server::GetInstance();
    server->run();
    cout << "hello world!" << endl;
}
