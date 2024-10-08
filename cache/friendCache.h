#ifndef FRIEND_CACHE
#define FRIEND_CACHE
#include <map>
#include <vector>

class FriendCache {
public:
    static FriendCache* GetInstance();
    std::map<int, std::vector<int>> mFriendCache;
    std::vector<int> getFriendList(int userId);
    void addFriend(int userId, int friendId);
    void updateFriendList();//从数据库中更新缓存
    void eraseFriendList(int userId);
};


#endif