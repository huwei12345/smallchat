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
    virtual void Exec(Connection* conn, Request& request, Response&);
};

class LoginProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool Login(const std::string& account, const std::string& password, UserInfo& user);

};

class RegisterProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    int Register(const Request& request, UserInfo& info);
};

class SearchAllFriendProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool SearchAllFriend(const Request& request, FriendList& info);
};

class FindFriendProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool FindFriend(const Request& request, FriendList& info);
};

class AddFriendProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool AddFriend(const Request& request);
};

class SendMessageProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool SendMessage(const Request &request, MessageInfo &info);
    bool sendMessageByNet(Connection *conn, MessageInfo message);
};

class GetAllMessageProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool GetAllMessage(const Request &request, vector<MessageInfo> &infoList);
};

class GetAllFriendReqProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool GetAllFriendReq(const Request &request, vector<FriendRequest> &infoList);
};

class UpdateUserStateProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool notifyStateToFriend(int userId, int state);
    bool UpdateUserState(const Request &request);
};

class ProcessFriendRequestProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessFriendRequest(Request &request, FriendRequest infoList);
};

class ProcessMessageReadProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessMessageRead(Request &request);
};

class CreateGroupProcessor : public RequestProcessor
{
    void Exec(Connection* conn, Request& request, Response&);
    bool CreateGroup(Request &request, GroupInfo& info);
    bool AddGroupOwner(Request &request);
};

class JoinGroupProcessor : public RequestProcessor
{
    void Exec(Connection* conn, Request& request, Response&);
public:
    bool JoinGroup(int userId, int groupId, int role);
};

class ResponseJoinGroupProcessor : public RequestProcessor
{
    void Exec(Connection* conn, Request& request, Response&);
    bool ResponseJoinGroup(Request &request);
};

class StoreFileProcessor : public RequestProcessor
{
    void Exec(Connection* conn, Request& request, Response&);
    bool StoreFile(Request &request, FileObject &fileObject);
};

class TransFileProcessor : public RequestProcessor
{
    void Exec(Connection* conn, Request& request, Response&);
    bool TransFile(Request &request, TransObject &object);
};

int stoiAll(const std::string &str);

class ProcessStartUpLoadFileProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessStartUpLoadFile(Request &request, FileInfo& info);
};

//通知服务器发送完毕
class ProcessUpLoadFileSuccessProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessUpLoadFileSuccess(Request &request, FileInfo& info);
};

#endif

