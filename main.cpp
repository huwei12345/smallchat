#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "server.h"
#include "MysqlPool.h"
#include "ServerConfig.h"
#include "Logger.h"
using namespace std;

int main() {
    signal(SIGPIPE, SIG_IGN);

    // 加载配置
    ServerConfig* config = ServerConfig::GetInstance();
    config->load("server_config.ini");

    // 初始化日志系统
    Logger::init(config->getString("log", "level", "info"));
    LOG_INFO("smallchat server starting...");

    for (int i = 0; i < FunctionCode::FunctionCodeString.size(); i++) {
        cout << i << "  " << FunctionCode::FunctionCodeString[i] << endl;
    }

    LOG_INFO("MySQL connecting to {}", config->getMysqlHost());
    MysqlPool* mysqlPool = MysqlPool::GetInstance();
    mysqlPool->init(config->getMysqlHost(), config->getMysqlUser(), config->getMysqlPassword());
    LOG_INFO("MySQL pool initialized");

    Server* server = Server::GetInstance();
    LOG_INFO("Server listening on port {}", config->getServerPort());
    server->run();
    LOG_INFO("server stopped");
}
