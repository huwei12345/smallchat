#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "processor.h"
#include "network.h"
#include "friendpage.h"
#include <QDateTime>
#include <QMessageBox>
ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    returnWindow = NULL;
    mUserId = 1;
    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
    connect(this, &ChatWindow::friendPageUpdate, (FriendPage*)this->parent(), &FriendPage::friendPageUpdate);
}

ChatWindow::ChatWindow(UserInfo info, QWidget *parent) :
    ChatWindow(parent)
{
    mInfo = info;
    mUserId = info.user_id;
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

//TOSEE:屏幕所见才算消息到达，可以ack,否则下次登录还按未收到消息从服务器发送
void ChatWindow::messageUpdate() {
    int smallNum = INT_MAX;
    int bigNum = 0;
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
        smallNum = min(smallNum, mUnReadMessageList[i].id);
        bigNum = max(bigNum, mUnReadMessageList[i].id);
    }
    emit confirmMessage(mUserId, ::user_id, smallNum, bigNum);//网络发送给服务器确认收到【start，end】的消息
    emit friendPageUpdate(mUserId);//让页面上头像停止抖动，切换为无消息状态
}
//非显示 切换 显示
void ChatWindow::showChatContent()
{
    printf("size = %d\n", mUnReadMessageList.size());
    for (int i = 0; i < mUnReadMessageList.size(); i++) {
        ui->plainTextEdit->appendPlainText(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(mUnReadMessageList[i].timestamp));
        ui->plainTextEdit->appendPlainText(QString::fromStdString(mUnReadMessageList[i].message_text));
        mCurrentMessageList.push_back(mUnReadMessageList[i]);
    }
    messageUpdate();
    mUnReadMessageList.clear();
}

void ChatWindow::addMessage(MessageInfo info)
{
    //单条消息，显示 切换 显示
    if (this->isVisible()) {
        printf("isVis");
        fflush(stdout);
        mCurrentMessageList.push_back(info);
        ui->plainTextEdit->appendPlainText(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(info.timestamp));
        ui->plainTextEdit->appendPlainText(QString::fromStdString(info.message_text));
        messageUpdate();
    }
    else {
        mUnReadMessageList.push_back(info);
    }
}

void ChatWindow::keyPressEvent(QKeyEvent *event)
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

//发送消息
void ChatWindow::on_pushButton_clicked()
{
    std::string content = ui->plainTextEdit_2->toPlainText().toStdString();
    if (content != "") {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        std::string name = ((FriendPage*)returnWindow)->mInfo.username;
        // 将日期和时间转换为字符串
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        MessageInfo info;
        info.receiver_id = mInfo.user_id;
        info.sender_id = mUserId;
        info.timestamp = currentDateTimeStr.toStdString();
        //此处应优化为以info为参数
        bool ret = Processor::SendMessage(mUserId, content);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
            QString s = QString::fromStdString(content);
            ui->plainTextEdit->appendPlainText(QString::fromStdString(name) + "           " + currentDateTimeStr);
            ui->plainTextEdit->appendPlainText(s);
            ui->plainTextEdit->appendPlainText("Trans failure......Try Again\n");
            return;
        }
        QString s = QString::fromStdString(content);
        ui->plainTextEdit->appendPlainText(QString::fromStdString(name) + "           " + currentDateTimeStr);
        ui->plainTextEdit->appendPlainText(s);
        mCurrentMessageList.push_back(info);
    }
}

void ChatWindow::userMessageRead()
{
    if (mUnReadMessageList.size() > 0) {
        //闪烁
    }
}
