#include "RequestProcessor.h"
#include "Protocol.h"
#include "MyProtocolStream.h"

#include "MysqlPool.h"
#include "server.h"

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
            cout << "id:" << userInfo.user_id << " name:" << userInfo.username << " email:" << userInfo.email << std::endl;
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
        session->mUserId = info.user_id;
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
        response.mData = friendList.friendListSerial();
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


void SendMessageProcessor::Exec(Connection* conn, Request &request, Response& response)
{
    MessageInfo message;
    bool ret = false;
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    MessageInfo info;
    stream >> info.receiver_id >> info.message_text;

    if (Server::GetInstance()->mUserSessionMap.count(info.receiver_id)) {
        //向消息所向用户发送消息
        //从userId索引到Connection再得到clientSocket
        //TODO:在线时，接收消息和朋友请求的逻辑
        //思路1：在线，直接发送，或者入库read=0, 再尝试发送，再修改read=1
        //思路2：直接入库read=0，不发送，等客户端进行心跳连接时，相应新消息和朋友请求
        //思路3：当在库中新增一项时，触发某任务，向客户发送消息
        Session* session = Server::GetInstance()->mUserSessionMap[info.receiver_id];
        Connection* friendConn = session->mConn;
        // if (friendConn != NULL && friendConn->connState != DisConnectionState) {
        //     sendMessageByNetwork(info, /*info.receiver_id*/);
        //     UpMessageDateBase();
        // }
    }
    else {
        //不在线，存库，等上线后发送
        ret = SendMessage(request);
    }
    response.init(ret, request.mType, request.mFunctionCode, request.mFlag, !request.mDirection, request.mTimeStamp + 10, request.mUserId);
    if (response.mCode) {
        //向当前用户返回true
        //response.mData = message.SendMessageSerial();
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
        SELECT u.user_id AS friend_id, u.username AS friend_username, u.email
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

bool FindFriendProcessor::FindFriend(const Request &request, FriendList &friendList)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    UserInfo info;
    stream >> info.username;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //查询
	sql::PreparedStatement* state2 = conn->prepareStatement("select * from users where username = ?;");

	state2->setString(1, info.username);
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

bool AddFriendProcessor::AddFriend(const Request &request)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    UserInfo info;
    stream >> info.user_id;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //查询
	sql::PreparedStatement* state = conn->prepareStatement("select * from users where user_id = ?;");
	state->setInt(1, request.mUserId);
    sql::ResultSet *st = state->executeQuery();
    
    if (st->rowsCount() <= 0) {
        printf("Row %d\n", (int)st->rowsCount());
        MysqlPool::GetInstance()->releaseConncetion(conn);
        return false;
    }
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

bool SendMessageProcessor::SendMessage(const Request &request)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    MessageInfo info;
    stream >> info.receiver_id >> info.message_text;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }

	sql::PreparedStatement* state2 = conn->prepareStatement(R"(insert into messages
        (sender_id, recipient_id, content) 
        values(?,?,?);)");
    state2->setInt(1, request.mUserId);
    state2->setInt(2, info.receiver_id);
    state2->setString(3, info.message_text);
    state2->execute();

    state2->close();
    MysqlPool::GetInstance()->releaseConncetion(conn);
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
    stream << user_id << username;
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
        response.mhasData = true;
        response.mData = request.mData;
    }
    else {
        response.mhasData = false;
        //some error info add
    }
}

bool UpdateUserStateProcessor::UpdateUserState(const Request &request)
{
    std::string mData = request.mData;
    MyProtocolStream stream(mData);
    int state = 0;
    stream >> state;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    //查询
	sql::PreparedStatement* state2 = conn->prepareStatement("update status = ? from users where user_id = ?;");
    state2->setInt(1, state);
	state2->setInt(2, request.mUserId);
    try {
        state2->executeUpdate();
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
}

bool ProcessMessageReadProcessor::ProcessMessageRead(Request &request)
{
    //范围确认 start-end
    //后续可以优化为根据协议如何确认，如type = byte[0] type1:范围确认 type2:单条确认 type3:时间确认 type4:位图确认
    string& data = request.mData;
    MyProtocolStream stream(data);
    int send_id = 0, recv_id = 0;
    int start = 0, end = 0;
    stream >> send_id >> recv_id;
    stream >> start >> end;
    sql::Connection* conn = MysqlPool::GetInstance()->getConnection();
    if (conn == NULL) {
        return false;
    }
    sql::PreparedStatement* state2 = conn->prepareStatement(R"(update is_read = ? from messages
        where sender_id = ? and recipient_id = ? and message_id >= ? and message_id <= ? ;)");
    state2->setInt(1, send_id);
    state2->setInt(2, recv_id);
    state2->setInt(3, start);
    state2->setInt(4, end);
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
