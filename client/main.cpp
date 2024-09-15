#include "client.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[]) {
    Client client;
    int code = 0;
    if (argc < 2) {
        printf("usage ./client code\n");
        code = 0;
    }
    else {
        code = atoi(argv[1]);
    }
    std::string str = client.sendAndRecv("12345\n", code);
    std::cout << str << std::endl;
    sleep(3);
    return 0;
}
