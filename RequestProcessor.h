#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H
#include <string>
#include <iostream>
#include <vector>
#include "Protocol.h"
class Request;
class Response;

class RequestProcessor {
public:
    virtual void Exec(Request& request, Response&);
};

class LoginProcessor : public RequestProcessor {
    void Exec(Request& request, Response&);
    bool Login(const std::string& account, const std::string& password, UserInfo& user);

};

class RegisterProcessor : public RequestProcessor  {
    void Exec(Request& request, Response&);
    int Register(const Request& request, UserInfo& info);
};

class SearchAllFriendProcessor : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool SearchAllFriend(const Request& request, FriendList& info);
};

class FindFriendProcessor  : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool FindFriend(const Request& request, FriendList& info);
};

class AddFriendProcessor  : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool AddFriend(const Request& request);
};

class SendMessageProcessor : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool SendMessage(const Request& request);
};

class GetAllMessageProcessor : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool GetAllMessage(const Request &request, vector<MessageInfo> &infoList);
};

class GetAllFriendReqProcessor : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool GetAllFriendReq(const Request &request, vector<FriendRequest> &infoList);
};

class UpdateUserStateProcessor  : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool UpdateUserState(const Request &request);
};

class ProcessFriendRequestProcessor  : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool ProcessFriendRequest(Request &request, FriendRequest infoList);
};

class ProcessMessageReadProcessor  : public RequestProcessor  {
    void Exec(Request& request, Response&);
    bool ProcessMessageRead(Request &request);
};

#endif

