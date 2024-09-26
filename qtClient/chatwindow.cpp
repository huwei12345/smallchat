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
connect(ClientNetWork::GetInstance(), &ClientNetWork::StartUpLoadFileSuccess, this, &ChatWindow::StartUpLoadFileSuccess);

    connect(ftpSender::GetInstance(), ftpSender::ftpFileSendOver, this, &ChatWindow::ftpSendFileSuccess);
    connect(ClientNetWork::GetInstance(), ClientNetWork::UpLoadFileSuccess, this, &ChatWindow::UpLoadFileSuccess);
    connect(ClientNetWork::GetInstance(), ClientNetWork::GetFileFirstSuccess, this, &ChatWindow::GetFileFirstSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::offlineTransFileSuccess, this, &ChatWindow::offlineTransFileSuccess);
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
    printf("size = %d\n", (int)mUnReadMessageList.size());
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
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

void ChatWindow::offlineTransFileSuccess(Response rsp)
{

}

void ChatWindow::userMessageRead()
{
    if (mUnReadMessageList.size() > 0) {
        //闪烁
    }
}
void ChatWindow::StartUpLoadFileSuccess(Response rsp)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    //Response: 告知允许上传，及部分参数
    int ret = rsp.mCode;
    std::string filePath;
    std::string fileName;
    stream2 >> filePath >> fileName;
    if (ret) {
        stream2 >> filePath >> fileName;
        ftp->GetInstance()->SendFile(filePath, fileName);//ftp发送队列异步发送
    }
    else {
        QMessageBox::Information(this, "提示", "Server is not allow SendFile");
    }
}


void ChatWindow::ftpSendFileSuccess(string filename)
{
    bool ret = Processor::SendMessageSuccess(filename);
}


void ChatWindow::UpLoadFileSuccess(Response rsp)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    ret = rsp.mCode;
    if (ret) {
        QMessageBox::information(this, "提示", "SendFile %s Over");
    }
    else {
        QMessageBox::information(this, "提示", "SendFile %s Send Failure");
    }
}

void ChatWindow::GetFileFirstSuccess(Response rsp)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int ret = rsp.mCode;
    FileInfo info;
    std::string filePath;
    std::string fileName;
    stream2 >> info.path >> info.filename >> info.filesize;
    if (ret) {
        if (info.filesize < 10 * 1024 * 1024) {
            ftp->GetInstance()->GetFile(filePath, fileName);//ftp获取队列异步获取
        }
        else {
            //询问，或者阻止
        }
    }
    else {
        QMessageBox::Information(this, "提示", "Server is not allow GetFile");
    }
}


void ChatWindow::on_toolButton_5_clicked()
{
    std::string Remotepath = "/";
    std::string filename = "./a.txt";
    bool ret = Processor::SendFile(Remotepath, filename);
}