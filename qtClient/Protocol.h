#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<iostream>
#include<string>
#include<vector>
#include"MyProtocolStream.h"
using namespace std;
using namespace net;

//二进制打印所有的值
inline void print(string& s) {
    for (int i = 0; i < (int)s.size(); i++) {
        std::cout << std::hex << (unsigned int)s[i] << " ";
    }
    std::cout << std::dec << std::endl;
}

namespace FunctionCode {
    enum FunctionCode {
        TEST                                  = 0,
        Register                              = 1,
        Login                                 = 2,
        SearchAllFriend                       = 3,
        FindFriend                            = 4,
        AddFriend                             = 5,
        SendMessage                           = 6,
        GetAllFriendRequest                   = 7,
        GetAllMessage                         = 8,
        UpdateUserState                       = 9,
        ProcessFriendRequest                  = 10,
        ProcessMessageRead                    = 11,
        //似乎会有服务器到客户端的广播，如消息传递、登录状态时的好友请求 朋友状态更新，需要监听
    };

    const std::vector<std::string> FunctionCodeString = {
        "TEST                "  ,
        "Register            "  ,
        "Login               "  ,
        "SearchAllFriend     "  ,
        "FindFriend          "  ,
        "AddFriend           "  ,
        "SendMessage         "  ,
        "GetAllFriendRequest "  ,
        "GetAllMessage       "  ,
        "UpdateUserState     "  ,
        "ProcessFriendRequest"  ,
        "ProcessMessageRead  "  ,
    };
};

class UserInfo {
public:
    UserInfo() : user_id(0), username(""), email(""), full_name(""), avatar_url(""),
      bio(""), sex(false), age(0), address("") , friendStatus("") { }
    UserInfo(int id, std::string uname, std::string em, std::string fn,
      std::string au, std::string b, bool s, int a, std::string add, std::string friendSta)
      : user_id(id), username(uname), email(em), full_name(fn),
        avatar_url(au), bio(b), sex(s), age(a), address(add) , friendStatus(friendSta){ }

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
    // 可以添加其他需要返回的用户信息字段
    // int status;//'online', 'offline', ''
    // int last_login;
    // int created_at;
    // int flag;
    // std::string now_address;
    // int useTime;
    std::string loginserial();
    std::string registerSerial();


    void print() {
        std::cout << "User ID: " << user_id << "\t"
       << "Username: " << username << "\t"
       << "Email: " << email << "\t"
       << "Full Name: " << full_name << "\t"
       << "Avatar URL: " << avatar_url << "\t"
       << "Bio: " << bio << "\t"
       << "Sex: " << (sex ? "Male" : "Female") << "\t"
       << "Age: " << age << "\t"
       << "Address: " << address << "\n";
    }
};

class MessageInfo {
public:
    MessageInfo() : id(0), sender_id(0), message_text(""),
        receiver_id(0), message_id(0) { }
    MessageInfo(int sendId, std::string message) : id(0), sender_id(sendId), message_text(message),
        receiver_id(0), message_id(0) { }
    int id;
    int sender_id;
    std::string message_text;
    int receiver_id;
    int message_id;
    // 可以添加其他需要返回的消息信息字段，如时间戳等
    inline void print() {
        std::cout << "sender_id: " << sender_id << "   "
                  << "message: " << message_text << "\n";
    }
};
class FriendRequest {
public:
    int reciver_id;
    int sender_id;
    bool state;
    std::string username;
    bool mAccept;
    //std::string message;添加朋友时写的信息
    //int time;什么时候添加的
};



struct friendHasher {
    std::size_t operator()(const FriendRequest& request) const {
        // 实现自定义的哈希算法，例如可以基于对象的某些成员变量计算哈希值
        // 返回计算得到的哈希值
        return std::hash<int>()(request.sender_id) + std::hash<int>()(request.reciver_id);
    }
};

struct friendEqual {
    bool operator()(const FriendRequest& lhs, const FriendRequest& rhs) const {
        // 实现自定义的相等比较逻辑，例如可以比较对象的所有成员变量
        return lhs.sender_id == rhs.sender_id && lhs.reciver_id == rhs.reciver_id;
    }
};




//size_t friendHasher(const FriendRequest& p) {//hash函数，得到hash码
//    return hash<int>()(p.user_id) + hash<string>()(p.username) + hash<int>()(p.state);
//}

//bool friendEqual(const FriendRequest& p1, const FriendRequest& p2) {
//    return p1.user_id == p2.user_id;
//}

// 普通函数作为哈希函数
//std::size_t friendHasher(const FriendRequest& request) {
//    // 实现自定义的哈希算法，例如基于对象的某些成员变量计算哈希值
//    return std::hash<int>()(request.user_id);
//}

//// 普通函数作为相等函数
//bool friendEqual(const FriendRequest& lhs, const FriendRequest& rhs) {
//    // 实现自定义的相等比较逻辑，例如比较对象的所有成员变量
//    return lhs.user_id == rhs.user_id;
//}

class FriendList : public std::vector<UserInfo> {
public:
    std::string friendListSerial() {
        std::string str;
        MyProtocolStream stream(str);
        stream << (int)size();
        for (auto it = begin(); it != end(); ++it) {
            stream << it->user_id << it->friendStatus << it->username << it->email;
        }
        return str;
    }
};

class Request {
public:
    Request()
        : mType(0), mFunctionCode(0), mFlag(0), mDirection(0),
          mTimeStamp(0), mData(""), mUserId(0), mSecret(0) {}
    // 构造函数
    Request(int type, int funcCode, int flag, int dir, int timeStamp,
            const std::string& data, int userId = 0, int secret = 0)
        : mType(type), mFunctionCode(funcCode), mFlag(flag), mDirection(dir),
          mTimeStamp(timeStamp), mData(data), mUserId(userId), mSecret(secret) {}
    int mType;
    int mFunctionCode;
    int mFlag;
    int mDirection;
    int mTimeStamp;//时间戳和任务编号
    std::string mData;
    int mUserId;
    int mSecret;

    std::string serial() {
        std::string ans;
        MyProtocolStream stream(ans, PROTOCOL_PACK);
        stream << mType << mFunctionCode << mFlag <<mDirection
          << mTimeStamp << mUserId << mSecret << mData << endll;
//        for (int i = 0; i < (int)ans.size(); i++) {
//            printf("%d ", ans[i]);
//        }
        return ans;
    }

    void deserial(std::string req) {
        MyProtocolStream stream(req, PROTOCOL_PACK);
        stream >> mType >> mFunctionCode >> mFlag
          >> mDirection >> mTimeStamp >> mUserId >> mSecret >> mData;
    }

    // 成员函数，用于打印 Request 对象的内容到一行
    void print() const {
        std::cout << "[Request] "
                  << "Type: " << mType << ", "
                  << "Function Code: " << FunctionCode::FunctionCodeString[mFunctionCode] << ", "
                  << "Flag: " << mFlag << ", "
                  << "Direction: " << mDirection << ", "
                  << "TimeStamp: " << mTimeStamp << ", "
                  << "mUserId: " << mUserId << ", "
                  << "mSecret: " << mSecret << ", "
                  << "Data: " << mData << std::endl;
    }
};

class Response {
public:
    int mType;
    int mFunctionCode;
    int mFlag;
    int mDirection;
    int mTimeStamp;//任务编号
    int mCode;//返回结果码值
    int mUserId;
    int mSecret;
    bool mhasData;
    std::string mData;

    // 无参构造函数
    Response()
        : mType(0), mFunctionCode(0), mFlag(0), mDirection(0),
          mTimeStamp(0), mCode(0), mUserId(0), mSecret(0), mhasData(false), mData("") {}

    // 有参构造函数
    Response(int type, int funcCode, int flag, int dir, int timeStamp,
             int code, int userId, int secret, bool hasData, const std::string& data)
        : mType(type), mFunctionCode(funcCode), mFlag(flag), mDirection(dir),
          mTimeStamp(timeStamp), mCode(code), mUserId(userId), mSecret(secret), mhasData(hasData), mData(data) {}

    // 成员函数，用于打印 Response 对象的内容到一行
    void print() const {
        std::cout << "[Response] "
                  << "Type: " << mType << ", "
                  << "Function Code: " << FunctionCode::FunctionCodeString[mFunctionCode] << ", "
                  << "Flag: " << mFlag << ", "
                  << "Direction: " << mDirection << ", "
                  << "TimeStamp: " << mTimeStamp << ", "
                  << "Code: " << (mCode ? "[True]" : "[False]") << ", "
                  << "mUserId: " << mUserId << ", "
                  << "mSecret: " << mSecret << ", "
                  << "Has Data: " << std::boolalpha << mhasData << ", "
                  << "Data: " << mData << std::endl;
    }
    void init(int code, int type, int functioncode, int flag, int direction, int timeStamp, int userId, int secret = 0) {
        mCode = code;
        mType = type;
        mFunctionCode = functioncode;
        mFlag = flag;
        mDirection = direction;
        mTimeStamp = timeStamp;
        mUserId = userId;
        mSecret = secret;
    }

    std::string serial() {
        std::string ans;
        MyProtocolStream stream(ans, PROTOCOL_PACK);
        stream << mType << mFunctionCode << mFlag << mDirection
          << mTimeStamp << mCode << mUserId << mSecret << mhasData;
        if (mhasData)
            stream << mData;
        stream << endll;
        return ans;
    }

    void deserial(std::string req) {
        MyProtocolStream stream(req, PROTOCOL_PACK);
        stream >> mType >> mFunctionCode >> mFlag
        >> mDirection >> mTimeStamp >> mCode >> mUserId >> mSecret >> mhasData;
        if (mhasData)
            stream >> mData;
    }
};


#endif
