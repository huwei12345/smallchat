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
    for (int i = 0; i < FunctionCode::FunctionCodeString.size(); i++) {
        cout << i << "  " << FunctionCode::FunctionCodeString[i] << endl;
    }
    MysqlPool* mysqlPool = MysqlPool::GetInstance();
    mysqlPool->init();
    Server* server = Server::GetInstance();
    server->run();
    cout << "hello world!" << endl;
}
