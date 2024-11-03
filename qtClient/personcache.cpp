#include "personcache.h"
#include <QDebug>
PersonCache* PersonCache::info = nullptr;

PersonCache *PersonCache::GetInstance()
{
    if (info == nullptr) {
        info = new PersonCache;
    }
    return info;
}

bool PersonCache::addPerson(UserInfo info, bool isFriend)
{
    mPersonCacheMap[info.user_id] = info;
    mPersonCacheMap[info.user_id].isFriend = isFriend;
    return true;
}

bool PersonCache::addPerson(UserInfo info, QIcon photo)
{
    mPersonCacheMap[info.user_id] = info;
    mPersonCacheMap[info.user_id].photo = photo;
    return true;
}

bool PersonCache::setPersonPhoto(int userId, QIcon icon, QString source)
{
    mPersonCacheMap[userId];
    mPersonCacheMap[userId].user_id = userId;
    mPersonCacheMap[userId].photo = icon;
    mPersonCacheMap[userId].source = source;
    return true;
}

bool PersonCache::erasePerson(int userId)
{
    if (mPersonCacheMap.count(userId)) {
        mPersonCacheMap.erase(userId);
    }
    return true;
}

UserInfo *PersonCache::getPersonInfo(int userId)
{
    if (mPersonCacheMap.count(userId)) {
        return (UserInfo*)(&mPersonCacheMap[userId]);
    }
    return nullptr;
}

QIcon *PersonCache::getPersonPhoto(int userId)
{
    if (mPersonCacheMap.count(userId)) {
        return &mPersonCacheMap[userId].photo;
    }
    return nullptr;
}

QString PersonCache::getPersonPhotoPath(int userId)
{
    if (mPersonCacheMap.count(userId)) {
        return mPersonCacheMap[userId].source;
    }
    return "";
}

bool PersonCache::count(int userId)
{
    return mPersonCacheMap.count(userId);
}

bool PersonCache::isFriend(int userId) {
    if (!mPersonCacheMap.count(userId))
        return false;
    return mPersonCacheMap[userId].isFriend;
}

UserInfoDetail::UserInfoDetail(const UserInfo &info) : UserInfo(info)
{
}


UserInfoDetail& UserInfoDetail::operator=(const UserInfo &detail)
{
    if (this != &detail) {
        UserInfo::operator=(detail); // 拷贝父类部分
    }
    return *this;
}

