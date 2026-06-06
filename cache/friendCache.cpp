#include "friendCache.h"


std::vector<int> FriendCache::getFriendList(int userId)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mFriendCache.count(userId))
        return mFriendCache[userId];
    return {};
}

void FriendCache::addFriend(int userId, int friendId)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFriendCache[userId].push_back(friendId);
}

void FriendCache::updateFriendList()
{

}

void FriendCache::eraseFriendList(int userId)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFriendCache.erase(userId);
}

FriendCache * FriendCache::GetInstance()
{
    static FriendCache cache;
    return &cache;
}
