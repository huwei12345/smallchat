#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<iostream>
#include<string>
#include<vector>
#include"MyProtocolStream.h"
#include "soft.h"

#ifndef SERVER
#include <QMutex>
#endif
using namespace std;
using namespace net;
class Session;
class Connection {
public:
    Connection() : clientSocket(0), session(NULL) { }
    Connection(int socket) : clientSocket(socket), session(NULL) { }
    int clientSocket;
    char buffer[4096];
    bool readRequest(std::string &requestData);
    // CallBack processRead; //CallBack
    bool processRead();
    bool closeConnection(int flag = 0);
    Session* session;
};

enum SessionState {
    ONLINE,
    Invisible,
    OFFLINE
};

class Session {
public:
    Session() : mLoginState(SessionState::ONLINE) {}
    Connection* mConn;
    int mUserId;
    int mLoginState;
    //应该还需要保持心跳连接
};

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
        ReciveMessage                         = 12,
        CreateGroup                           = 13,
        JoinGroup                             = 14,
        ResponseJoinGroup                     = 15,
        StoreFile                             = 16,
        TransFile                             = 17,
        FindGroup                             = 18,

        //上送文件，头像、图片、文件等
        StartUpLoadFile                       = 19,//Request : 通知服务器要发送文件，告知文件参数 Response: 告知允许上传，及部分参数
        UpLoadFileSuccess                     = 20,//Request: 告知服务器文件已通过FTP传输完毕。 Response:验证文件后，告知是否上传成功

        //主动获取
        GetFile                               = 21,//主动获取，告知服务器要获取的文件， Response:验证文件是否存在和大小等参数
        GetFileSuccess                        = 22,
        GetFileThird                          = 23,
        //GetFileSecond                         = 15,//根据文件大小决定是否有这一步（10M以内不用）， 告知服务器是否将要获取，更改本地状态为正在接收。
        //GetFileThird                          = 16,//服务器发给客户端的通知性相应，告知文件发送完成。或者文件将取消发送。Response:客户端响应给服务器已经接收完毕或者其他状态。


        //被动接收
        NofifyFileComing                      = 24,//服务器发个客户端告知有文件将传来，Response: 客户端相应服务器，是否接收，若准备接受，切换为接收状态
        AgreeRecvFile                         = 25,
        //现在改为本地FTP Get后似乎不用第二步了，直接复用22响应就可以了。或者以后可以再加一步，作为ftp的进一步协商
        TransFileOver                         = 26,//服务器告知客户端文件已传输完毕，Response: 客户端相应服务器接收情况。服务器必须收到接受情况后才完成此次任务，否则将文件传输状态回滚。

        GetAllOfflineFile                         = 27,
        GetOfflineFile                            = 28,
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
        "ReciveMessage"         ,

        "CreateGroup         "  ,
        "JoinGroup           "  ,
        "ResponseJoinGroup   "  ,
        "StoreFile           "  ,
        "TransFile           "  ,
        "FindGroup           "  ,

//上送文件，头像、图片、文件等
        "StartUpLoadFile     ",
        "UpLoadFileSuccess   ",
        "GetFile             ",
        "GetFileSuccess      ",
        "GetFileThird        ",
        "NofifyFileComing    ",
        "AgreeRecvFile       ",
        "TransFileOver       ",
        "GetAllOfflineFile   ",
        "GetOfflineFile      ",
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
    int status;//'online', 'offline', ''
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
    std::string timestamp;
    // 可以添加其他需要返回的消息信息字段，如时间戳等
    inline void print() {
        std::cout << "sender_id: " << sender_id << "   "
                  << "message: " << message_text << "\n";
    }
};

enum FileServerType {
    TOUXIANG,
    SENDTOPERSON,
    SENDTOGROUP,
    STOREFILE,
    STORETIMEFILE,
    ARRIVEFROMPOG//从他人或者群来的文件
};

class FileInfo {
public:
    FileInfo() : id(0), ftpTaskId(0) {}
    int id;
    int ftpTaskId;
    int send_id;
    int recv_id;
    int owner;
    string serverPath;
    string serverFileName;
    string ClientPath;
    string fileType;
    int serviceType;
    int filesize;
    int fileMode;
    long long md5sum;
    std::string timestamp;
#ifndef SERVER
    static int GenerateId;
    static QMutex genMutex;
    void Generate() {
        QMutexLocker locker(&FileInfo::genMutex);
        ftpTaskId = ++FileInfo::GenerateId;
        std::cout << "Gen FtpId : " << ftpTaskId << std::endl;
    }
#endif
    void print() const {
        std::cout << "id: " << id
                  << "ftpTaskid: " << ftpTaskId
                  << "\tsend_id: " << send_id
                  << "\trecv_id: " << recv_id
                  << "\tserverPath: " << serverPath
                  << "\tserverFileName: " << serverFileName
                  << "\tClientPath: " << ClientPath
                  << "\tfileType: " << fileType
                  << "\tserviceType: " << serviceType
                  << "\tfilesize: " << filesize
                  << "\tfileMode: " << fileMode
                  << "\tmd5sum: " << md5sum
                  << "\ttimestamp: " << timestamp
                  << std::endl;
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

class GroupInfo {
public:
    int id;
    int admin_id;
    std::string gtype;
    std::string group_name;
    std::string description;
    std::string tips;
};


class FileObject {
public:
    int user_id;
    int parent_id;
    std::string item_name;
    int item_type;
    std::string file_path;
};

class TransObject {
public:
    int sender_id;
    int receiver_id;
    std::string file_path;
    std::string message;
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

// 普通函数作为相等函数
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
            stream << it->user_id << it->friendStatus << it->username << it->email << it->avatar_url << it->status;
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
    bool returnFlag;
    // 无参构造函数
    Response()
        : mType(0), mFunctionCode(0), mFlag(0), mDirection(0),
          mTimeStamp(0), mCode(0), mUserId(0), mSecret(0), mhasData(false), mData(""), returnFlag(true) {}

    // 有参构造函数
    Response(int type, int funcCode, int flag, int dir, int timeStamp,
             int code, int userId, int secret, bool hasData, const std::string& data)
        : mType(type), mFunctionCode(funcCode), mFlag(flag), mDirection(dir),
          mTimeStamp(timeStamp), mCode(code), mUserId(userId), mSecret(secret), mhasData(hasData), mData(data), returnFlag(true) {}

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
