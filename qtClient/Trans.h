#ifndef TRANS_H
#define TRANS_H
//#include <iostream>
//#include "Protocol.h"
//#include <memory.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <arpa/inet.h>
//using namespace std;

////server
//namespace Trans {

//int send(int fd, const string& str) {
//	//阻塞发送
//	int total = 0;
//	int len = str.length();
//	const char* src = str.c_str();
//	while (total < len) {
//		int ret = ::send(fd, src + total, len - total, 0);
//		if (ret > 0) {
//			total += ret;
//		}
//		else if (ret == 0) {
//			return 0;
//		}
//		else {
//			if (errno == EWOULDBLOCK || errno == EAGAIN) {
//				continue;
//			}
//			std::cout << "Send Error ret = -1 errno = " << errno << std::endl;
//			return -1;
//		}
//	}
//	return len;
//}


//int receive(int fd, string& ans, int len) {
//	char *buf = new char[len];
//	int total = 0;
//	while (total < len) {
//		int ret = ::recv(fd, buf + total, len - total, 0);
//		if (ret > 0) {
//			total += ret;
//		}
//		else if (ret == 0) {
//			return 0;
//		}
//		else {
//			if (errno == EWOULDBLOCK || errno == EAGAIN) {
//				continue;
//			}
//			std::cout << "Receive Error ret = -1 errno = " << errno << std::endl;
//			return -1;
//		}
//	}
//	ans.assign(buf, buf + len);
//	return len;
//}

//bool send(int fd, Request& req) {
//	string ans = req.serial();
//	int ret = send(fd, ans);
//	if (ret > 0) {
//		return true;
//	}
//	return false;
//}

//int receive(int fd, Response& rsp) {
//	string ans;
//	int ret = receive(fd, ans, 4);
//	if (ret != 4) {
//		return -1;
//	}
//	int len = 0;
//	memcpy(&len, ans.c_str(), 4);
//	len = htonl(len);
//	if (len - 4 <= 0) {
//		return -1;
//	}
//	string other;
//	ret = receive(fd, other, len - 4);
//	if (ret <= 0) {
//		return -1;
//	}
//	ans += other;
//	rsp.deserial(ans);
//	//TODO:在deserial里CRC校验
//	return len;
//}



//// client

//bool send(int fd, Response& rsp) {
//	string ans = rsp.serial();
//	int ret = send(fd, ans);
//	if (ret > 0) {
//		return true;
//	}
//	return false;
//}

//int receive(int fd, Request& req) {
//	string ans;
//	int ret = receive(fd, ans, 4);
//	if (ret != 4) {
//		return -1;
//	}
//	int len = 0;
//	memcpy(&len, ans.c_str(), 4);
//	len = htonl(len);
//	if (len - 4 <= 0) {
//		return -1;
//	}
//	string other;
//	ret = receive(fd, other, len - 4);
//	if (ret <= 0) {
//		return -1;
//	}
//	ans += other;
//	req.deserial(ans);
//	//TODO:在deserial里CRC校验
//	return len;
//}

//}
#endif
