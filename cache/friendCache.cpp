#include "friendCache.h"


std::vector<int> FriendCache::getFriendList(int userId)
{
    if (mFriendCache.count(userId))
        return mFriendCache[userId];
    return {};
}

void FriendCache::addFriend(int userId, int friendId)
{
    mFriendCache[userId].push_back(friendId);
}

void FriendCache::updateFriendList()
{

}

void FriendCache::eraseFriendList(int userId)
{
    mFriendCache.erase(userId);
}

FriendCache * FriendCache::GetInstance()
{
    static FriendCache* cache = new FriendCache;
    return cache;
}
