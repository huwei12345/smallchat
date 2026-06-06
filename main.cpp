#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "server.h"
#include "MysqlPool.h"
#include "ServerConfig.h"
using namespace std;

int main() {
    // 加载配置
    ServerConfig* config = ServerConfig::GetInstance();
    config->load("server_config.ini");

    for (int i = 0; i < FunctionCode::FunctionCodeString.size(); i++) {
        cout << i << "  " << FunctionCode::FunctionCodeString[i] << endl;
    }

    MysqlPool* mysqlPool = MysqlPool::GetInstance();
    mysqlPool->init(config->getMysqlHost(), config->getMysqlUser(), config->getMysqlPassword());

    Server* server = Server::GetInstance();
    server->run();
    cout << "hello world!" << endl;
}
