#include "groupchatwindow.h"
#include "ui_groupchatwindow.h"

#include <QIcon>
#include <QKeyEvent>
#include "processor.h"

GroupChatWindow::GroupChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChatWindow)
{
    ui->setupUi(this);
    returnWindow = NULL;
    mUserId = 1;
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));

//    ui->plainTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑内容
//    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
//    connect(this, &ChatWindow::resetFriendNewMessage, (FriendPage*)this->parent(), &FriendPage::resetFriendNewMessage);

//    //这两个应该放在网络线程（通用 通信确认业务），但同时，发送完文件也应该通知聊天框，看具体业务。
//    //connect(FtpSender::GetInstance(), &FtpSender::ftpFileSendOver, this, &ChatWindow::ftpSendFileSuccess);
//    QObject::connect(ui->plainTextEdit, &QTextEdit::cursorPositionChanged, this, &ChatWindow::handleCursorPositionChange);

//    mEmojiSelector = new EmojiSelector;
//    connect(mEmojiSelector, &EmojiSelector::emojiSelected, this, &ChatWindow::emojiSelected);
//    connect(this, &ChatWindow::sendFiletoPersonSuccess, this, &ChatWindow::sendFiletoPersonSucc);


    // 创建一个垂直布局
//    QWidget* widget = ui->widget_2;

//    // 创建一个 QSplitter
//    QSplitter *splitter = new QSplitter(Qt::Vertical);

//    // 添加左侧和右侧的部件
//    QWidget *upWidget = ui->plainTextEdit;
//    QWidget *downWidget = ui->widget_5;
//    ui->widget_5->setMinimumWidth(80); // 设置最小宽度
//    // 将部件添加到分隔器
//    splitter->addWidget(upWidget);
//    splitter->addWidget(downWidget);

//    // 设置分隔条的初始大小
//    splitter->setSizes(QList<int>({240, 150}));  // 左右部件的初始宽度

//    // 将分隔器添加到布局
//    QLayout* layout = widget->layout();
//    layout->addWidget(splitter);
    // 设置主窗口的布局
}

GroupChatWindow::GroupChatWindow(GroupInfo info, QWidget *parent) :
    GroupChatWindow(parent)
{
    mInfo = info;
    mUserId = user_id;

//    QString labelText = ui->label->text();
//    labelText += QString::fromStdString(mInfo.username);
//    ui->label->setText(labelText);
}

GroupChatWindow::~GroupChatWindow()
{
    delete ui;
}

bool GroupChatWindow::init()
{
    //初始化群成员列表

    //初始化群消息

    //初始化通知

    //初始化群文件列表

    //持久化本地

    return true;
}

void GroupChatWindow::keyPressEvent(QKeyEvent *event)
{
    // 当按下Return/Enter键时
    if(event->key() == Qt::Key_Escape) {
        hide();
        returnWindow->show();
    }
    else {
        // 继续处理其他按键事件
        QWidget::keyPressEvent(event);
    }
}
