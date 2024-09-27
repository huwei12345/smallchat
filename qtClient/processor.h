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
    static bool getAllMessage(int userId);
    static bool getAllFriendRequest(int userId);
    static bool ChangeUserState(int state);
    static bool processFriendRequest(FriendRequest friendRequest);
static bool CreateGroup(GroupInfo& info);
    static bool JoinGroup(int groupId);
    static bool FindGroup(int groupId);
    static bool SendFile(std::string path, std::string file);
    static bool SendMessageSuccess(std::string file);
    static bool GetMessageSuccess(std::string file);
    static bool GetFile(std::string path, std::string filename);
    bool processMessageRead(std::vector<int> messageList);
};

#endif // PROCESSOR_H
