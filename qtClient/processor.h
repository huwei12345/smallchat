#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "Protocol.h"

extern int user_id;

class Processor {
public:
    Processor();
    static bool Login(std::string& username, std::string& password);
    static bool Register(std::string& username, std::string& password, std::string& email);
    static bool SendMessage(int reciveId, std::string& content);
    static bool FindFriend(int friendId);
    static bool FindFriendByName(std::string &friendName);
    static bool AddFriend(int friendId);
    static bool findAllFriend(int userId);
    static bool findAllGroup(int userId);
    static bool getAllMessage(int userId);
    static bool getAllFriendRequest(int userId);
    static bool getAllGroupRequest(int userId);
    static bool ChangeUserState(int state);
    static bool processFriendRequest(FriendRequest friendRequest);
    static bool processGroupJoinRequest(GroupJoinRequest friendRequest);


    static bool CreateGroup(GroupInfo& info);
    static bool JoinGroup(int groupId);
    static bool FindGroup(int groupId);
    static bool FindGroupByName(std::string &groupName);

    static bool SendFile(FileInfo info);
    static bool GetFile(FileInfo info);
    static bool GetFileSuccess(FileInfo info);
    static bool SendFileSuccess(FileInfo info);

    static bool AgreeRecvFile(bool agree, FileInfo info);
    static bool getAllOfflineFile(int userId);

    static bool StoreFile(FileInfo info);

    bool processMessageRead(std::vector<int> messageList);
    static bool getFriendPhoto(UserInfo& info);

    static bool findSpaceFileTree(int userId);
};

#endif // PROCESSOR_H
