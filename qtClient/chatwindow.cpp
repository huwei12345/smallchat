#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "processor.h"
#include "network.h"
#include "friendpage.h"
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

void ChatWindow::messageUpdate() {
    int smallNum = INT_MAX;
    int bigNum = 0;
    for (int i = 0; i < mUnReadMessageList.size(); i++) {
        ui->plainTextEdit->appendPlainText(QString::fromStdString(mUnReadMessageList[i].message_text));
        smallNum = min(smallNum, mUnReadMessageList[i].id);
        bigNum = max(bigNum, mUnReadMessageList[i].id);
    }
    emit confirmMessage(mUserId, smallNum, bigNum);//网络发送给服务器确认
    emit friendPageUpdate(mUserId);
}

void ChatWindow::addMessage(MessageInfo info)
{
    mUnReadMessageList.push_back(info);
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

void ChatWindow::on_pushButton_clicked()
{
    std::string content = ui->plainTextEdit_2->toPlainText().toStdString();
    if (content != "") {
        bool ret = Processor::SendMessage(mUserId, content);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
            QString s = QString::fromStdString(content);
            ui->plainTextEdit->appendPlainText(s);
            ui->plainTextEdit->appendPlainText("Trans failure......\n");
            return;
        }
        QString s = QString::fromStdString(content);
        ui->plainTextEdit->appendPlainText(s);
        ui->plainTextEdit->appendPlainText("Trans......\n");
    }
}

void ChatWindow::userMessageRead()
{
    if (mUnReadMessageList.size() > 0) {
        //闪烁
    }
}
