#include<iostream>
#include"Protocol.h"
using namespace std;
using namespace net;

void test() {
	string request;
	MyProtocolStream stream(request, PROTOCOL_PACK);
	print(request);
	stream.loadInt32(1000);	
	print(request);
	stream.flush();
	print(request);
	printf("\n\n");
	Request req;
	req.mType = 1;
    req.mFunctionCode = 2;
    req.mFlag = 3;
    req.mDirection = 4;
    req.mTimeStamp = 11;//时间戳和任务编号
    req.mData = "hello world!";
	std::string ans = req.serial();
	print(ans);
	Request copy;
	copy.deserial(ans);
	copy.print();
	printf("\n\n");
	Response rsp(1, 2, 3, 4, 1234567890, 200, true, "example data");
	rsp.print();
	std::string ans2 = rsp.serial();
	print(ans2);
	Response copyR;
	copyR.deserial(ans2);
	copyR.print();
}
