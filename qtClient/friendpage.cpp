#include "friendpage.h"
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
#include "findfriendpage.h"
#include "creategrouppage.h"
#include "chatwindow.h"
#include "userbutton.h"
#include "network.h"
#include "processor.h"

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

    connect(ClientNetWork::GetInstance(), &ClientNetWork::findAllFriendSuccess, this, &FriendPage::findAllFriendSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllMessageSuccess, this, &FriendPage::getAllMessageSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllFriendRequestSuccess, this, &FriendPage::getAllFriendRequestSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::UpDateUserStateSuccess, this, &FriendPage::UpDateUserStateSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ReciveMessageSuccess, this, &FriendPage::ReciveMessageSuccess);
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

void FriendPage::findAllFriendSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    qDebug() << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        UserInfo info;
        stream2 >> info.user_id >> info.friendStatus >> info.username >> info.email;
        addFriendToPage(i, info);//
        info.print();
        mFriendList.push_back(info);
        mChatWindowMap[info.user_id] = new ChatWindow(info);
        connect(this, &FriendPage::userMessageUnRead, mChatWindowMap[info.user_id], &ChatWindow::userMessageRead);
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
        //这个ChatWindow一定一定已经在上面一个函数里创建了
        mChatWindowMap[info.sender_id]->addMessage(info);

        info.print();
        QString str = mFriendButton[info.sender_id]->text();
        int index = str.indexOf("[New");
        if (index != -1)
            str = str.remove(index, str.length() - index);
        //有新消息来时添加text,点击后，出现新消息后，删除text
        mFriendButton[info.sender_id]->setText(str + "  [New Message]");
    }
}

void FriendPage::friendPageUpdate(int uid) {
    QString str = mFriendButton[uid]->text();
    int index = str.indexOf("[New");
    if (index != -1)
        str = str.remove(index);
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

void FriendPage::ReciveMessageSuccess(Response response)
{
    std::string &mData = response.mData;
    MessageInfo info;
    //TODO: stream 应该重载>>(MessageInfo\UserInfo...)
    if (response.mhasData) {
        MyProtocolStream stream(mData);
        stream >> info.id >> info.sender_id >> info.message_text >> info.timestamp;
        int x = mChatWindowMap[info.sender_id]->windowState();
        if (mChatWindowMap[info.sender_id]->windowState() == Qt::WindowNoState) {
            printf("iiiiiiiiiiiiiiiiiiiiiiii  %d  \n", x);
        }
        else {
            printf("iiiiiiiiiiiiiiiiiiiiiiii  %d  \n", x);
        }
        info.print();
        mChatWindowMap[info.sender_id]->addMessage(info);
        QString str = mFriendButton[info.sender_id]->text();
        int index = str.indexOf("[New");
        if (index != -1)
            str = str.remove(index, str.length() - index);
        //有新消息来时添加text,点击后，出现新消息后，删除text
        mFriendButton[info.sender_id]->setText(str + "  [New Message]");
        //如果ChatWindow已经打开：
        //如果ChatWindow没有打开：
    }
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
    //ChatWindow* chatWindow = new ChatWindow;
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    printf("userId = %d\n", userId);
    ChatWindow* chatWindow = mChatWindowMap[userId];
    chatWindow->returnWindow = this;
    chatWindow->messageUpdate();
    chatWindow->show();
    chatWindow->showChatContent();
    this->hide();
    qDebug() << " hello!" << item;
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

