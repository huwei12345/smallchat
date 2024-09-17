#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <string>
#include <string.h>
#include <vector>
 
using namespace std;
using namespace sql;
using namespace sql::mysql;

std::string rawString = R"(This is a raw string with "quotes", \backslashes, and
line breaks. It's very convenient for including literals with special characters.)";

vector<std::string> tableCreateStringMap = {
	//User
	R"(
	CREATE TABLE users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    full_name VARCHAR(100),
    avatar_url VARCHAR(255),
    bio TEXT,
    status ENUM('online', 'offline') DEFAULT 'offline',
    last_login DATETIME,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    flag CHAR DEFAULT 0,
    sex BOOLEAN DEFAULT true,
    age INT DEFAULT 0,
    address VARCHAR(200) DEFAULT 'unknown',
    now_address VARCHAR(200) DEFAULT 'unknown',
    useTime INT DEFAULT 0
	);
)",
	//Messages
	R"(
	CREATE TABLE messages (
    message_id INT AUTO_INCREMENT PRIMARY KEY,
    sender_id INT NOT NULL,
    recipient_id INT NOT NULL,
    content TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    is_read BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (sender_id) REFERENCES users(user_id),
    FOREIGN KEY (recipient_id) REFERENCES users(user_id)
	);
)",
	//FriendShips
	R"(
	CREATE TABLE friendships (
    friendship_id INT AUTO_INCREMENT PRIMARY KEY,
    user1_id INT NOT NULL,
    user2_id INT NOT NULL,
    status ENUM('pending', 'accepted', 'rejected') DEFAULT 'pending',
    since DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user1_id) REFERENCES users(user_id),
    FOREIGN KEY (user2_id) REFERENCES users(user_id),
    CONSTRAINT unique_friendship UNIQUE (user1_id, user2_id)
	);
)",
	//Groups
	R"(
	CREATE TABLE group_t(
    group_id INT AUTO_INCREMENT PRIMARY KEY,
    group_name VARCHAR(100) NOT NULL,
    description TEXT,
    admin_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    gtype VARCHAR(100) DEFAULT NULL,
    Tips TEXT,
    FOREIGN KEY (admin_id) REFERENCES users(user_id)
	);
)",
	//GroupMembers
	R"(
    CREATE TABLE group_members (
    group_member_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    user_id INT NOT NULL,
    role ENUM('member', 'admin', 'owner', 'applyer') DEFAULT 'member',
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (group_id) REFERENCES group_t(group_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    CONSTRAINT unique_membership UNIQUE (group_id, user_id)
    );
)",
	//UserStorage
	R"(
    CREATE TABLE user_storage (
    storage_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    parent_id INT,
    item_name VARCHAR(255) NOT NULL,
    item_type ENUM('file', 'folder', 'image', 'video') NOT NULL,
    file_path VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (parent_id) REFERENCES user_storage(storage_id),
    CONSTRAINT unique_item_name_per_user UNIQUE (user_id, parent_id, item_name)
    );
)",
	//OffllineTransfers
	R"(
    CREATE TABLE offline_transfers (
    transfer_id INT AUTO_INCREMENT PRIMARY KEY,
    sender_id INT NOT NULL,
    receiver_id INT NOT NULL,
    file_name VARCHAR(255) NOT NULL,
    file_type ENUM('file', 'video', 'zip') NOT NULL,
    file_size INT,
    upload_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expiry_time TIMESTAMP,
    downloaded ENUM('true', 'false') DEFAULT 'false',
    FOREIGN KEY (sender_id) REFERENCES users(user_id),
    FOREIGN KEY (receiver_id) REFERENCES users(user_id),
    INDEX (expiry_time)
    );
)"
};



int testSql() {
	Driver* driver = sql::mysql::get_driver_instance();
	string host = "tcp://127.00.1:3306/study";
	string user = "root";
	string passwd = "123456";
	Connection* conn = driver->connect(host, user, passwd);
	if (conn->isValid()) {
		printf("conn success\n");
	}
	//添加
	//PreparedStatement* state = conn->prepareStatement("insert into student(username, password, age) Values(?, ?, ?);");
	////state: insert into
	//state->setString(1, "xf");
	//state->setString(2, "333");
	//state->setInt(3, 40);
	//state->execute();
 	//printf("success 2\n\n");

	//查询
	PreparedStatement* state2 = conn->prepareStatement("select * from student");
	//state2->setString(1, "101");
	//state2->setString(2, "101");
	//state2->setInt(3, 101);
	ResultSet *st = state2->executeQuery();
 	printf("success 3\n\n");
	
try {	
	while (st->next()) {
		int id = st->getInt("id");
		string name = st->getString("username");
		int age = st->getInt("age");
		cout << "id:" << id << " name:" << name << " age:" << age << endl;
	}	
 }
catch(sql::SQLException& e) {
	cout << "# ERR " << e.what();
	cout << " Err Code: " << e.getErrorCode();
	cout << " SQLState: " << e.getSQLState() << endl;
}
	//删除
	PreparedStatement* state3 = conn->prepareStatement("delete from student where username = ? and password = ?;");
	state3->setString(1, "xf");
	state3->setString(2, "333");
	state3->execute();
	printf("success 4\n\n");


	//修改
	PreparedStatement* state4 = conn->prepareStatement("update student set age = ? where username = ?;");
	state4->setString(1, "27");
	state4->setString(2, "lixin");
	state4->execute();
	printf("success 5\n\n");
	

	//普通statement
	Statement *stmt = conn->createStatement();
	//stmt->executeQuery("select * from student");
	//stmt->execute("select * from student");
	//stmt->executeUpdate("select * from student");
	//此处若调用了execute系列函数，则必须对可能的结果集进行遍历或者处理，否则不能进行后续的设置事务操作，有错误
	printf("success 6\n\n");
	//conn->commit();


	//事务 默认的连接是每个都是一个事务的，需要手动关闭，形成多条语句一个事务
try {
	Statement *stmt2 = conn->createStatement();
	conn->setAutoCommit(false);
	string sql1 = "insert into student(username, password, age) Values(\"lz\", \"123\", 25);";
	string sql2 = "insert into student(id, username, password, age) Values(33, 111, \"123\", 25);";
	//即时是increament也能添加成功，但2次添加时，就由于重复而添加不成功，事务回滚
	stmt2->execute(sql1);
	stmt2->execute(sql2);
	conn->commit();
	printf("success 7\n\n");
}
catch(sql::SQLException& e) {
	cout << "failure 7 update falied: " << e.what() << endl;
	//conn->rollback();
}
	printf("success 8\n\n");
	return 0;

}

void initMysqlTable(Connection* conn) {
	try {
		conn->setAutoCommit(false);
		Statement *stmt2 = conn->createStatement();
		stmt2->execute("create database mIM");
		stmt2->execute("use mIM");
		bool ret = true;
		for (auto execStr : tableCreateStringMap) {
			stmt2->execute(execStr);
		}
		conn->commit();
		printf("Create Mysql Table Success\n\n");
	}
	catch(sql::SQLException& e) {
		cout << "Create Mysql Table Failure: " << e.what() << endl;
		conn->rollback();
	}
}

int main()
{
	Driver* driver = sql::mysql::get_driver_instance();
	string host = "tcp://127.00.1:3306";
	string user = "root";
	string passwd = "123456";
	Connection* conn = driver->connect(host, user, passwd);
	if (conn->isValid()) {
		printf("conn success\n");
	}
	initMysqlTable(conn);
	return 0;
}


//执行sql语句的三个接口：
//
//1、executeUpdate 执行 INSERT、UPDATE 或 DELETE 语句以及 SQL DDL（数据定义语言）语句，该方法返回 int 类型的值，显示数据库中改变的行数
//2、executeQuery 方法，用于执行查询语句的方法，执行后返回代表查询结果的ResultSet对象，在ResultSet对象中存储查询产生的结果。
//ResultSet.next()方法是判断这个结果集中下一行是否有内容，如果有内容，返回true，逐行判断，ResultSet.get×××（参数）方法是用来获取这一行的某一个字段，这里的‘参数’，可以是字段的名字也可以相对来说字段名的位置（从1开始）
//2、execute返回boolean类型，true表示执行的是查询语句，false表示执行的是insert,delete,update等等。
//如果返回值为true ，则用 statement.getResultSet();方法返回一个ResultSet对象，就可以对ResultSet对象遍历输出，查看结果。


//char 和 varchar 区别
//`char`和`varchar`都是SQL数据库中用于存储字符串数据类型的字段类型，它们的主要区别在于存储方式、空间效率以及长度处理：
//
//1. 存储方式：
//   char：定长字符串类型，每个`char`类型的字段会占用预定义的固定长度的空间，不论实际存储的字符串长度是否达到预设长度。例如，如果你声明一个`char(10)`字段，那么即使你只存储了3个字符，也会占用10个字符的存储空间，并用空格或其他空白字符填充剩余的7个字符位置。
//   varchar：变长字符串类型，根据实际存储的字符数动态分配存储空间。例如，对于`varchar(10)`，若存储3个字符，只会占用包含3个字符及必要结束标记的存储空间。
//
//2. 空间效率：
//   char：当数据长度往往固定且较小，或者经常填充到预定义长度时，空间利用率较高，但存储短字符串时可能会造成空间浪费。
//   varchar：更适合存储长度变化较大的字符串，能有效节省空间，特别是对于大多数字符串都不满预定义长度的情况下。
//
//3. 性能：
//   char：因为长度固定，在数据存储和检索上可能会更快，特别是在索引密集型操作中，因为存储结构更加紧凑和可预测。
//   varchar：数据长度不固定，尽管现代数据库系统对此做了优化，但在某些条件下，尤其是在索引较大且字符串长度变化范围大的情况下，其性能可能略低于char。
//
//4. 适用场景：
//   char：适用于存储固定长度的数据，如身份证号码、ID、邮政编码等，或者是长度一定且不需要额外空间的场景。
//  varchar：适用于存储长度可变的数据，如姓名、地址、评论等，这些数据长度可能有很大差异。
//
//需要注意的是，不同数据库系统对char和varchar的具体实现可能会有所不同，包括最大长度限制、是否填充空格、存储效率等方面。在MySQL等数据库中，varchar的最大长度可达65535字节，而在Oracle数据库中，varchar2是varchar的增强版本，通常用于替代varchar。
