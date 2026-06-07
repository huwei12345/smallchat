# smallchat 项目文档

## 目录

- [项目概述](#项目概述)
- [项目整体评价](#项目整体评价)
- [后端功能与特点](#后端功能与特点)
- [前端功能与特点](#前端功能与特点)
- [技术栈](#技术栈)
- [项目架构](#项目架构)
- [分支管理](#分支管理)
- [代码问题分析与优化建议](#代码问题分析与优化建议)
- [部署指南](#部署指南)
- [数据库设计](#数据库设计)
- [API 接口说明](#api-接口说明)
- [开发规范](#开发规范)
- [路线图](#路线图)

---

## 项目概述

**smallchat** 是一款基于 C/S 架构的即时通讯软件，采用 C++ + Linux 服务器 + MySQL 数据库 + Qt 客户端的技术栈。目前处于基础功能开发与项目架构搭建阶段，基础功能可用约 50%。

### 已实现功能

| 模块 | 功能 |
|---|---|
| 用户 | 注册、登录、登出、状态管理 |
| 好友 | 搜索、添加、请求/接受、在线状态通知 |
| 消息 | 1对1聊天、群聊、图片/文件消息、已读状态 |
| 群组 | 创建、加入、审批、成员管理 |
| 文件 | FTP 上传/下载、云空间目录树、离线文件 |

---

## 项目整体评价

### 优势

1. **架构设计合理**：采用 Reactor 模式 + 多线程 IO，充分利用 Linux epoll 的高性能特性，具备良好的并发处理能力
2. **协议设计规范**：自定义二进制协议支持变长编码、校验和，相比 JSON/XML 更高效
3. **功能覆盖全面**：涵盖 IM 核心功能（用户、好友、消息、群组、文件），具备完整的产品形态
4. **前后端分离**：共享协议代码，保证序列化一致性，降低维护成本
5. **扩展性良好**：Command 模式便于新增功能，只需添加新的 Processor

### 不足

1. **线程安全缺失**：核心数据结构无锁保护，多线程环境下存在崩溃风险
2. **内存管理粗糙**：大量 raw pointer，缺乏 RAII 和智能指针
3. **错误处理不完善**：部分路径直接 exit，缺乏优雅降级
4. **测试覆盖为零**：无单元测试和集成测试
5. **配置硬编码**：数据库密码、端口号等散落在代码中

### 总体评分

| 维度 | 评分 | 说明 |
|---|---|---|
| 架构设计 | ⭐⭐⭐⭐ | Reactor 模式选型正确，线程模型合理 |
| 代码质量 | ⭐⭐ | 缺乏规范，内存管理需加强 |
| 功能完整度 | ⭐⭐⭐ | 核心功能基本可用，细节待完善 |
| 安全性 | ⭐⭐ | 无认证机制，存在 SQL 注入风险 |
| 可维护性 | ⭐⭐⭐ | 模块划分清晰，但缺乏文档和测试 |
| **综合** | **⭐⭐⭐** | **学习项目优秀，生产就绪需加固** |

---

## 后端功能与特点

### 核心功能

#### 1. 用户管理
- **注册/登录**：支持用户名+密码注册，密码哈希存储
- **会话管理**：Session 绑定 userId，支持在线/离线/隐身状态
- **状态通知**：用户上下线时自动通知好友

#### 2. 好友系统
- **搜索好友**：支持按用户名/ID 搜索
- **好友请求**：发送/接受/拒绝好友请求
- **好友列表**：内存缓存（FriendCache），快速查询
- **在线状态**：实时同步好友在线状态

#### 3. 消息系统
- **私聊消息**：1对1 文本消息
- **群聊消息**：群组内广播消息
- **消息类型**：文本、图片、文件
- **已读状态**：消息已读确认机制
- **离线消息**：支持离线文件存储和推送

#### 4. 群组管理
- **创建群组**：设置群名、描述、管理员
- **加入群组**：申请加入，管理员审批
- **成员管理**：成员角色（owner/admin/member）
- **群消息**：群内消息广播

#### 5. 文件传输
- **FTP 上传/下载**：基于 vsftpd 的文件传输
- **云空间**：用户个人文件存储（目录树结构）
- **离线文件**：支持离线文件发送和接收
- **文件管理**：创建目录、删除、重命名、移动

### 技术特点

#### 1. 高性能网络模型
```
┌─────────────────────────────────────────────────────────┐
│                    Main Thread                           │
│              (epoll 监听 + 轮询分配)                      │
└───────────────────────┬─────────────────────────────────┘
                        │ socketpair 唤醒
         ┌──────────────┼──────────────┐
         ▼              ▼              ▼
   ┌──────────┐   ┌──────────┐   ┌──────────┐
   │ IO Thread│   │ IO Thread│   │ IO Thread│
   │ (epoll)  │   │ (epoll)  │   │ (epoll)  │
   └──────────┘   └──────────┘   └──────────┘
```

- **边缘触发 (EPOLLET)**：减少系统调用，提高吞吐
- **非阻塞 IO**：所有 socket 设置 O_NONBLOCK
- **多线程分发**：Round-Robin 算法分配连接到 IO 线程

#### 2. 自定义二进制协议
```cpp
// 协议格式
┌─────────────┬─────────────┬─────────────────────┐
│ 包长度 (4B)  │ 校验和 (2B) │   数据 (变长)        │
└─────────────┴─────────────┴─────────────────────┘

// 7-bit 编码示例
uint32_t value = 300;  // 二进制: 100101100
// 编码后: 10101100 00000010 (2 字节)
```

- **变长整数**：7-bit 编码，小数字节省空间
- **流式序列化**：`stream << userId << nickname;`
- **校验和**：可选的数据完整性校验

#### 3. 命令模式请求处理
```cpp
// 服务器启动时注册
requestProcessor[FunctionCode::Login] = new LoginProcessor;
requestProcessor[FunctionCode::SendMessage] = new SendMessageProcessor;

// 收到请求时分发
requestProcessor[request->mFunctionCode]->Exec(conn, request, response);
```

- **解耦**：每个功能独立 Processor，互不影响
- **易扩展**：新增功能只需添加新 Processor 类
- **可测试**：每个 Processor 可独立测试

#### 4. 数据库连接池
```cpp
class MysqlPool {
    vector<sql::Connection*> mConnectionPool;      // 全部连接
    queue<sql::Connection*> mIdleConnectionQue;    // 空闲连接
    Mutex mMutex;                                   // 线程安全
};
```

- **连接复用**：避免频繁创建/销毁连接
- **线程安全**：互斥锁保护连接池
- **可配置**：连接数可调整（默认 4）

#### 5. 缓存机制
```cpp
class FriendCache {
    map<int, vector<int>> mFriendListMap;  // userId -> friendList
};
```

- **内存缓存**：好友列表常驻内存
- **快速查询**：O(1) 时间复杂度
- **减少 DB 压力**：避免频繁查询数据库

### 性能指标（理论值）

| 指标 | 估算值 | 说明 |
|---|---|---|
| 连接数 | 10K+ | 受限于文件描述符 |
| 消息吞吐 | 10K msg/s | 单机，取决于消息大小 |
| 延迟 | < 10ms | 局域网环境 |
| 内存占用 | ~100MB | 空闲状态，1000 连接 |

---

## 前端功能与特点

### 核心功能

#### 1. 用户界面
- **登录窗口**：用户名+密码登录，支持记住密码
- **注册窗口**：新用户注册
- **主窗口**：好友列表、群组列表、聊天窗口
- **设置页面**：服务器配置、个人信息修改

#### 2. 聊天功能
- **私聊窗口**：1对1 聊天界面
- **群聊窗口**：群组聊天界面
- **消息类型**：
  - 文本消息：支持富文本
  - 图片消息：内联显示，点击预览
  - 文件消息：超链接形式，点击下载
  - 表情消息：自定义表情选择器
  - 抖动窗口：特殊消息触发窗口抖动

#### 3. 好友管理
- **好友列表**：分组显示，支持搜索
- **好友请求**：查看/处理好友请求
- **用户资料卡**：查看好友详细信息
- **在线状态**：实时显示好友在线状态

#### 4. 群组功能
- **创建群组**：设置群名、描述
- **加入群组**：搜索并申请加入
- **群成员管理**：查看/管理群成员
- **群消息**：群聊消息收发

#### 5. 文件管理
- **文件发送**：选择文件发送给好友/群组
- **文件接收**：接收文件并保存到本地
- **云空间**：浏览/管理云端文件
- **头像上传**：上传/修改个人头像

### 技术特点

#### 1. Qt Widgets + QML 混合架构
```cpp
// Widgets: 主要界面
class LoginWindow : public QMainWindow { ... };
class ChatWindow : public QWidget { ... };

// QML: 特定组件
// PersonCard.qml - 用户资料卡
```

- **Widgets**：传统桌面 UI，稳定可靠
- **QML**：声明式 UI，适合动画和自定义组件
- **混合使用**：各取所长

#### 2. 无边框窗口设计
```cpp
class FramelessWidget : public QWidget {
    // 自定义标题栏
    // 支持拖拽移动
    // 支持边缘缩放
};
```

- **现代 UI**：无系统边框，自定义外观
- **可定制**：完全控制窗口样式
- **流畅体验**：自定义动画和过渡效果

#### 3. 系统托盘集成
```cpp
QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/main/icon.jpeg"));
trayIcon->setContextMenu(trayMenu);
trayIcon->show();
```

- **最小化到托盘**：关闭窗口时最小化到系统托盘
- **托盘菜单**：右键菜单显示/退出
- **消息提示**：托盘图标闪烁提示新消息

#### 4. 信号槽机制
```cpp
// 网络层通知 UI 层
connect(ClientNetWork::GetInstance(), &ClientNetWork::loginSuccessful, 
        this, &LoginWindow::onLoginSuccessful);

// UI 层通知网络层
connect(this, &ChatWindow::confirmMessage, 
        ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
```

- **松耦合**：模块间通过信号通信，无直接依赖
- **线程安全**：支持跨线程信号槽连接
- **类型安全**：编译时检查参数类型

#### 5. 自定义消息渲染
```cpp
class ChatViewDelegate : public QStyledItemDelegate {
    // 自定义绘制消息气泡
    // 支持文本/图片/文件不同样式
    // 支持富文本和表情
};

// 消息类型
class ChatEditText : public QWidget { ... };    // 文本消息
class ChatEditImage : public QWidget { ... };   // 图片消息
class ChatEditFile : public QWidget { ... };    // 文件消息
```

- **自定义委托**：QListView + Delegate 实现复杂消息样式
- **多种消息类型**：文本、图片、文件独立渲染
- **富文本支持**：支持表情、链接、图片混排

#### 6. 内置 FTP 客户端
```cpp
class FtpSender : public QObject {
    // FTP 文件上传/下载
    // 异步传输，不阻塞 UI
    // 传输进度回调
};
```

- **异步传输**：文件传输不阻塞界面
- **进度反馈**：实时显示传输进度
- **断点续传**：支持大文件传输中断恢复

#### 7. 表情系统
```cpp
class EmojiSelector : public QDialog {
    // 表情选择器
    // 表情编码/解码
    // 混合文本和表情
};
```

- **表情选择器**：网格布局，点击选择
- **自定义编码**：表情以特殊编码存储（如 `#0xa301`）
- **混合显示**：文本和表情可混合发送

### UI 页面一览

| 页面 | 类名 | 功能 |
|---|---|---|
| 登录页 | `LoginWindow` | 用户登录、注册入口、设置入口 |
| 注册页 | `RegisterPage` | 新用户注册 |
| 好友页 | `FriendPage` | 好友列表、群组列表、搜索 |
| 聊天页 | `ChatWindow` | 私聊界面、消息收发 |
| 群聊页 | `GroupChatWindow` | 群聊界面 |
| 设置页 | `SettingPage` | 个人信息、服务器配置 |
| 登录设置 | `LoginSettingPage` | 服务器 IP、FTP 配置 |
| 创建群组 | `CreateGroupPage` | 创建新群组 |
| 查找好友 | `FindFriendPage` | 搜索用户/群组 |
| 表情选择 | `EmojiSelector` | 表情网格选择器 |
| 用户资料 | `PersonCardCon` | 用户信息卡片 |

### 交互流程示例

```
用户登录流程:
┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐
│ LoginUI │───►│ Network │───►│ Server  │───►│   DB    │
└─────────┘    └─────────┘    └─────────┘    └─────────┘
     │              │              │              │
     │  Login Req   │  Login Req   │  Query User  │
     │─────────────►│─────────────►│─────────────►│
     │              │              │              │
     │              │  Login Resp  │  User Data   │
     │◄─────────────│◄─────────────│◄─────────────│
     │              │              │              │
     │ loginSuccess │              │              │
     │◄─────────────│              │              │
     │              │              │              │
     ▼              │              │              │
 FriendPage         │              │              │
```

---

## 技术栈

| 层级 | 技术 | 版本 |
|---|---|---|
| 服务器 | C++ + Linux epoll | C++11 |
| 数据库 | MySQL | 8.0 |
| 客户端 | Qt (Widgets + QML) | 5.9 ~ 5.14.2 |
| 文件传输 | FTP (vsftpd) | 稳定版 |
| 序列化 | 自定义二进制协议 | - |

---

## 项目架构

### 服务器 Reactor 模型

```
                    ┌─────────────────┐
                    │   Main Thread   │
                    │  (epoll 监听)    │
                    └────────┬────────┘
                             │ socketpair 唤醒
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
        ┌──────────┐   ┌──────────┐   ┌──────────┐
        │ IO Thread│   │ IO Thread│   │ IO Thread│
        │ (epoll)  │   │ (epoll)  │   │ (epoll)  │
        └──────────┘   └──────────┘   └──────────┘
```

- **Main EventLoop**: 接受新连接，轮询分配给 IO 线程
- **IO EventLoop**: 处理读写事件，每个线程独立 epoll 实例
- **唤醒机制**: 通过 `socketpair` 实现线程间通信

### 请求处理 (Command Pattern)

```cpp
// FunctionCode 枚举定义 ~40 种操作
enum FunctionCode {
    Register = 1,
    Login = 2,
    SendMessage = 6,
    CreateGroup = 13,
    TransFile = 17,
    // ...
};

// 每个操作对应一个 Processor
class LoginProcessor : public RequestProcessor {
    void Exec(Connection* conn, Request& request, Response&);
};
```

服务器启动时注册所有 Processor，收到请求后根据 `FunctionCode` 分发。

### 自定义二进制协议 (MyProtocolStream)

```
┌─────────────┬─────────────┬─────────────────────┐
│ 包长度 (4B)  │ 校验和 (2B) │   数据 (变长)        │
└─────────────┴─────────────┴─────────────────────┘
```

- 7-bit 编码的变长整数
- 流式序列化: `stream << userId << nickname << age;`
- 流式反序列化: `stream >> userId >> nickname >> age;`

### 连接/会话模型

```cpp
class Connection {
    int clientSocket;      // socket fd
    char buffer[4096];     // 读缓冲
    Session* session;      // 关联会话
    EventLoop* mEvLoop;    // 所属事件循环
};

class Session {
    Connection* mConn;     // 关联连接
    int mUserId;           // 用户ID
    int mLoginState;       // ONLINE/OFFLINE/Invisible
};
```

- `Server::mConnectionMap` — fd → Connection
- `Server::mUserSessionMap` — userId → Session

### 共享代码 (服务器 ↔ Qt 客户端)

| 文件 | 用途 |
|---|---|
| `Protocol.h` | 数据模型、Request/Response、FunctionCode |
| `MyProtocolStream.cpp/.h` | 二进制序列化 |
| `Trans.h` | socket 收发辅助 |

通过 `soft.h` 中的 `#define SERVER` 条件编译区分平台（服务器用 pthread，客户端用 QMutex）。

### 目录结构

```
smallchat/
├── server              # 服务器可执行文件
├── makefile            # 服务器构建文件
├── *.cpp / *.h         # 服务器源码
├── cache/              # 好友缓存模块
│   └── friendCache.cpp/.h
├── client/             # 测试客户端
│   ├── makefile
│   └── *.cpp / *.h
├── qtClient/           # Qt GUI 客户端
│   ├── untitled2.pro   # Qt 项目文件
│   ├── config.ini      # 客户端配置
│   ├── libs/           # 预编译库
│   └── ftp/            # FTP 客户端实现
├── sql/                # 数据库初始化
│   ├── main.cpp        # 建表脚本
│   ├── MysqlManager.cpp/.h
│   └── sql.zip         # MySQL 驱动安装包
├── test/               # 协议测试
├── vsftpd.conf         # FTP 配置文件
├── soft.h              # 条件编译开关
├── mutex.h             # 跨平台互斥锁
├── threadPool.h        # 线程管理
└── txt.md              # 开发笔记
```

---

## 分支管理

### 分支概览

| 分支 | 说明 | 最新提交 |
|---|---|---|
| **mainx** (当前) | 主要开发分支，功能最完整 | `d06a4c5` setting |
| main | 稳定主分支 | 较早版本 |
| origin/dev_session | 早期 session 开发 | `3818f0c` |
| origin/hw/feat/f1 | 演示分支（仅测试文件） | `3e1f59a` |

### mainx 分支开发进度

从提交历史看，已完成的功能：

- 用户系统：注册、登录、logout、用户信息卡
- 聊天功能：1对1聊天、群聊、消息重构、图片传输显示
- 文件功能：文件列表、Space 文件管理、文件删除重命名
- UI：表情支持、抖动窗口、头像显示、chatWindow 重构
- 配置：setting 页面、login 配置页面

---

## 代码问题分析与优化建议

基于代码审查，从**架构、安全、性能、代码质量、项目管理**五个维度分析：

### 一、架构问题 (高优先级)

| 问题 | 位置 | 影响 | 建议 |
|---|---|---|---|
| **线程安全缺失** | `mConnectionMap`/`mUserSessionMap` 多线程读写无锁 | 崩溃、数据竞争 | 添加读写锁或使用线程安全容器 |
| **单例模式非线程安全** | `Server::GetInstance()`, `MysqlPool::GetInstance()` | 多线程初始化竞争 | 使用 `std::call_once` 或 Meyer's Singleton |
| **连接管理职责不清** | `EventLoop` 和 `Server` 都操作 Connection | 生命周期混乱 | 统一由 ConnectionManager 管理 |
| **无写缓冲区** | `EventLoop::sendDataAll` 直接同步写 | 大消息阻塞 IO 线程 | 实现异步写缓冲区 |
| **任务队列无锁** | `EventLoop::mTaskQueue` 多线程访问 | 数据竞争 | 使用线程安全队列 |

### 二、安全问题 (高优先级)

| 问题 | 位置 | 风险 | 建议 |
|---|---|---|---|
| **SQL 注入** | `MysqlPool` 直接拼接 SQL | 数据库被攻击 | 全部使用 PreparedStatement |
| **硬编码密码** | `MysqlPool.h:18` root/123456 | 凭证泄露 | 使用环境变量或配置文件 |
| **无输入校验** | `readRequest` 未校验数据长度 | 缓冲区溢出 | 添加长度边界检查 |
| **无认证机制** | 服务器无 token/session 验证 | 未授权访问 | 实现 JWT 或 session 认证 |
| **FTP 明文传输** | `vsftpd.conf` | 数据泄露 | 启用 FTPS 或用 SSH/SFTP |

### 三、性能问题 (中优先级)

| 问题 | 位置 | 影响 | 建议 |
|---|---|---|---|
| **同步数据库操作** | 所有 Processor 直接调用 MySQL | 阻塞 IO 线程 | 异步 DB 线程池 |
| **连接池过小** | `MysqlPool` 默认 4 连接 | 高并发瓶颈 | 动态调整或增加默认值 |
| **无心跳机制** | Session 无超时检测 | 死连接占用资源 | 添加心跳包和超时清理 |
| **epoll_wait 超时** | `Run()` 中 100ms 超时 | CPU 空转 | 改用事件驱动，无事件时阻塞 |
| **listen backlog 过小** | `listen(fd, 5)` | 高并发连接丢失 | 增加到 128 或更高 |

### 四、代码质量问题

| 问题 | 示例 | 建议 |
|---|---|---|
| **内存泄漏** | `parseRequest` 返回 `new Request`，部分路径未 delete | 使用智能指针 `unique_ptr` |
| **资源未释放** | `EventLoop::addSocket` 异常时未关闭 fd | RAII 包装 socket fd |
| **魔法数字** | `buffer[4096]`, `requestProcessor[100]` | 定义常量 |
| **调试输出** | 大量 `printf` 散落各处 | 统一日志系统 (spdlog) |
| **注释代码** | `server.cpp` 大量注释掉的旧代码 | 删除，用 git 历史追溯 |
| **职责混乱** | `Connection::processRead` 包含业务逻辑 | 分离网络层和业务层 |
| **错误处理** | `exit(EXIT_FAILURE)` 滥用 | 异常或错误码传播 |
| **拼写错误** | `NofifyFileComing`, `eraseSocket` | 统一命名规范 |

### 五、项目管理问题

| 问题 | 建议 |
|---|---|
| **无单元测试** | 添加 gtest，至少覆盖协议序列化、业务逻辑 |
| **无 CI/CD** | 配置 GitHub Actions 自动构建 |
| **无代码规范** | 添加 `.clang-format`，统一风格 |
| **硬编码配置** | 数据库连接、端口号等提取到配置文件 |
| **无文档** | API 文档、架构图、部署文档 |
| **缺少 TODO 追踪** | 代码中 TODO 散落，应使用 Issue 管理 |

### 优化优先级建议

```
P0 (立即修复):
├── 线程安全 (mConnectionMap 锁)
├── SQL 注入防护
└── 内存泄漏修复

P1 (短期优化):
├── 异步数据库线程池
├── 写缓冲区实现
├── 心跳机制
└── 配置外部化

P2 (中期改进):
├── 单元测试覆盖
├── 统一日志系统
├── 认证机制
└── CI/CD 流水线

P3 (长期规划):
├── 协议升级 (Protobuf)
├── 分布式支持
└── 性能压测与调优
```

---

## 部署指南

### 一、环境要求

| 组件 | 版本要求 | 说明 |
|---|---|---|
| **操作系统** | Ubuntu 22.04 LTS | 服务器端 |
| **编译器** | g++ (支持 C++11) | 服务器编译 |
| **MySQL** | 8.0 | 数据库 |
| **MySQL Connector/C++** | 8.0+ | C++ 驱动 |
| **Qt** | 5.9 ~ 5.14.2 | 客户端 (推荐 5.14.2) |
| **vsftpd** | 任意稳定版 | FTP 文件传输 |

### 二、服务器端部署

#### 1. 安装系统依赖

```bash
# 更新系统
sudo apt update && sudo apt upgrade -y

# 安装编译工具
sudo apt install -y g++ make

# 安装 MySQL
sudo apt install -y mysql-server mysql-client

# 安装 ODBC 依赖
sudo apt install -y unixodbc unixodbc-dev odbcinst
```

#### 2. 安装 MySQL Connector/C++

```bash
# 从项目 sql 目录解压安装包
cd sql/
unzip sql.zip
sudo dpkg -i *.deb

# 或从官方下载安装
# https://dev.mysql.com/downloads/connector/cpp/
```

#### 3. 配置 MySQL

```bash
# 启动 MySQL
sudo systemctl start mysql
sudo systemctl enable mysql

# 设置 root 密码 (项目默认: 123456)
sudo mysql_secure_installation

# 登录 MySQL
mysql -u root -p

# 创建数据库用户 (可选，生产环境建议)
CREATE USER 'smallchat'@'localhost' IDENTIFIED BY 'your_password';
GRANT ALL PRIVILEGES ON mIM.* TO 'smallchat'@'localhost';
FLUSH PRIVILEGES;
```

#### 4. 初始化数据库

```bash
cd sql/
make
./main
```

执行后会自动创建 `mIM` 数据库和以下表：
- `users` - 用户表
- `messages` - 私聊消息表
- `group_messages` - 群消息表
- `friendships` - 好友关系表
- `group_t` - 群组表
- `group_members` - 群成员表
- `user_storage` - 用户云存储表
- `offline_transfers` - 离线文件传输表

#### 5. 配置 FTP 服务

```bash
# 安装 vsftpd
sudo apt install -y vsftpd

# 备份原配置
sudo cp /etc/vsftpd.conf /etc/vsftpd.conf.bak

# 使用项目配置
sudo cp vsftpd.conf /etc/vsftpd.conf

# 创建 FTP 用户 (项目默认: huwei)
sudo useradd -m huwei
sudo passwd huwei  # 设置密码: 123456

# 创建 FTP 根目录
sudo mkdir -p /home/huwei/ftp
sudo chown -R huwei:huwei /home/huwei/ftp

# 创建 chroot_list 文件
sudo touch /etc/vsftpd.chroot_list

# 重启 vsftpd
sudo systemctl restart vsftpd
sudo systemctl enable vsftpd
```

#### 6. 编译并运行服务器

```bash
cd smallchat/
make
./server
```

服务器默认监听端口 **8080**。

### 三、客户端部署

#### 1. 安装 Qt 开发环境

```bash
# Ubuntu 安装 Qt
sudo apt install -y qt5-default qtcreator

# 或从官网安装 Qt 5.14.2
# https://www.qt.io/download-qt-installer
```

#### 2. 配置项目

编辑 `qtClient/config.ini`：

```ini
[global]
SERVERIP=192.168.58.132    # 服务器 IP 地址
FTPIP=192.168.58.132       # FTP 服务器 IP
FTPPORT=21                 # FTP 端口
FTPUSER=huwei              # FTP 用户名
FTPPASSWORD=123456         # FTP 密码
FTPTYPE=normal
```

#### 3. 编译客户端

```bash
# 方式1: Qt Creator
# 1. 打开 qtClient/untitled2.pro
# 2. 选择 Qt 5.14 套件
# 3. 点击构建

# 方式2: 命令行
cd qtClient/
qmake
make
```

### 四、测试客户端 (可选)

```bash
cd client/
make
./client
```

这是一个简单的命令行测试客户端，用于验证服务器功能。

### 五、网络配置

确保以下端口开放：

| 端口 | 协议 | 用途 |
|---|---|---|
| 8080 | TCP | IM 服务器 |
| 21 | TCP | FTP 控制连接 |
| 30000-30999 | TCP | FTP 被动模式数据连接 |

```bash
# 防火墙配置
sudo ufw allow 8080/tcp
sudo ufw allow 21/tcp
sudo ufw allow 30000:30999/tcp
```

### 六、常见问题

| 问题 | 解决方案 |
|---|---|
| `mysql_driver.h not found` | 检查 MySQL Connector/C++ 是否正确安装 |
| `libmysqlcppconn.so not found` | `sudo ldconfig` 或设置 `LD_LIBRARY_PATH` |
| FTP 连接失败 | 检查 vsftpd 状态、用户权限、防火墙 |
| Qt 编译错误 | 确认 Qt 版本 5.9~5.14.2，检查套件配置 |
| 服务器启动失败 | 检查端口 8080 是否被占用 |

### 七、生产环境建议

| 项目 | 当前状态 | 建议 |
|---|---|---|
| 数据库密码 | 硬编码 123456 | 使用环境变量或配置文件 |
| FTP 传输 | 明文 | 启用 FTPS 或改用 SFTP |
| 日志 | printf 输出 | 使用日志文件 (log4cpp/spdlog) |
| 进程管理 | 手动运行 | 使用 systemd 管理服务 |
| 监控 | 无 | 添加健康检查和监控 |

---

## 数据库设计

### ER 图

```
┌─────────────┐       ┌─────────────┐       ┌─────────────┐
│    users    │       │ friendships │       │    users    │
│─────────────│       │─────────────│       │─────────────│
│ user_id (PK)│◄──────│ user1_id(FK)│       │ user_id (PK)│
│ username    │       │ user2_id(FK)│──────►│ username    │
│ password_hash│      │ status      │       │ ...         │
│ email       │       │ since       │       └─────────────┘
│ ...         │       └─────────────┘
└─────────────┘
       │
       │ 1:N
       ▼
┌─────────────┐       ┌─────────────┐
│  messages   │       │   group_t   │
│─────────────│       │─────────────│
│ message_id  │       │ group_id(PK)│
│ sender_id   │       │ group_name  │
│ recipient_id│       │ admin_id(FK)│
│ content     │       │ ...         │
│ is_read     │       └─────────────┘
│ ...         │              │
└─────────────┘              │ 1:N
                             ▼
                    ┌─────────────┐
                    │group_members│
                    │─────────────│
                    │group_id (FK)│
                    │ user_id(FK) │
                    │ role        │
                    │ ...         │
                    └─────────────┘
```

### 表结构说明

#### users 表
```sql
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
```

#### messages 表
```sql
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
```

#### friendships 表
```sql
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
```

#### group_t 表
```sql
CREATE TABLE group_t(
    group_id INT AUTO_INCREMENT PRIMARY KEY,
    group_name VARCHAR(100) UNIQUE NOT NULL,
    description TEXT,
    admin_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    gtype VARCHAR(100) DEFAULT NULL,
    Tips TEXT,
    FOREIGN KEY (admin_id) REFERENCES users(user_id)
);
```

#### group_members 表
```sql
CREATE TABLE group_members (
    group_member_id INT AUTO_INCREMENT PRIMARY KEY,
    group_id INT NOT NULL,
    user_id INT NOT NULL,
    role ENUM('member', 'admin', 'owner', 'applyer', 'tourist', 'reject') DEFAULT 'member',
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (group_id) REFERENCES group_t(group_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    CONSTRAINT unique_membership UNIQUE (group_id, user_id)
);
```

#### user_storage 表
```sql
CREATE TABLE user_storage (
    storage_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    parent_id INT,
    item_name VARCHAR(255) NOT NULL,
    item_type VARCHAR(255) NOT NULL,
    file_path VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    expired_time INT DEFAULT 0,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (parent_id) REFERENCES user_storage(storage_id),
    CONSTRAINT unique_item_name_per_user UNIQUE (user_id, parent_id, item_name)
);
```

#### offline_transfers 表
```sql
CREATE TABLE offline_transfers (
    transfer_id INT AUTO_INCREMENT PRIMARY KEY,
    sender_id INT NOT NULL,
    receiver_id INT NOT NULL,
    file_name VARCHAR(255) NOT NULL,
    file_type VARCHAR(255) NOT NULL,
    file_size INT,
    upload_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    expiry_time TIMESTAMP,
    downloaded ENUM('true', 'false') DEFAULT 'false',
    FOREIGN KEY (sender_id) REFERENCES users(user_id),
    FOREIGN KEY (receiver_id) REFERENCES users(user_id),
    INDEX (expiry_time)
);
```

---

## API 接口说明

### FunctionCode 枚举

```cpp
enum FunctionCode {
    TEST                    = 0,
    Register                = 1,   // 用户注册
    Login                   = 2,   // 用户登录
    SearchAllFriend         = 3,   // 获取好友列表
    FindFriend              = 4,   // 搜索用户
    AddFriend               = 5,   // 添加好友
    SendMessage             = 6,   // 发送消息
    GetAllFriendRequest     = 7,   // 获取好友请求列表
    GetAllMessage           = 8,   // 获取聊天记录
    UpdateUserState         = 9,   // 更新用户状态
    ProcessFriendRequest    = 10,  // 处理好友请求
    ProcessMessageRead      = 11,  // 标记消息已读
    ReciveMessage           = 12,  // 接收消息
    CreateGroup             = 13,  // 创建群组
    JoinGroup               = 14,  // 加入群组
    ResponseJoinGroup       = 15,  // 响应加群请求
    TransFile               = 17,  // 传输文件
    FindGroup               = 18,  // 搜索群组
    StartUpLoadFile         = 19,  // 开始上传文件
    UpLoadFileSuccess       = 20,  // 上传文件完成
    GetFile                 = 21,  // 获取文件
    GetFileSuccess          = 22,  // 获取文件成功
    GetFileThird            = 23,  // 获取文件第三步
    NofifyFileComing        = 24,  // 通知文件即将到来
    AgreeRecvFile           = 25,  // 同意接收文件
    TransFileOver           = 26,  // 文件传输结束
    GetAllOfflineFile       = 27,  // 获取离线文件列表
    GetOfflineFile          = 28,  // 获取离线文件
    // ... 更多功能码
};
```

### 请求/响应格式

```cpp
class Request {
    int mFunctionCode;      // 功能码
    std::string mData;      // 序列化数据
    // ...
};

class Response {
    int mFunctionCode;      // 功能码
    int mReturnCode;        // 返回码
    bool returnFlag;        // 是否需要返回
    std::string mData;      // 序列化数据
    // ...
};
```

---

## 开发规范

### 代码风格

- 使用 4 空格缩进
- 类名使用 PascalCase
- 函数名使用 camelCase
- 常量使用 UPPER_SNAKE_CASE
- 成员变量以 `m` 前缀 (如 `mUserId`)

### 命名规范

| 类型 | 规范 | 示例 |
|---|---|---|
| 类名 | PascalCase | `RequestProcessor` |
| 函数名 | camelCase | `processRead()` |
| 成员变量 | m前缀 + camelCase | `mUserId` |
| 常量 | UPPER_SNAKE_CASE | `MAX_EVENTS` |
| 枚举值 | PascalCase | `FunctionCode::Login` |

### Git 提交规范

```
<type>(<scope>): <subject>

类型:
- feat: 新功能
- fix: 修复 bug
- docs: 文档更新
- style: 代码格式调整
- refactor: 重构
- test: 测试相关
- chore: 构建/工具相关
```

---

## 路线图

### 短期目标 (1-2 个月)

- [ ] 修复线程安全问题
- [ ] 实现写缓冲区
- [ ] 添加心跳机制
- [ ] 配置外部化
- [ ] 基础单元测试

### 中期目标 (3-6 个月)

- [ ] 异步数据库操作
- [ ] 统一日志系统
- [ ] 用户认证机制 (JWT)
- [ ] CI/CD 流水线
- [ ] 性能优化

### 长期目标 (6-12 个月)

- [ ] 协议升级 (Protobuf)
- [ ] 分布式部署支持
- [ ] 消息推送服务
- [ ] 音视频通话
- [ ] 移动端客户端

---

## 联系方式

- 作者: huwei
- QQ: 531931740
- GitHub: https://github.com/huwei12345/smallchat

---

*文档最后更新: 2026-06-01*
