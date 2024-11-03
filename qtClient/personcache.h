#ifndef PERSONCACHE_H
#define PERSONCACHE_H
#include "Protocol.h"

#include <QIcon>
#include <map>
#include <unordered_map>
class UserInfoDetail : public UserInfo {
public:
    UserInfoDetail() : UserInfo(), isFriend(false) { }
    UserInfoDetail(int id, std::string uname, std::string em, std::string fn,
      std::string au, std::string b, bool s, int a, std::string add, std::string friendSta)
      : UserInfo(id, uname, em, fn, au, b, s, a, add, friendSta), isFriend(false) { }

    UserInfoDetail(const UserInfoDetail& detail) = default;
    UserInfoDetail& operator=(const UserInfoDetail& detail) = default;

    UserInfoDetail(const UserInfo& info);
    UserInfoDetail& operator=(const UserInfo& detail);

    QIcon photo;
    QString source;
    bool isFriend;
};

class PersonCache
{
public:
    static PersonCache* GetInstance();
    bool addPerson(UserInfo info, bool isFriend = false);
    bool addPerson(UserInfo info, QIcon photo);
    bool erasePerson(int userId);
    UserInfo* getPersonInfo(int userId);
    QIcon* getPersonPhoto(int userId);
    //不暴露UserInfoDetail给外部
    bool setPersonPhoto(int userId, QIcon icon, QString source);
    QString getPersonPhotoPath(int userId);
    bool count(int userId);
    bool isFriend(int userId);
private:
    PersonCache() = default;
    std::unordered_map<int, UserInfoDetail> mPersonCacheMap;
    static PersonCache* info;
};

#endif // PERSONCACHE_H
