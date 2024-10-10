#include "RequestProcessor.h"
#include <regex>
#include "Protocol.h"
#include "MyProtocolStream.h"
#include "MysqlPool.h"
#include "server.h"
#include "settime.h"
#include "./cache/friendCache.h"


int stoiAll(const std::string &str) {
    int number = 0; 
    if (str.size() > 15) {
        return -1;
    }
    for (int i = 0; i < str.size(); i++) {
        if (str[i] > '9' || str[i] < '0') {
            return -1;
        }
        number = number * 10 + (str[i] - '0'); 
    }
    return number;
}

void RequestProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    response.mType = request.mType;
    response.mDirection = !request.mDirection;
    response.mFunctionCode = request.mFunctionCode;
    response.mCode = 200;
    response.mhasData = 1;
    response.mData = "hello world!";
    response.mTimeStamp = 1111;
    static int count = 0;
    printf("count = %d\n", count);
    count++;
}


bool LoginProcessor::Login(const std::string& username, const std::string& password, UserInfo& userInfo) {
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //查询
	sql::PreparedStatement* state2 = conn->prepareStatement("select * from users where username = ? and password_hash = ?;");
	state2->setString(1, username);
    state2->setString(2, password);
    sql::ResultSet *st = state2->executeQuery();
    try {	
        while (st->next()) {
            userInfo.user_id = st->getInt("user_id");
            userInfo.username = st->getString("username");
            userInfo.email = st->getString("email");
            userInfo.avatar_url = st->getString("avatar_url");
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
    }
    int row = st->rowsCount();
    st->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    if (row == 0 || userInfo.user_id == 0) {
        return false;
    }
    return true;
}

void LoginProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    std::string& data = request.mData;
    MyProtocolStream stream(data);
    string account, password;
    stream >> account >> password;
    UserInfo info;
    bool ret = Login(account, password, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        response.mData = info.loginserial();
        //conn->mUserId = info.user_id;
        //conn->mLoginState = ;
        Session* session = new Session;
        session->mConn = conn;
        conn->session = session;
        //互相指可能有问题
        session->mUserId = info.user_id;
        Server::GetInstance()->mUserSessionMap[info.user_id] = session;
        //session->mLoginState = info.status; 登陆状态
    }
    else {
        //some error info add
    }
}

int RegisterProcessor::Register(const Request& request, UserInfo& info) {
    string s = request.mData;
    MyProtocolStream stream(s);
    string password;
    stream >> info.username >> password >> info.email >> info.full_name >>
	  info.avatar_url >> info.bio >> info.sex >> info.age >> info.address;

    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }

    //查询      values('lixin','123456','lixin.com','li','xxx.com','111',1,0,'shanxi');)");
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(insert into users
        (username, password_hash, email, full_name, avatar_url, bio,sex,age,address) 
        values(?,?,?,?,?,?,?,?,?);)");
    state2->setString(1, info.username);
    state2->setString(2, password);
    state2->setString(3, info.email);
    state2->setString(4, info.full_name);
    state2->setString(5, info.avatar_url);
    state2->setString(6, info.bio);
    state2->setBoolean(7, info.sex);
    state2->setInt(8, info.age);
    state2->setString(9, info.address);
    try {
        state2->execute();
    }
    catch(sql::SQLException &e) {
        if (e.getErrorCode() == 1062) {
            printf("user has been Register\n");
            info.print();
            cout << " Err Code: " << e.getErrorCode();
            cout << "# ERR " << e.what();
            cout << " SQLState: " << e.getSQLState() << std::endl;
        }
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return 2;
    }

    state2->close();
    sql::PreparedStatement* state3 = conn->prepareStatement("SELECT LAST_INSERT_ID();");
    sql::ResultSet* rs = state3->executeQuery();
    int autoIncKeyFromFunc = -1;
    if (rs->next()) {
        autoIncKeyFromFunc = rs->getInt(1);
    } else {
        // throw an exception from here
    }
    rs->close();
    state3->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);

    info.user_id = autoIncKeyFromFunc;
    info.print();
    if (1) {
        return 1;
    }
    return 0;
}

void RegisterProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    UserInfo info;
    int ret = Register(request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode == 1) {
        response.mhasData = true;
        response.mData = info.registerSerial();
    }
    else {
        //some error info add
    }
}


void SearchAllFriendProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    FriendList friendList;
    bool ret = SearchAllFriend(request, friendList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        //绑定Cache中好友
        bindAllFriendState(conn, request, friendList);
        response.mData = friendList.friendListSerial();
        //通知好友自己上线
        ProcessNotifyStateProcessor processor;
        processor.Notify(conn, friendList, request.mUserId, ONLINE);
    }
    else {
        //some error info add
    }
}


void FindFriendProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    FriendList friendList;
    bool ret = FindFriend(request, friendList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        response.mData = friendList.friendListSerial();
    }
    else {
        //some error info add
    }
}

bool FindFriendProcessor::FindFriend(const Request &request, FriendList &friendList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    UserInfo info;
    int type = 0;//0 按名字，1 按id
    stream >> info.username;
    if (info.username[0] >= '0' && info.username[0] <= '9') {
        type = 1;
        info.user_id = stoiAll(info.username);
        if (info.user_id <= 0) {
            return false;
        }
    }
    else {
        type = 0;
        stream >> info.username;
    }
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = NULL;
    //查询
    if (type == 1) {
        state2 = conn->prepareStatement("select * from users where user_id = ?;");
        state2->setInt(1, info.user_id);
    }
    else {
	    state2 = conn->prepareStatement("select * from users where username = ?;");
	    state2->setString(1, info.username);
    }
    sql::ResultSet *st = state2->executeQuery();
    int id = -1;
    try {
        while (st->next()) {
            info.user_id = st->getInt("user_id");
            info.username = st->getString("username");
            info.email = st->getString("email");
            cout << "id:" << info.user_id << " name:" << info.username << " email:" << info.email << std::endl;
            friendList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    MysqlPool::GetInstance()->releaseConncetion(conn);
    if (friendList.size() > 0)
        return true;
    return false;
}

void FindGroupProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    vector<GroupInfo> groupList;
    bool ret = FindGroup(request, groupList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        MyProtocolStream stream(response.mData);
        stream << (int)groupList.size();
        for (auto &info : groupList) {
            stream << info.id << info.group_name << info.role << info.admin_id << info.gtype << info.description << info.tips << info.timestamp;
            info.print();
        }
    }
    else {
        //some error info add
    }
}

bool FindGroupProcessor::FindGroup(const Request &request, vector<GroupInfo> &groupList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    GroupInfo info;
    int type = 0;//0 按名字，1 按id
    stream >> info.group_name;
    printf("group.name = %s\n", info.group_name.c_str());
    if (info.group_name[0] >= '0' && info.group_name[0] <= '9') {
        type = 1;
        info.id = stoiAll(info.group_name);
        printf("info.id = %d\n", info.id);
        if (info.id <= 0) {
            return false;
        }
    }
    else {
        type = 0;
        printf("group.name = %s\n", info.group_name.c_str());
        stream >> info.group_name;
    }
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = NULL;
    //查询
    if (type == 1) {
        state2 = conn->prepareStatement("select * from group_t where group_id = ?;");
        state2->setInt(1, info.id);
    }
    else {
	    state2 = conn->prepareStatement("select * from group_t where group_name = ?;");
	    state2->setString(1, info.group_name);
    }
    sql::ResultSet *st = state2->executeQuery();
    int id = -1;
    try {
        while (st->next()) {
            info.id = st->getInt("group_id");
            info.role = "other";
            info.group_name = st->getString("group_name");
            info.admin_id = st->getInt("admin_id");
            info.gtype = st->getString("gtype");
            info.description = st->getString("description");
            info.tips = st->getString("Tips");
            info.timestamp = st->getString("created_at");
            //cout << "id:" << info.id << " name:" << info.role << " email:" << info.group_name << "status:" << info.admin_id << std::endl;
            groupList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    MysqlPool::GetInstance()->releaseConncetion(conn);
    if (groupList.size() > 0)
        return true;
    return false;
}

void AddFriendProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    UserInfo info;
    bool ret = AddFriend(request);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        
    }
    else {
        //some error info add
    }
}

bool AddFriendProcessor::AddFriend(const Request &request)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    UserInfo info;
    stream >> info.user_id;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        //记录一下错误
        return false;
    }
    //查询
    //SQL 注入：确保对 request.mUserId 和 info.user_id 进行适当的验证或清理，以防止 SQL 注入攻击。
	sql::PreparedStatement* state = conn->prepareStatement("select * from users where user_id = ?;");
	state->setInt(1, request.mUserId);
    sql::ResultSet *st = state->executeQuery();
    //资源管理：考虑使用 RAII（资源获取即初始化）来管理资源，如数据库连接 (conn)、预编译语句 (state, state2) 和结果集 (st)。这有助于自动资源清理，避免资源泄漏。
    if (st->rowsCount() <= 0) {
        printf("Row %d\n", (int)st->rowsCount());
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    //事务处理：根据应用程序需求，考虑将数据库操作包装在事务中 (conn->setAutoCommit(false))，以确保操作的原子性（全部成功或全部失败）。
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(insert into friendships
        (user1_id, user2_id, status) 
        values(?,?,?);)");
    state2->setInt(1, request.mUserId);
    state2->setInt(2, info.user_id);
    state2->setInt(3, 1);
    
    try {
        state2->execute();
    }
    catch(sql::SQLException& e) {
        //日志和调试：特别是在错误情况下（catch 块），添加更详细的日志记录，有助于调试和监控系统行为。
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    st->close();
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

bool SendMessageProcessor::sendMessageByNet(Connection* conn, MessageInfo message) {
    std::string data;
    int clientSocket = conn->clientSocket; 
    MyProtocolStream stream(data);
    stream << message.id << message.sender_id << message.receiver_id << message.timestamp << message.message_text;
    Response rsp(1, FunctionCode::SendMessage, 3, 4, 5, 1, message.sender_id, 1, true, data);
    string str = rsp.serial();
    bool success = sendResponse(clientSocket, &rsp);
    if (success > 0) {
        return true;
    }
    return false;
}

void SendMessageProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    MessageInfo info;//返回值
    bool ret = false;
    ret = SendMessage(request, info);
    //在线时，直接通过网络发送消息给客户端，（接收消息和朋友请求的逻辑）
    if (Server::GetInstance()->mUserSessionMap.count(info.receiver_id)) {
        //从userId索引到Connection再得到clientSocket
        //思路1：在线，直接发送，入库read=0, 等消息确认相应，再修改read=1
        //思路2：直接入库read=0，不发送，等客户端进行心跳连接时，相应新消息和朋友请求
        //思路3：当在库中新增一项时，触发某任务，向客户发送消息，异步发送
        Session* session = Server::GetInstance()->mUserSessionMap[info.receiver_id];
        Connection* friendConn = session->mConn;
        if (friendConn != NULL/* && friendConn->connState != DisConnectionState*/) {
            bool ret = sendMessageByNet(friendConn, info/*info.receiver_id*/);
        }
    }
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        //TODO:发出去的消息，相应需要带ID吗？
        //向当前用户返回true
    }
    else {
        //some error info add
    }
}

bool SearchAllFriendProcessor::SearchAllFriend(const Request &request, FriendList &friendList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    UserInfo info;
    stream >> info.user_id;
    
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //TODO: 联合查询，用户的名字、email等返回
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(
        SELECT u.user_id AS friend_id, u.username AS friend_username, u.email, u.avatar_url
        FROM friendships f
        JOIN users u ON u.user_id = CASE
            WHEN f.user1_id = ? THEN f.user2_id
            WHEN f.user2_id = ? THEN f.user1_id
            ELSE NULL
        END
        WHERE f.status = ? AND (f.user1_id = ? OR f.user2_id = ?);
        )");
    state2->setInt(1, info.user_id);
    state2->setInt(2, info.user_id);
    state2->setInt(3, 2);
    state2->setInt(4, info.user_id);
    state2->setInt(5, info.user_id);
    sql::ResultSet *st = state2->executeQuery();

    int id = -1;
    try {
        while (st->next()) {
            info.user_id = st->getInt("friend_id");
            info.username = st->getString("friend_username");
            info.email = st->getString("email");
            info.friendStatus = "accept";
            info.avatar_url = st->getString("avatar_url");
            cout << "id:" << info.user_id << " name:" << info.username << " email:" << info.email << "status:" << info.friendStatus << std::endl;
            friendList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    MysqlPool::GetInstance()->releaseConncetion(conn);

    return true;
}

bool SearchAllFriendProcessor::bindAllFriendState(Connection *conn, Request &request, FriendList &friendList)
{
    for (auto &u : friendList) {
        FriendCache::GetInstance()->addFriend(request.mUserId, u.user_id);
        //大致获取其他好友状态，如果好友session在，就认为在线。在不在线其实不重要
        //就是个显示，在好友临界状态。 
        //刚上线，后续会收到好友的通知。 如果是通知已经发完了，那说明server端就已经有session了。不会错误 
        //刚下线，也会收到通知，如果是通知没有收到，session还没有取消
        //所以先删除session，再发通知下线
        //先创建session，再发通知上线
        
        //那种一会上线一会下线，上了又下，下了又上这种。总之它在服务器的队列里是一致的。
        //每隔2分钟，客户端发送一次同步请求好友列表。

        if (Server::GetInstance()->mUserSessionMap.count(u.user_id)) {
            u.status = ONLINE;
        }
    }
    return true;
}

void SearchAllGroupProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    vector<GroupInfo> groupList;
    bool ret = SearchAllGroup(request, groupList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        //绑定Cache中好友
        MyProtocolStream stream(response.mData);
        stream << (int)groupList.size();
        for (auto &info : groupList) {
            stream << info.id << info.group_name << info.role << info.admin_id << info.gtype << info.description << info.tips;
            info.print();
        }
    }
    else {
        //some error info add
    }
}


bool SearchAllGroupProcessor::SearchAllGroup(const Request& request, std::vector<GroupInfo>& groupList) {
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    GroupInfo info;
    int user_id = 0;
    stream >> user_id;
    
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //TODO: 联合查询，用户的名字、email等返回
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(
        SELECT g.group_id, gm.role, g.group_name, g.admin_id, g.gtype, g.description, g.Tips
        FROM group_members gm
        JOIN group_t g ON gm.user_id = ?
        WHERE gm.group_id = g.group_id and role <= 3;
        )");
    state2->setInt(1, user_id);
    sql::ResultSet *st = state2->executeQuery();

    int id = -1;
    try {
        while (st->next()) {
            info.id = st->getInt("group_id");
            info.role = st->getString("role");
            info.group_name = st->getString("group_name");
            info.admin_id = st->getInt("admin_id");
            info.gtype = st->getString("gtype");
            info.description = st->getString("description");
            info.tips = st->getString("Tips");
            //cout << "id:" << info.id << " name:" << info.role << " email:" << info.group_name << "status:" << info.admin_id << std::endl;
            groupList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    MysqlPool::GetInstance()->releaseConncetion(conn);

    return true;
}




bool SendMessageProcessor::SendMessage(const Request &request, MessageInfo& info)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    stream >> info.receiver_id >> info.message_text;
    info.sender_id = request.mUserId;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    info.timestamp = SetTime::GetInstance()->getAccurateTime();
    printf("info.time = %s\n", info.timestamp.c_str());
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(insert into messages
        (sender_id, recipient_id, content) 
        values(?,?,?);)");
    state2->setInt(1, request.mUserId);
    state2->setInt(2, info.receiver_id);
    state2->setString(3, info.message_text);
    state2->execute();

    state2->close();
    sql::PreparedStatement* state3 = conn->prepareStatement("SELECT LAST_INSERT_ID();");
    sql::ResultSet* rs = state3->executeQuery();
    int autoIncKeyFromFunc = -1;
    if (rs->next()) {
        autoIncKeyFromFunc = rs->getInt(1);
    } else {
        // throw an exception from here
    }
    info.id = autoIncKeyFromFunc;
    rs->close();
    state3->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    if (info.id <= 0)
        return false;
    return true;

}

std::string UserInfo::registerSerial() {
    std::string str;
    MyProtocolStream stream(str);
    stream << user_id << username << email << full_name << avatar_url << bio << sex << age << address;
    return str;
}


std::string UserInfo::loginserial() {
    std::string str;
    MyProtocolStream stream(str);
    stream << user_id << username << email << avatar_url;
    return str;
}

void GetAllMessageProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    std::vector<MessageInfo> message;
    bool ret = GetAllMessage(request, message);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << (int)message.size();
        for (int i = 0; i < message.size(); i++) {
            stream << message[i].id << message[i].sender_id << message[i].message_text << message[i].timestamp;
        }
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool GetAllMessageProcessor::GetAllMessage(const Request &request, vector<MessageInfo>& infoList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }

	sql::PreparedStatement* state2 = conn->prepareStatement(R"(select message_id, sender_id, recipient_id, content, timestamp from messages
        where is_read = ? and recipient_id = ?;)");
    state2->setInt(1, 0);
    state2->setInt(2, request.mUserId);
    sql::ResultSet *st = state2->executeQuery();
    int id = -1;
    try {
        while (st->next()) {
            MessageInfo info;
            info.id = st->getInt("message_id");
            info.sender_id = st->getInt("sender_id");
            info.message_text = st->getString("content");
            info.timestamp = st->getString("timestamp");
            cout << "sender_id:" << info.sender_id << " message:" << info.message_text << std::endl;
            infoList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    st->close();
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

void GetAllFriendReqProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    std::vector<FriendRequest> infoList;
    bool ret = GetAllFriendReq(request, infoList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << (int)infoList.size();
        for (int i = 0; i < infoList.size(); i++) {
            stream << infoList[i].sender_id;
            stream << infoList[i].username;
            stream << infoList[i].state;
        }
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool GetAllFriendReqProcessor::GetAllFriendReq(const Request &request, vector<FriendRequest> &infoList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(select f.user1_id, u.username, f.status, f.since
        from friendships f join users u 
        on f.status = ?
        and f.user2_id = ? 
        and f.user1_id = u.user_id;)");
    state2->setInt(1, 1);//pending
    state2->setInt(2, request.mUserId);
    sql::ResultSet *st = state2->executeQuery();

    try {
        while (st->next()) {
            FriendRequest info;
            info.sender_id = st->getInt("user1_id");
            info.reciver_id = request.mUserId;
            std::string state = st->getString("status");
            info.state = (state == "pending" ? false : true);
            info.username = st->getString("username");
            cout << "FriendRequest to " << info.reciver_id <<  " from " << info.sender_id << " username: " << info.username << std::endl;
            infoList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    st->close();
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

void UpdateUserStateProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    bool ret = UpdateUserState(request);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        //修改状态成功
        response.mhasData = true;
        response.mData = request.mData;
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}


bool UpdateUserStateProcessor::notifyStateToFriend(int userId, int state) {
    std::string data;
    vector<int> friendList;
    //friendList = getAllFriend();//从缓存读取，或者从数据库读取
    MyProtocolStream stream(data);
    stream << state;
    Response rsp(1, FunctionCode::SendMessage, 3, 4, 5, 1, userId, 1, true, data);
    string str = rsp.serial();
    for (int i = 0; i < friendList.size(); i++) {
        bool success = sendResponse(friendList[i], &rsp);
        if (success > 0) {
            
        }
    }
    //可能在用户初始获取朋友列表时，还要参考缓存中的朋友状态，或者从数据库读取
    //用户状态可能不需要，微信也没有，因为消息是不是实时不重要
    return true;
}

bool UpdateUserStateProcessor::UpdateUserState(const Request &request)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    int state = 0;
    stream >> state;
    //不用修改数据库了，默认上线，然后直接修改内存中的登录状态
    if (Server::GetInstance()->mUserSessionMap[request.mUserId]->mLoginState == state) {
        //不可能存在，在客户端避免
        return false;
    }
    Server::GetInstance()->mUserSessionMap[request.mUserId]->mLoginState = state;
    notifyStateToFriend(request.mUserId, state);
    //通知本人各个好友，通知这种行为可能需要进一步设计为异步任务，如上下线、消息、好友请求等。

    // sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    // if (conn == NULL) {
    //     return false;
    // }
    // //查询
	// sql::PreparedStatement* state2 = conn->prepareStatement("update users set status = ? where user_id = ?;");
    // state2->setInt(1, state);
	// state2->setInt(2, request.mUserId);
    // try {
    //     state2->executeUpdate();
    // }
    // catch(sql::SQLException& e) {
    //     cout << "# ERR " << e.what();
    //     cout << " Err Code: " << e.getErrorCode();
    //     cout << " SQLState: " << e.getSQLState() << std::endl;
    //     MysqlPool::GetInstance()->releaseConncetion(conn);
    //     return false;
    // }
    // MysqlPool::GetInstance()->releaseConncetion(conn);

    return true;
}

void ProcessFriendRequestProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    FriendRequest fr;
    bool ret = ProcessFriendRequest(request, fr);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << fr.sender_id << fr.reciver_id << fr.mAccept;
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}


bool ProcessFriendRequestProcessor::ProcessFriendRequest(Request & request, FriendRequest fr)
{
    //
    string mData = request.mData;
    MyProtocolStream stream(mData);
    stream >> fr.sender_id >> fr.reciver_id >> fr.mAccept;
    std::cout << fr.sender_id << "  " << fr.reciver_id << " " << fr.mAccept << std::endl;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //trans
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update friendships set status = ?
        where user1_id = ? and user2_id = ?;)");
    state2->setInt(1, fr.mAccept ? 2 : 3);
    state2->setInt(2, fr.sender_id);
    state2->setInt(3, fr.reciver_id);

    sql::PreparedStatement* state3 = conn->prepareStatement(R"(delete from friendships
        where user1_id = ? and user2_id = ?;)");
    state3->setInt(1, fr.reciver_id);
    state3->setInt(2, fr.sender_id);
    try {
        state2->executeUpdate();
        state3->executeUpdate();
    }
    catch(sql::SQLException& e) {
        //rollback
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        state2->close();
        state3->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    state2->close();
    state3->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

void ProcessMessageReadProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    ProcessMessageRead(request);
    response.returnFlag = false;
    //No Response
}

bool ProcessMessageReadProcessor::ProcessMessageRead(Request &request)
{
    //范围确认 start-end
    //后续可以优化为根据协议如何确认，如type = byte[0] type1:范围确认 type2:单条确认 type3:时间确认 type4:位图确认
    string& data = request.mData;
    MyProtocolStream stream(data);
    int send_id = 0, recv_id = 0;
    int start = 0, end = 0;
    stream >> send_id >> recv_id >> start >> end;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update messages set is_read = ? 
        where sender_id = ? and recipient_id = ? and message_id >= ? and message_id <= ? ;)");
    state2->setInt(1, 1);
    state2->setInt(2, send_id);
    state2->setInt(3, recv_id);
    state2->setInt(4, start);
    state2->setInt(5, end);
    try {
        state2->executeUpdate();
    }
    catch(sql::SQLException& e) {
        //rollback
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        state2->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);    
    return true;
}

void CreateGroupProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    GroupInfo info;
    MyProtocolStream stream(request.mData);
    stream >> info.group_name >> info.gtype >> info.admin_id >> info.description >> info.tips;
    bool ret = CreateGroup(request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        info.role = "owner";
        stream << info.id << info.group_name << info.role << info.admin_id << info.gtype << info.description << info.tips;
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

/*
    std::string group_name = "Sample Group"; // Replace with actual group name
    std::string description = "This is a sample group"; // Replace with actual description
    int admin_id = 1; // Replace with actual admin user ID
    int gtype = 0; // Replace with actual group type
    std::string tips = "Some tips for the group"; // Replace with actual tips

    if (CreateGroup(group_name, description, admin_id, gtype, tips)) {
        std::cout << "Group created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create group." << std::endl;
    }
    */

bool CreateGroupProcessor::CreateGroup(Request &request, GroupInfo& info)
{
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == nullptr) {
        std::cerr << "Failed to get database connection." << std::endl;
        return false;
    }
    // Prepare SQL statement to insert into group_t table
    sql::PreparedStatement* pstmt = conn->prepareStatement(R"(
        INSERT INTO group_t (group_name, description, admin_id, gtype, Tips)
        VALUES (?, ?, ?, ?, ?)
    )");
    pstmt->setString(1, info.group_name);
    pstmt->setString(2, info.description);
    pstmt->setInt(3, info.admin_id);
    pstmt->setString(4, info.gtype);
    pstmt->setString(5, info.tips);

    try {
        pstmt->execute();
        pstmt->close();
        sql::PreparedStatement* state3 = conn->prepareStatement("SELECT LAST_INSERT_ID();");
        sql::ResultSet* rs = state3->executeQuery();
        int autoIncKeyFromFunc = -1;
        if (rs->next()) {
            autoIncKeyFromFunc = rs->getInt(1);
        } else {
            // throw an exception from here
        }
        info.id = autoIncKeyFromFunc;
        printf("id = %d\n", info.id);
        rs->close();
        state3->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        if (info.id <= 0) {
            return false;
        }
        //作为拥有者
        JoinGroupProcessor proc;
        return proc.JoinGroup(info.admin_id, info.id, 3);
    } catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    return true;
}

void JoinGroupProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    MyProtocolStream stream(request.mData);
    int groupId = 1;
    stream >> groupId;
    bool ret = JoinGroup(request.mUserId, groupId, 4);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        //修改状态成功
    }
    else {
        response.mhasData = false;
    }
}

bool JoinGroupProcessor::JoinGroup(int userId, int groupId, int role)
{
    printf("Join Group %d\n", groupId);
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == nullptr) {
        std::cerr << "Failed to get database connection." << std::endl;
        return false;
    }
    //TODO:申请态
    // Prepare SQL statement to insert into group_members table
    sql::PreparedStatement* pstmt = conn->prepareStatement(R"(
        INSERT INTO group_members (group_id, user_id, role)
        VALUES (?, ?, ?)
    )");
    pstmt->setInt(1, groupId);
    pstmt->setInt(2, userId);
    pstmt->setInt(3, role);

    try {
        pstmt->execute();
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    return true;
}

//相应加群
void ResponseJoinGroupProcessor::Exec(Connection *conn, Request &request, Response &)
{
    
}
bool ResponseJoinGroupProcessor::ResponseJoinGroup(Request &request)
{
    
    return false;
}


void StoreFileProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    FileObject fileObject;
    bool ret = StoreFile(request, fileObject);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {

    }
    else {
        response.mhasData = false;
        //some error info add
    }
}


//bool AddUserStorageItem(int user_id, const std::string& item_name, const std::string& item_type, const std::string& file_path, int parent_id = 0) {
//可以在注册用户后，为其添加一个根项，用于管理
bool StoreFileProcessor::StoreFile(Request &request, FileObject& fileObject)
{
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == nullptr) {
        std::cerr << "Failed to get database connection." << std::endl;
        return false;
    }

    // Prepare SQL statement to insert into user_storage table
    sql::PreparedStatement* pstmt = conn->prepareStatement(R"(
        INSERT INTO user_storage (user_id, parent_id, item_name, item_type, file_path)
        VALUES (?, ?, ?, ?, ?)
    )");
    pstmt->setInt(1, fileObject.user_id);
    pstmt->setInt(2, fileObject.parent_id);
    pstmt->setString(3, fileObject.item_name);
    pstmt->setInt(4, fileObject.item_type);
    pstmt->setString(5, fileObject.file_path);
    try {
        pstmt->execute();
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    return false;
}
/*
    int user_id = 1;  // Replace with actual user ID
    std::string item_name = "example_file.txt"; // Replace with actual item name
    std::string item_type = "file"; // Replace with actual item type
    std::string file_path = "/path/to/example_file.txt"; // Replace with actual file path
    int parent_id = 0; // Optional parent ID, replace with actual parent ID if needed

    if (AddUserStorageItem(user_id, item_name, item_type, file_path, parent_id)) {
        std::cout << "User storage item added successfully." << std::endl;
    } else {
        std::cerr << "Failed to add user storage item." << std::endl;
    }
*/


void TransFileProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    TransObject fileObject;
    bool ret = TransFile(request, fileObject);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool TransFileProcessor::TransFile(Request &request, TransObject& object)
{
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == nullptr) {
        std::cerr << "Failed to get database connection." << std::endl;
        return false;
    }

    // Prepare SQL statement to insert into offline_transfer table
    sql::PreparedStatement* pstmt = conn->prepareStatement(R"(
        INSERT INTO offline_transfer (sender_id, receiver_id, file_path, message, created_at)
        VALUES (?, ?, ?, ?, NOW())
    )");
    pstmt->setInt(1, object.sender_id);
    pstmt->setInt(2, object.receiver_id);
    pstmt->setString(3, object.file_path);
    pstmt->setString(4, object.message);

    try {
        pstmt->execute();
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    return false;
}
/*
  int sender_id = 1;  // Replace with actual sender ID
    int receiver_id = 2; // Replace with actual receiver ID
    std::string file_path = "/path/to/offline_file.txt"; // Replace with actual file path
    std::string message = "Hello, here is an offline message."; // Replace with actual message

    if (OfflineTransfer(sender_id, receiver_id, file_path, message)) {
        std::cout << "Offline transfer recorded successfully." << std::endl;
    } else {
        std::cerr << "Failed to record offline transfer." << std::endl;
    }
*/

bool checkDisk(FileInfo& info) {
    return true;
}

bool checkUserLimit(FileInfo& info ) {
    return true;
}

void ProcessStartUpLoadFileProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    FileInfo info;
    bool ret = ProcessStartUpLoadFile(request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        //或许名字和路径需要加工一下
        stream << info.ftpTaskId << info.id << info.serverPath << info.serverFileName;
    }
    else {
        //TODO究竟是由于服务器业务上不需要发送文件，还是服务器出现问题需要进一步通信信息说明
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        //或许名字和路径需要加工一下
        stream << info.ftpTaskId << info.id << info.serverPath << info.serverFileName;
        //some error info add
    }
}

bool ProcessStartUpLoadFileProcessor::ProcessStartUpLoadFile(Request &request, FileInfo& info)
{
    //Request : 通知服务器要发送文件，告知文件参数 Response: 告知允许上传，及部分参数

    string& data = request.mData;
    MyProtocolStream stream(data);
    stream >> info.ftpTaskId >> info.id >> info.serviceType >> info.send_id >> info.recv_id >> info.serverPath >> info.serverFileName >> info.fileType >> info.filesize >> info.fileMode >> info.md5sum;
    if (info.serviceType == FileServerType::TOUXIANG) {
    
        std::regex pattern(R"(^mr(1[0-9]|20|[1-9])\.jpg$)");
        // 使用regex_match进行完全匹配
        if (std::regex_match(info.serverFileName, pattern)) {
            //TODO:相当于同意替换默认头像，在SQL中直接修改users表头像路径
            std::cout << "Text matches the pattern." << std::endl;
            ProcessUpLoadFileSuccessProcessor pro;
            pro.UpDateUserPhotoSQL(request, info);
            return false;//不需要传输常规文件，返回false
        } else {
            info.serverPath = "userPhoto/";
            std::cout << "Text does not match the pattern." << std::endl;
        }
    }
    else {
        info.serverPath = to_string(info.send_id) + "/";
    }
    //可能需要创建目录，和检查
    bool ret = checkDisk(info);
    bool ret2 = checkUserLimit(info);

    return ret && ret2;
}
void ProcessUpLoadFileSuccessProcessor::Exec(Connection* conn, Request &request, Response &response)
{
    FileInfo info;
    bool ret = ProcessUpLoadFileSuccess(conn, request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
    }
    else {
        //some error info add
    }
}

bool checkmd5(FileInfo& info) {
    return true;
}

bool ProcessUpLoadFileSuccessProcessor::ProcessUpLoadFileSuccess(Connection* conn, Request &request, FileInfo& info)
{
    string& data = request.mData;
    MyProtocolStream stream(data);
    stream >> info.ftpTaskId >> info.id >> info.serviceType >> info.send_id >> info.recv_id >> info.serverPath >> info.serverFileName >> info.fileType >> info.filesize >> info.fileMode >> info.md5sum;
    bool ret = checkmd5(info);

    if (info.recv_id != 0) {
        if (ProcessUpLoadSQL(request, info) == true) {
            //通知要接收用户
            ProcessNofifyFileComingProcessor processor;
            Response response;
            if (processor.NofifyFileComing(conn, request, info)) {
                //在线而发送失败
            }
        }
    }
    if (info.serviceType == FileServerType::TOUXIANG) {
        return UpDateUserPhotoSQL(request, info);
    }
    return ret;
}

bool ProcessUpLoadFileSuccessProcessor::ProcessUpLoadSQL(Request &request, FileInfo &info)
{
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == nullptr) {
        std::cerr << "Failed to get database connection." << std::endl;
        return false;
    }
    std::cout << info.send_id << "   " << info.recv_id << "  " << info.serverPath << " + " << info.serverFileName << info.fileType << "  " << info.filesize;
    // Prepare SQL statement to insert into offline_transfer table
    info.fileType = "zip";
    info.filesize = 10;
    sql::PreparedStatement* pstmt = conn->prepareStatement(R"(
        INSERT INTO offline_transfers (sender_id, receiver_id, file_name, file_type, file_size)
        VALUES (?, ?, ?, ?, ?)
    )");
    pstmt->setInt(1, info.send_id);
    pstmt->setInt(2, info.recv_id);
    pstmt->setString(3, info.serverPath + info.serverFileName);
    pstmt->setString(4, info.fileType);
    pstmt->setInt(5, info.filesize);
    try {
        pstmt->execute();
        pstmt->close();
        sql::PreparedStatement* state3 = conn->prepareStatement("SELECT LAST_INSERT_ID();");
        sql::ResultSet* rs = state3->executeQuery();
        int autoIncKeyFromFunc = -1;
        if (rs->next()) {
            autoIncKeyFromFunc = rs->getInt(1);
        } else {
            // throw an exception from here
        }
        info.id = autoIncKeyFromFunc;
        printf("id = %d\n", info.id);
        rs->close();
        state3->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        if (info.id <= 0) {
            return false;
        }
        return true;
    } catch (sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQLState: " << e.getSQLState() << std::endl;
        pstmt->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    return false;
}

bool ProcessUpLoadFileSuccessProcessor::UpDateUserPhotoSQL(Request &request, FileInfo &info)
{
    //范围确认 start-end
    //后续可以优化为根据协议如何确认，如type = byte[0] type1:范围确认 type2:单条确认 type3:时间确认 type4:位图确认
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update users set avatar_url = ? 
        where user_id = ?;)");
    state2->setString(1, info.serverPath + info.serverFileName);
    state2->setInt(2, request.mUserId);
    try {
        state2->executeUpdate();
    }
    catch(sql::SQLException& e) {
        //rollback
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        state2->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);    
    return true;
}

void ProcessGetFileProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    FileInfo info;
    bool ret = ProcessGetFile(request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        //传给客户端使用ftp获取时的一些参数，可能包括密码
        stream << info.ftpTaskId << info.id << info.serverPath << info.serverFileName << info.filesize;
    }
    else {
        //some error info add
    }
}

bool ProcessGetFileProcessor::ProcessGetFile(Request &request, FileInfo &info)
{
    string& data = request.mData;
    MyProtocolStream stream(data);
    stream >> info.ftpTaskId >> info.id >> info.send_id >> info.recv_id >> info.owner >> info.serverPath >> info.serverFileName >> info.filesize;
    if (info.send_id > 0) {
        //来自用户   
    }
        /*
        ret = stat(info.filename);
        if (ret == true) {
            if (size < info.size) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    */
    return true;
}

void ProcessGetFileSuccessProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    FileInfo info;
    bool ret = ProcessGetFileSuccess(request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
    }
    else {
        //some error info add
    }
}

bool ProcessGetFileSuccessProcessor::ProcessGetFileSuccess(Request &request, FileInfo &info)
{
    string& data = request.mData;
    MyProtocolStream stream(data);
    stream >> info.ftpTaskId >> info.id >> info.send_id >> info.recv_id >> info.serverPath >> info.serverFileName >> info.md5sum;
    if (info.send_id > 0) {
        //来自用户
        if (setReadedBySQL(request, info)) {
            //设置read=1不成功
        }
    }
    bool ret = checkmd5(info);
    return ret;
}

bool ProcessGetFileSuccessProcessor::setReadedBySQL(Request &request, FileInfo &info)
{
    //范围确认 start-end
    //后续可以优化为根据协议如何确认，如type = byte[0] type1:范围确认 type2:单条确认 type3:时间确认 type4:位图确认
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update offline_transfers set downloaded = ? 
        where transfer_id = ? and sender_id = ? and receiver_id = ?;)");
    printf("[[[[read=1 %d:   %d  %d %s]]]]\n", info.id, info.send_id, info.recv_id, info.serverFileName.c_str());
    state2->setInt(1, 1);
    state2->setInt(2, info.id);
    state2->setInt(3, info.send_id);
    state2->setInt(4, info.recv_id);
    try {
        state2->executeUpdate();
    }
    catch(sql::SQLException& e) {
        //rollback
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        state2->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);    
    return true;
}

std::queue<FileInfo> getFileInfoList() {
    return {};
}


//但这里应该首先保证该文件已经传输完成，在传输完成的响应后，添加入库，然后发送通知，而不是直接发送，
//所以Notify其实就等于本地客户先UpLoadFile，根据标志位再notify其他用户get
//其实是发送文件，某人发送给另一个人
//如果send_id = -1 ，说明是系统发送的
//如果send_id = 0， 说明是群组发送的
//如果send_id > 0, 说明是人发送的
//先入库，然后如果在线，直接发送通知，等客户端收到数据，然后再改库

//这个Exec貌似不应该作为协议请求，而只是作为Response通知，而后客户端通过GetFile流程获取文件，判断GetFile里的recv_id是否是来自于人还是系统文件
void ProcessNofifyFileComingProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    //弃用
    bool ret = false;
    FileInfo info;
    NofifyFileComing(conn, request, info);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        //TODO:发出去的消息，相应需要带ID吗？
        //向当前用户返回true
    }
    else {
        //some error info add
    }
    //响应返回给本用户表示发送成功，通知给其他用户。如何区分？flag
    response.returnFlag = false;
}

bool ProcessNofifyFileComingProcessor::NofifyFileComing(Connection *conn, Request &request, FileInfo &info)
{
    bool ret = true;
    //在线时，直接通过网络发送文件给客户端，（接收消息和朋友请求的逻辑）
    if (Server::GetInstance()->mUserSessionMap.count(info.recv_id)) {
        Session* session = Server::GetInstance()->mUserSessionMap[info.recv_id];
        Connection* friendConn = session->mConn;
        if (friendConn != NULL/* && friendConn->connState != DisConnectionState*/) {
            ret = sendNotifyFileByNet(friendConn, info);
        }
    }
    return ret;
}

bool ProcessNofifyFileComingProcessor::sendNotifyFileByNet(Connection* conn, FileInfo info) {
    std::string data;
    int clientSocket = conn->clientSocket; 
    MyProtocolStream stream(data);
    stream << info.ftpTaskId << info.id << info.send_id << info.recv_id << info.serverPath << info.serverFileName << info.filesize << info.fileType;
    Response rsp(1, FunctionCode::NofifyFileComing, 3, 4, 5, 1, info.send_id, 1, true, data);
    string str = rsp.serial();
    bool success = sendResponse(clientSocket, &rsp);
    if (success > 0) {
        return true;
    }
    return false;
}

void ProcessTransFileOverProcessor::Exec(Connection *conn, Request &request, Response &)
{
}

bool ProcessTransFileOverProcessor::TransFileOver(Request &request, FileInfo &info)
{
    return false;
}

void ProcessGetAllOfflineFileProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    std::vector<FileInfo> infoList;
    bool ret = GetAllOfflineFile(request, infoList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << (int)infoList.size();
        for (int i = 0; i < infoList.size(); i++) {
            stream << infoList[i].ftpTaskId;
            stream << infoList[i].id;
            stream << infoList[i].send_id;
            stream << infoList[i].recv_id;
            stream << infoList[i].serviceType;
            stream << infoList[i].serverPath;
            stream << infoList[i].serverFileName;
            stream << infoList[i].timestamp;
            infoList[i].print();
        }
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool ProcessGetAllOfflineFileProcessor::GetAllOfflineFile(Request &request, vector<FileInfo> &infoList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }

	sql::PreparedStatement* state2 = conn->prepareStatement(R"(select transfer_id, sender_id, receiver_id, file_name, file_type, file_size, upload_time from offline_transfers
        where downloaded = ? and receiver_id = ?;)");
    state2->setInt(1, 2);
    state2->setInt(2, request.mUserId);
    sql::ResultSet *st = state2->executeQuery();
    int id = -1;
    try {
        while (st->next()) {
            FileInfo info;
            info.id = st->getInt("transfer_id");
            info.send_id = st->getInt("sender_id");
            info.recv_id = request.mUserId;
            info.serviceType = FileServerType::ARRIVEFROMPOG;
            //name和path需要分开吗
            info.serverFileName = st->getString("file_name");
            info.fileType = st->getString("file_type");
            info.filesize = st->getInt("file_size");
            info.timestamp = st->getString("file_type");
            infoList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    st->close();
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

void ProcessGetOfflineFileProcessor::Exec(Connection *conn, Request &request, Response &)
{
    
}

bool ProcessGetOfflineFileProcessor::GetOfflineFile(Request &request, FileInfo &info)
{
    return false;
}

void ProcessNotifyStateProcessor::Exec(Connection *conn, Request &request, Response &response)
{

}

bool ProcessNotifyStateProcessor::Notify(Connection * conn, FriendList & friendList, int mUserId, int state)
{
    bool success = true;
    for (auto &u : friendList) {
        if (Server::GetInstance()->mUserSessionMap.count(u.user_id)) {
            Session* session = Server::GetInstance()->mUserSessionMap[u.user_id];
            Connection* friendConn = session->mConn;
            if (friendConn != NULL/* && friendConn->connState != DisConnectionState*/) {
                int clientSocket = friendConn->clientSocket; 
                std::string data;
                MyProtocolStream stream(data);
                stream << mUserId << state;
                Response rsp(1, FunctionCode::UpdateUserState, 3, 4, 5, 1, mUserId, 1, true, data);
                rsp.mhasData = true;
                string str = rsp.serial();
                success &= sendResponse(clientSocket, &rsp);
            }
        }
    }
    return success;
}

bool ProcessNotifyStateProcessor::Notify(Connection *conn, vector<int> &friendList, int mUserId, int state)
{
    bool success = true;
    for (auto &u : friendList) {
        if (Server::GetInstance()->mUserSessionMap.count(u)) {
            Session* session = Server::GetInstance()->mUserSessionMap[u];
            Connection* friendConn = session->mConn;
            if (friendConn != NULL/* && friendConn->connState != DisConnectionState*/) {
                int clientSocket = friendConn->clientSocket; 
                std::string data;
                MyProtocolStream stream(data);
                stream << mUserId << state;
                Response rsp(1, FunctionCode::UpdateUserState, 3, 4, 5, 1, mUserId, 1, true, data);
                rsp.mhasData = true;
                string str = rsp.serial();
                success &= sendResponse(clientSocket, &rsp);
            }
        }
    }
    return success;
}

void GetAllGroupReqProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    std::vector<GroupJoinRequest> infoList;
    bool ret = GetAllGroupReq(request, infoList);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << (int)infoList.size();
        for (auto &info : infoList) {
            stream << info.user_id << info.username
                   << info.email << info.avatar_url << info.bio << info.sex
                   << info.age << info.address
                   << info.groupId << info.group_name << info.applyTimeStamp;
        }
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool GetAllGroupReqProcessor::GetAllGroupReq(const Request &request, vector<GroupJoinRequest> &infoList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //TODO:
	sql::PreparedStatement* state2 = conn->prepareStatement(R"(select u.user_id, u.username, u.email, g.group_id 
        from users u join group_members g 
        on u.user_id = g.user_id 
        where g.role = 4  
        and g.group_id in (select group_id from group_members where user_id = ? 
                           and (role = 2 or role = 3));
    )");
    state2->setInt(1, request.mUserId);
    sql::ResultSet *st = state2->executeQuery();
    //获得用户的个人信息，找到管理的群，找到这些群的申请者。

    // select u.user_id, u.username, u.email, g.group_id from users u join group_members g 
    // on u.user_id = group_members.user_id
    // where g.role = 4
    // g.group_id in
    // (select group_id from group_members where user_id = ? and (role = 2 or role = 3));
    
    
    try {
        while (st->next()) {
            GroupJoinRequest info;
            info.user_id = st->getInt("user_id");
            info.username = st->getString("username");
            info.email = st->getString("email");
            // info.avatar_url = st->getString("avatar_url");
            // info.bio = st->getString("bio");
            // info.sex = st->getInt("sex");
            // info.age = st->getInt("age");
            // info.address = st->getString("address");
            info.groupId = st->getInt("group_id");
            // info.group_name = st->getString("group_name");
            // info.applyTimeStamp = st->getString("joined_at");
            info.mAccept = false;
            infoList.push_back(info);
        }
    }
    catch(sql::SQLException& e) {
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    st->close();
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}

void ProcessGroupJoinReqProcessor::Exec(Connection *conn, Request &request, Response &response)
{
    GroupJoinRequest groupJoinRequest;
    bool ret = ProcessGroupJoinRequest(request, groupJoinRequest);
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        response.mhasData = true;
        std::string &data = response.mData;
        MyProtocolStream stream(data);
        stream << groupJoinRequest.user_id << groupJoinRequest.groupId << ret;
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool ProcessGroupJoinReqProcessor::ProcessGroupJoinRequest(Request &request, GroupJoinRequest groupJoinRequest)
{
    string mData = request.mData;
    MyProtocolStream stream(mData);
    stream >> groupJoinRequest.user_id >> groupJoinRequest.groupId >> groupJoinRequest.mAccept;
    //std::cout << fr.sender_id << "  " << fr.reciver_id << " " << fr.mAccept << std::endl;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //trans
    //TODO:这里时间应该重新设置
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update group_members set role = ?
        where user_id = ? and group_id = ?;)");
    state2->setInt(1, groupJoinRequest.mAccept ? 1 : 6);//6为拒绝
    state2->setInt(2, groupJoinRequest.user_id);
    state2->setInt(3, groupJoinRequest.groupId);
    //如果已经被其他管理员处理了要提示
    try {
        state2->executeUpdate();
    }
    catch(sql::SQLException& e) {
        //rollback
        cout << "# ERR " << e.what();
        cout << " Err Code: " << e.getErrorCode();
        cout << " SQLState: " << e.getSQLState() << std::endl;
        state2->close();
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
    return true;
}


/*
2024年10月10日21:23:22

线上加群

加群成功提示他人和返回。（不返回也行，微信就是）

修改时间

群名字通过Qt对象关联
*/