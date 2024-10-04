#include "clientpersoninfo.h"

ClientPersonInfo* ClientPersonInfo::info = NULL;

ClientPersonInfo *ClientPersonInfo::GetInstance()
{
    if (info == NULL) {
        info = new ClientPersonInfo;
    }
    return info;
}

void ClientPersonInfo::init(UserInfo info)
{
    user_id = info.user_id;
    username = info.username;
    email = info.email;
    full_name = info.full_name;
    avatar_url = info.avatar_url;
    bio = info.bio;
    sex = info.sex;
    age = info.age;
    address = info.address;
    friendStatus = info.friendStatus;
}

ClientPersonInfo::ClientPersonInfo(UserInfo info)
{
    user_id = info.user_id;
    username = info.username;
    email = info.email;
    full_name = info.full_name;
    avatar_url = info.avatar_url;
    bio = info.bio;
    sex = info.sex;
    age = info.age;
    address = info.address;
    friendStatus = info.friendStatus;
}
