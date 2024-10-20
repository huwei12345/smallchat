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

bool PersonCache::addPerson(UserInfo info)
{
    mPersonCacheMap[info.user_id] = info;
    return true;
}

bool PersonCache::addPerson(UserInfo info, QIcon photo)
{
    mPersonCacheMap[info.user_id] = info;
    mPersonCacheMap[info.user_id].photo = photo;
    return true;
}

bool PersonCache::setPersonPhoto(int userId, QIcon icon)
{
    mPersonCacheMap[userId];
    mPersonCacheMap[userId].user_id = userId;
    mPersonCacheMap[userId].photo = icon;
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
