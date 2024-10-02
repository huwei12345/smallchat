﻿#include "friendpage.h"
#include "ui_friendpage.h"
#include <QToolButton>
#include <QIcon>
#include <QKeyEvent>
#include <QToolButton>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include "findfriendpage.h"
#include "creategrouppage.h"
#include "chatwindow.h"
#include "userbutton.h"
#include "network.h"
#include "processor.h"
#include "ftpsender.h"
bool FriendPage::initPage() {
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));

    // 设置图片，可以是本地文件路径或者网络图片URL
    QIcon icon(":/friend/touxiang.jpeg"); // 使用资源文件
    // 或者
    // QIcon icon("path/to/your/image.png"); // 使用本地文件路径
    ui->toolButton_5->setIcon(icon);
    ui->toolButton_5->setIconSize(QSize(50, 50)); // 设置图片大小
    m_index = 0;
    m_FindFriendPage = NULL;
    m_CreateGroupPage = NULL;
    //更新头像
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ChangeOwnerPic, this, &FriendPage::ChangeOwnerPic);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::findAllFriendSuccess, this, &FriendPage::findAllFriendSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllMessageSuccess, this, &FriendPage::getAllMessageSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllOfflineFileSuccess, this, &FriendPage::getAllOfflineFileSuccess);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllFriendRequestSuccess, this, &FriendPage::getAllFriendRequestSuccess);


    connect(ClientNetWork::GetInstance(), &ClientNetWork::MessageArriveClient, this, &FriendPage::MessageArriveClient);
    //被动离线文件到来，转为主动获取
    connect(ClientNetWork::GetInstance(), &ClientNetWork::NofifyFileComing, this, &FriendPage::NofifyFileComing);


    //修改个人在线状态，未完成
    connect(ClientNetWork::GetInstance(), &ClientNetWork::UpDateUserStateSuccess, this, &FriendPage::UpDateUserStateSuccess);


    //主动发送接收
    connect(ClientNetWork::GetInstance(), &ClientNetWork::GetFileFirstSuccess, this, &FriendPage::GetFileFirstSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::StartUpLoadFileSuccess, this, &FriendPage::StartUpLoadFileSuccess);

    //获取 服务器对客户端已经收发完成消息的相应，服务器的响应文件操作成功，非FTP的文件操作成功
    connect(ClientNetWork::GetInstance(), &ClientNetWork::GetFileSuccess, this, &FriendPage::GetFileSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::UpLoadFileSuccess, this, &FriendPage::SendFileSuccess);
    return true;
}

FriendPage::FriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendPage)
{
    ui->setupUi(this);
    initPage();
}



FriendPage::FriendPage(UserInfo &info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendPage)
{
    mInfo = info;
    mUserId = info.user_id;
    ui->setupUi(this);
    initPage();
}

int FriendPage::init() {
    int ret = initFriendList();
    //QThread::msleep(20); // 睡眠1秒
    ret = initMessageList();
    //QThread::msleep(20); // 睡眠1秒
    ret = initFriendRequest();

    ret = initMyPhoto();
    ret = initAllOfflineFile();
    return ret;
}


bool FriendPage::initMyPhoto() {
    FileInfo info;
    info.Generate();
    info.serverPath = "userPhoto/";
    info.serverFileName = "tx" + mInfo.username + ".jpg";
    info.ClientPath = QCoreApplication::applicationDirPath().toStdString() + "/userPhoto/" + info.serverFileName;
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::GetFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool FriendPage::initFriendList() {
    bool ret = Processor::findAllFriend(mUserId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool FriendPage::initMessageList() {
    bool ret = Processor::getAllMessage(mUserId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool FriendPage::initFriendRequest() {
    bool ret = Processor::getAllFriendRequest(mUserId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool FriendPage::initAllOfflineFile() {
    bool ret = Processor::getAllOfflineFile(mUserId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

void FriendPage::findAllFriendSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        UserInfo info;
        stream2 >> info.user_id >> info.friendStatus >> info.username >> info.email;
        addFriendToPage(i, info);//
        info.print();
        mFriendList.push_back(info);
        ChatWindow* chatWindow = new ChatWindow(info);
        mChatWindowMap[info.user_id] = chatWindow;
        connect(this, &FriendPage::userMessageUnRead, chatWindow, &ChatWindow::userMessageRead);
        connect(chatWindow, &ChatWindow::friendPageUpdate, this, &FriendPage::friendPageUpdate);
    }
}


void FriendPage::getAllMessageSuccess(Response response) {
    //TODO:闪烁几个窗口，右下角图标添加几个提示
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    qDebug() << "sizeMessage: " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        MessageInfo info;
        stream2 >> info.id;
        stream2 >> info.sender_id;
        stream2 >> info.message_text;
        stream2 >> info.timestamp;
        qDebug() << info.sender_id;
        qDebug() << QString::fromStdString(info.message_text);
        info.receiver_id = user_id;

        info.print();
        QString str = mFriendButton[info.sender_id]->text();
        int index = str.indexOf("[New");
        if (index != -1)
            str = str.remove(index, str.length() - index);
        //有新消息来时添加text,点击后，出现新消息后，删除text
        mFriendButton[info.sender_id]->setText(str + "  [New Message]");

        //这个ChatWindow一定一定已经在上面一个函数里创建了
        mChatWindowMap[info.sender_id]->addMessage(info);
    }
}

void FriendPage::getAllOfflineFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    qDebug() << "sizeOfflineFile: " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        FileInfo info;
        info.Generate();
        stream2 >> info.ftpTaskId;
        stream2 >> info.id;
        stream2 >> info.send_id;
        stream2 >> info.recv_id;
        stream2 >> info.serviceType;
        stream2 >> info.serverPath;
        stream2 >> info.serverFileName;
        stream2 >> info.timestamp;
        info.ClientPath = std::string("D:/BaiduNetdiskDownload/untitled2/userInfo/") + info.serverFileName;
        info.print();
        FtpSender::GetInstance()->addFile(info);
        FtpSender::GetInstance()->GetFile(info);
        //当获取后还需要提醒窗口已下载完成

        QString str = mFriendButton[info.send_id]->text();
        int index = str.indexOf("[New");
        if (index != -1)
            str = str.remove(index, str.length() - index);
        //有新消息来时添加text,点击后，出现新消息后，删除text
        mFriendButton[info.send_id]->setText(str + "  [New Message]");

        //这个ChatWindow一定一定已经在上面一个函数里创建了
        mChatWindowMap[info.send_id]->addFileArrive(info);
    }
}

void FriendPage::friendPageUpdate(int uid) {
    fflush(stdout);
    QString str = mFriendButton[uid]->text();
    int index = str.indexOf("[New");
    if (index != -1)
        str = str.remove(index, str.length() - index);
    //有新消息来时添加text,点击后，出现新消息后，删除text
    mFriendButton[uid]->setText(str);
}


void FriendPage::getAllFriendRequestSuccess(Response response) {
    //TODO:右下角图标添加几个提示
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        FriendRequest info;
        stream2 >> info.sender_id >> info.username >> info.state;
        info.reciver_id = mUserId;
        friendMessageArrive(info);
        //addFriendToPage(i, info.username);//
        printf("user_id: %d username: %s state %d\n", info.sender_id, info.username.c_str(), info.state);
        fflush(stdout);
    }
    if (size != 0) {
    }
}

void FriendPage::UpDateUserStateSuccess(Response response) {
    if (response.mCode) {
        QMessageBox::information(this,"提示","修改状态成功！");
    }
}

void FriendPage::MessageArriveClient(Response response)
{
    //在线消息到达
    std::string &mData = response.mData;
    MyProtocolStream stream(mData);
    MessageInfo info;
    stream >> info.id >> info.sender_id >> info.receiver_id >> info.timestamp >> info.message_text;
    QString str = mFriendButton[info.sender_id]->text();
    int index = str.indexOf("[New");
    if (index != -1)
        str = str.remove(index, str.length() - index);
    //有新消息来时添加text,点击后，出现新消息后，删除text
    mFriendButton[info.sender_id]->setText(str + "  [New Message]");

    mChatWindowMap[info.sender_id]->addMessage(info);
}

FriendPage::~FriendPage()
{
    delete ui;
}

void FriendPage::setReturn(QWidget *widget)
{
    this->returnWindow = widget;
}

void FriendPage::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) // 当按下Return/Enter键时
    {
        hide();
        returnWindow->show();
    }
    else
    {
        // 继续处理其他按键事件
        QWidget::keyPressEvent(event);
    }
}

bool FriendPage::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            QToolButton *button = qobject_cast<QToolButton *>(obj);
            if (button)
            {
                // 获取按钮在父控件坐标系中的矩形区域
                QRect buttonRect = button->rect();

                // 获取鼠标事件的位置
                QPoint mousePos = mouseEvent->pos();
                qDebug() << buttonRect.x() << " " << buttonRect.y() << " " << buttonRect.width() << " " << buttonRect.height();
                qDebug() << mousePos.x() << " " << mousePos.y();
                // 判断鼠标点击位置是否在按钮的矩形区域内
                if (buttonRect.contains(mousePos))
                {
                    // 处理点击事件
                    emit chatWithFriend(ui->listWidget->itemAt(button->pos()));
                    // 标记事件已处理
                    event->accept();
                    return true; // 表示事件已处理
                }
            }
        }
    }
    return false; // 没有处理该事件，继续传递
}


void FriendPage::addFriendToPage(int i, UserInfo info) {
    QToolButton* button = new QToolButton();
    QString str = QString::fromStdString(info.username);
    str += QString::number(i);
    button->setText(str);
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QIcon userIcon(QPixmap(":/friend/touxiang.jpeg"));
    button->setIcon(userIcon);
    //button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    button->setMinimumHeight(60);
    button->setMinimumWidth(60);
    button->setIconSize(QSize(60, 60));
    QString str2("item");
    str2 += QString::number(i);
    ui->listWidget->addItem(str2);
    ui->listWidget->setItemWidget(ui->listWidget->item(i), button);
    ui->listWidget->item(i)->setWhatsThis(QString::number(info.user_id));
    qDebug() << "Hello XXXXX!" << button;
    //installEventFilter + eventFilter overide可以实现防覆
    button->installEventFilter(this); // 在当前类中实现 eventFilter 方法
    mFriendButton[info.user_id] =button;
    /*
    QLabel* button = new QLabel();
    QString str("朋友");
    str += QString::number(i);
    QString str2("item");
    str2 += QString::number(i);
    ui->listWidget->addItem(str2);
    ui->listWidget->setItemWidget(ui->listWidget->item(i), button);

    */


    // 连接信号和槽
    connect(ui->listWidget, &QListWidget::itemClicked, this, &FriendPage::chatWithFriend);

    //    connect(button, &QToolButton::clicked, this, [this]() {
    //        qDebug() << "index: " << i;
    //        ChatWindow* chatWindow = new ChatWindow;
    //        chatWindow->returnWindow = this;
    //        chatWindow->show();
    //        this->hide();
    //    });

    QSize size;
    size.setHeight(60);
    size.setWidth(60);
    ui->listWidget->item(i)->setSizeHint(size);
    //ui->listWidget->setMinimumHeight(60);
}

void FriendPage::chatWithFriend(QListWidgetItem* item) {
    //需要根据是哪个item获取是哪个chatWindow
    int userId = item->whatsThis().toInt();
    ChatWindow* chatWindow = mChatWindowMap[userId];
    chatWindow->returnWindow = this;
    chatWindow->show();
    chatWindow->showChatContent();
}

void FriendPage::on_toolButton_4_clicked()
{
    UserInfo info;
    info.username = "朋友";
    addFriendToPage(m_index, info);
    m_index++;
}


void FriendPage::on_toolButton_clicked()
{
    if (m_FindFriendPage == NULL) {
        m_FindFriendPage = new FindFriendPage;
        m_FindFriendPage->returnWindow = this;
    }
    m_FindFriendPage->show();
    this->hide();
}


void FriendPage::on_toolButton_3_clicked()
{
    if (m_CreateGroupPage == NULL) {
        m_CreateGroupPage = new CreateGroupPage;
        m_CreateGroupPage->returnWindow = this;
    }
    m_CreateGroupPage->show();
    this->hide();
}

void FriendPage::on_comboBox_currentIndexChanged(int index)
{
    //UpdateUserState
    Processor::ChangeUserState(index + 1);
}

void FriendPage::friendMessageArrive(FriendRequest info)
{
    mFriendRequestSet.insert(info);
    //按钮抖动
    // 创建定时器
    return;
    if (mFriendRequestTimer == NULL) {
        mFriendRequestTimer = new QTimer(this);
        // 设置定时器超时时间，例如500毫秒
        mFriendRequestTimer->setInterval(500);

        // 连接定时器到一个槽函数
        QToolButton* button = ui->toolButton_2;
        connect(mFriendRequestTimer, &QTimer::timeout, [button]() {
            button->setVisible(!button->isVisible());
        });
    }
    // 启动定时器
    mFriendRequestTimer->start();
}

void FriendPage::on_toolButton_2_clicked()
{
    if (mFriendRequestSet.size() == 0) {
        return;
    }
    FriendRequest request =  *mFriendRequestSet.begin();
    QString str;
    QMessageBox::StandardButton reply;
    str = "是否通过来自朋友" + QString::fromStdString(request.username) + "的好友请求";
    reply = QMessageBox::question(nullptr, "Confirmation", str, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // 用户点击了 "Yes" 按钮
        // 执行相应的操作
        request.mAccept = true;
        Processor::processFriendRequest(request);
    } else {
        // 用户点击了 "No" 按钮，或者关闭了对话框
        // 执行相应的操作，或者什么都不做
        request.mAccept = false;
        Processor::processFriendRequest(request);
    }
    mFriendRequestSet.erase(request);
//    if (mFriendRequestSet.size() == 0) {
//        mFriendRequestTimer->stop();
    //    }
}

//服务器告知客户端有文件传输而来，客户端大概准备接收文件
void FriendPage::NofifyFileComing(Response response)
{
    //在线 离线文件到达
    std::string &mData = response.mData;
    MyProtocolStream stream(mData);
    FileInfo info;
    info.Generate();
    stream >> info.ftpTaskId >> info.id >> info.send_id >> info.recv_id >> info.serverPath >> info.serverFileName >> info.filesize >> info.fileType;
    info.print();
    //判断是否接收
    info.ClientPath = std::string("D:/BaiduNetdiskDownload/untitled2/userInfo/") + info.serverFileName;
    info.filesize = 10;
    int ret = false;
    if (info.filesize < 1000 * 1000 * 10) {
        FtpSender::GetInstance()->addFile(info);
        Processor::AgreeRecvFile(true, info);
        ret =true;
    }
    else {
        ret = QMessageBox::information(this, "提示", "是否接收", QMessageBox::Yes | QMessageBox::No);
        if (ret) {
            FtpSender::GetInstance()->addFile(info);
            Processor::AgreeRecvFile(true, info);
        }
        else {
            Processor::AgreeRecvFile(false, info);
        }
    }

    if (ret) {
        //界面显示，具体怎么提示是否接收，怎么显示后续再看...
        QString str = mFriendButton[info.send_id]->text();
        int index = str.indexOf("[New");
        if (index != -1)
            str = str.remove(index, str.length() - index);
        //有新消息来时添加text,点击后，出现新消息后，删除text
        mFriendButton[info.send_id]->setText(str + "  [New Message]");

        mChatWindowMap[info.send_id]->addFileArrive(info);
        //FtpSender::GetInstance()->GetFile(info);
    }
}


void FriendPage::ChangeOwnerPic()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/userPhoto/tx" + QString::fromStdString(mInfo.username) + ".jpg";
    qDebug() << "修改个人头像，当前工作目录:" << filePath;
    if (QFile::exists(filePath)) {
        // 如果文件存在，加载图像并设置为头像
        QIcon icon(filePath);
        ui->toolButton_5->setIcon(icon);
        ui->toolButton_5->setIconSize(QSize(50, 50));
    } else {
        // 文件不存在，给用户提示
        QMessageBox::warning(nullptr, "警告", "头像文件不存在。");
    }
}

void FriendPage::GetFileFirstSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int ret = response.mCode;
    if (ret) {
        int ftptaskId = -1;
        stream2 >> ftptaskId;
        FileInfo info = FtpSender::GetInstance()->file(ftptaskId);//这样缓存了clientPath和clientFileName
        stream2 >> info.id >> info.serverPath >> info.serverFileName >> info.filesize;
        info.print();
        info.filesize = 10;
        if (info.filesize < 10 * 1024 * 1024) {
            qDebug() << "Will Ftp Get .............................";
            FtpSender::GetInstance()->GetFile(info);//ftp获取队列异步获取
        }
        else {
            //询问，或者阻止
        }
    }
    else {
        QMessageBox::information(this, "提示", "Server is not allow GetFile");
    }
}

//服务器可返回，也可不返回
void FriendPage::GetFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    bool ret = response.mCode;
    if (ret) {
        qDebug() << "GetFile Over";
    }
    else {
        qDebug() << "GetFile Failure";
    }
}
//服务器可返回，也可不返回
void FriendPage::SendFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    bool ret = response.mCode;
    if (ret) {
        QMessageBox::information(this, "提示", "SendFile %s Over");
    }
    else {
        QMessageBox::information(this, "提示", "SendFile %s Send Failure");
    }
}

//头像按钮,上传头像
void FriendPage::on_toolButton_5_clicked()
{
    // 弹出文件选择对话框
    /*
        tr("Open Image"),
            "",
            tr("Images (*.png *.jpg *.jpeg *.bmp *.gif);;All Files (*)"));
    */
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),
        "",
        tr("All Files (*);;Text Files (*.txt)"));
    //D:/easybcd.zip
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
        return;
    }

    FileInfo info;
    info.Generate();
    info.ClientPath = fileName.toStdString();
    info.serverPath = "userPhoto/";
    info.serviceType = FileServerType::TOUXIANG;
    info.serverFileName = "tx" + mInfo.username + ".jpg";
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::SendFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
}

void FriendPage::StartUpLoadFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    //Response: 告知允许上传，及部分参数
    int ret = response.mCode;
    if (ret) {
        int ftpTaskId = -1;
        stream2 >> ftpTaskId;
        FileInfo info = FtpSender::GetInstance()->file(ftpTaskId);
        stream2 >> info.id >> info.serverPath >> info.serverFileName;
        FtpSender::GetInstance()->SendFile(info);//ftp发送队列异步发送
    }
    else {
        QMessageBox::information(this, "提示", "Server is not allow SendFile");
    }
}
