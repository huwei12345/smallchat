#ifndef CLIENTPERSONINFO_H
#define CLIENTPERSONINFO_H
#include "Protocol.h"
#include <string>
class ClientPersonInfo
{
public:
    static ClientPersonInfo* GetInstance();
    void init(UserInfo info);
    int user_id;
    std::string username;
    std::string email;
    std::string full_name;
    std::string avatar_url;
    std::string bio;
    bool sex;
    int age;
    std::string address;
    std::string friendStatus;
    std::string avatar_localPath;

private:
    static ClientPersonInfo* info;
    ClientPersonInfo() : user_id(0), username(""), email(""), full_name(""), avatar_url(""),
      bio(""), sex(false), age(0), address("") , friendStatus("") { }
    ClientPersonInfo(int id, std::string uname, std::string em, std::string fn,
      std::string au, std::string b, bool s, int a, std::string add, std::string friendSta)
      : user_id(id), username(uname), email(em), full_name(fn),
        avatar_url(au), bio(b), sex(s), age(a), address(add) , friendStatus(friendSta){ }
    ClientPersonInfo(UserInfo info);
};

#endif // CLIENTPERSONINFO_H
