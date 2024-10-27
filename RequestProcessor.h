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
    bool bindAllFriendState(Connection* conn, Request& request, FriendList& friendList);
};

class SearchAllGroupProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool SearchAllGroup(const Request& request, std::vector<GroupInfo>& info);
    //bool bindAllFriendState(Connection* conn, Request& request, FriendList& friendList);
};

class FindFriendProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool FindFriend(const Request& request, FriendList& info);
};

class FindGroupProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool FindGroup(const Request& request, vector<GroupInfo>& infoList);
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

class ProcessFriendRequestProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessFriendRequest(Request &request, FriendRequest infoList);
};


class GetAllGroupReqProcessor : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool GetAllGroupReq(const Request &request, vector<GroupJoinRequest> &infoList);
};

class ProcessGroupJoinReqProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessGroupJoinRequest(Request &request, GroupJoinRequest info);
};

class UpdateUserStateProcessor  : public RequestProcessor  {
    void Exec(Connection* conn, Request& request, Response&);
    bool notifyStateToFriend(int userId, int state);
    bool UpdateUserState(const Request &request);
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
public:
    bool StoreFile(Request &request, FileInfo &fileObject);
    bool StoreFileSQL(Request &request, FileInfo &info);
    bool InitUserSpaceRoot(int user_id);
    int GetUserSpaceId(int user_id);
};


int stoiAll(const std::string &str);

class ProcessStartUpLoadFileProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessStartUpLoadFile(Request &request, FileInfo& info);
};

//通知服务器发送完毕
class ProcessUpLoadFileSuccessProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessUpLoadFileSuccess(Connection* conn, Request &request, FileInfo& info);
public:
    bool ProcessUpLoadSQL(Request &request, FileInfo& info);
    bool UpDateUserPhotoSQL(Request &request, FileInfo& info);
};

//主动获取，告知服务器要获取的文件， Response:验证文件是否存在和大小等参数
class ProcessGetFileProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessGetFile(Request &request, FileInfo& info);
};

class ProcessGetFileSuccessProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool ProcessGetFileSuccess(Request &request, FileInfo& info);
    bool setReadedBySQL(Request &request, FileInfo& info);
};

//被动接收，告知服务器要获取的文件， Response:验证文件是否存在和大小等参数
class ProcessNofifyFileComingProcessor : public RequestProcessor {
public:
    void Exec(Connection* conn, Request& request, Response&);
    bool NofifyFileComing(Connection* conn, Request &request, FileInfo& info);
    bool SendFileBySQL(Request &request, FileInfo& info);
    bool sendNotifyFileByNet(Connection *conn, FileInfo info);
};

//暂未用到，目前提示文件收发结束都由客户端处理
class ProcessTransFileOverProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool TransFileOver(Request &request, FileInfo& info);
};


class ProcessGetAllOfflineFileProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool GetAllOfflineFile(Request &request, vector<FileInfo> &infoList);
};

class ProcessGetOfflineFileProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
    bool GetOfflineFile(Request &request, FileInfo& info);
};

class ProcessNotifyStateProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response& response);


public:
    bool Notify(Connection *conn, FriendList &friendList, int mUserId, int state);
    bool Notify(Connection *conn, vector<int> &friendList, int mUserId, int state);
};


class ProcessFindSpaceFileTreeProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response& response);
    bool FindSpaceFileTree(const Request& request, vector<FileInfo>& fileList);
};

#endif

