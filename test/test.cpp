#include "MyProtocolStream.h"
#include <iostream>
#include <string.h>
using namespace std;
using namespace net;

void print(string& s) {
    for (int i = 0; i < s.length(); i++) {
        unsigned char c = s[i];
        std::cout << std::hex << (unsigned int)c << " ";
    }
    std::cout << std::endl;
}

/*
        length(4B) |  CRC(2B)


*/

int main() {
    string request;
    MyProtocolStream reqStream(request, PROTOCOL_PACK);
    print(request);
    reqStream.loadInt32(1000);
    print(request);
    reqStream.loadString("hello");
    print(request);
    reqStream.loadCString("world", 6);
    print(request);
    float ff = 3.14;
    reqStream.loadFloat(ff);
    print(request);
    char c1 = 'a';
    reqStream.loadChar(c1);
    reqStream.flush();
    print(request);
    
    int x = 0;
    reqStream.getInt32(x);
    printf("x = %d m_pos = %d\n", x, reqStream.currentIndex());
    string s;
    reqStream.getString(s);
    s += '\0';
    printf("%s  m_pos = %d\n", s.c_str(), reqStream.currentIndex());
    char ccc[128];
    memset(ccc, 0, sizeof(ccc));
    int lll = 0;
    reqStream.getCString(ccc, lll);
    printf("%s %d\n", ccc, lll);
    float fff;
    reqStream.getFloat(fff);
    printf("fff %f\n", fff);
    char c2;
    reqStream.getChar(c2);
    printf("c2 %c\n", c2);
    reqStream.clear();
    printf("m_pos = %d\n",reqStream.currentIndex());
    reqStream.setPos(0);
    printf("\n`````````````````````````````````````````\n");
    
    print(request);
    reqStream.loadInt32(3000000);
    //print(request);
    reqStream.loadString("hahahahahaha");
    //print(request);
    reqStream.flush();
    print(request);
    reqStream.getInt32(x);
    printf("x = %d m_pos = %d\n", x, reqStream.currentIndex());
    s = "";
    reqStream.getString(s);
    s += '\0';
    printf("%s  m_pos = %d\n", s.c_str(), reqStream.currentIndex());
    //print(request);
    reqStream.clear();
    printf("m_pos = %d\n",reqStream.currentIndex());
    reqStream.setPos(0);

    printf("\n-------------------------------\n");
    //导入
    reqStream << (int)1000 << "Hello World!" << (float)3.14 << (double)6.28 << (short)20 << endll;
    // reqStream << (int)1000;
    // print(request);
    // reqStream << "Hello World!";
    // print(request);
    // reqStream << (float)3.14;
    // print(request);
    // reqStream << (double)6.28;
    // print(request);
    // reqStream << (short)20;
    // print(request);
    // reqStream << net::endl;
    // print(request);

    int xx;
    string h;
    float f;
    double d;
    short m;
    print(request);
    printf("len = %d\n", reqStream.size());
    //导出
    reqStream >> xx >> h >> f >> d >> m;
    std::cout << std::dec << xx << " " << h << " " << f << " " << d << " " << m << std::endl;
}
