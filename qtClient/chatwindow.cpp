#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "processor.h"
#include "network.h"
#include "friendpage.h"
#include "ftpsender.h"
#include <QDateTime>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "emojiccoder.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    returnWindow = NULL;
    mUserId = 1;
    ui->plainTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑内容
    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
    connect(this, &ChatWindow::friendPageUpdate, (FriendPage*)this->parent(), &FriendPage::friendPageUpdate);

    //这两个应该放在网络线程（通用 通信确认业务），但同时，发送完文件也应该通知聊天框，看具体业务。
    //connect(FtpSender::GetInstance(), &FtpSender::ftpFileSendOver, this, &ChatWindow::ftpSendFileSuccess);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::offlineTransFileSuccess, this, &ChatWindow::offlineTransFileSuccess);

    QObject::connect(ui->plainTextEdit_2, &QTextEdit::cursorPositionChanged, this, &ChatWindow::handleCursorPositionChange);
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
        ui->plainTextEdit->append(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(mUnReadMessageList[i].timestamp));
        ui->plainTextEdit->append(QString::fromStdString(mUnReadMessageList[i].message_text));
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
        ui->plainTextEdit->append(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(info.timestamp));
        ui->plainTextEdit->append(QString::fromStdString(info.message_text));
        mEmojiCoder->explain(ui->plainTextEdit, QString::fromStdString(info.message_text));
        messageUpdate();
    }
    else {
    mUnReadMessageList.push_back(info);
    }
}



void ChatWindow::addFileArrive(FileInfo info)
{
//    if (this->isVisible()) {
//        printf("isVis");
//        fflush(stdout);
//        mCurrentMessageList.push_back(info);
//        ui->plainTextEdit->appendPlainText(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(info.timestamp));
//        ui->plainTextEdit->appendPlainText(QString::fromStdString(info.message_text));
//        messageUpdate();
//    }
//    else {
//    mUnReadMessageList.push_back(info);
//    }
    MessageInfo infom;
    infom.message_text = "[" + info.serverFileName + " Arrive in " + info.ClientPath + "]";
    if (this->isVisible()) {
        printf("isVis");
        fflush(stdout);
        mCurrentMessageList.push_back(infom);
        ui->plainTextEdit->append(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(infom.timestamp));
        ui->plainTextEdit->append(QString::fromStdString(infom.message_text));
        messageUpdate();
    }
    else {
        mUnReadMessageList.push_back(infom);
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
            ui->plainTextEdit->append(QString::fromStdString(name) + "           " + currentDateTimeStr);
            ui->plainTextEdit->append(s);
            ui->plainTextEdit->append("Trans failure......Try Again\n");
            return;
        }
        QString s = QString::fromStdString(content);
        ui->plainTextEdit->append(QString::fromStdString(name) + "           " + currentDateTimeStr);
        ui->plainTextEdit->append(s);
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



//发送给好友文件
void ChatWindow::on_toolButton_5_clicked()
{
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
    info.serverPath = std::to_string(user_id) + string("/");
    QFileInfo fileInfo(QString::fromStdString(info.ClientPath));
    // 获取文件名（包括扩展名）
    info.serverFileName = fileInfo.fileName().toStdString();
    info.send_id = user_id;
    info.recv_id = this->mInfo.user_id;
    info.serviceType = FileServerType::SENDTOPERSON;
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::SendFile(info);
}
/*
QFileInfo fileInfo(filePath);

    // 获取文件名（包括扩展名）
    QString fileName = fileInfo.fileName();
    qDebug() << "File name with extension:" << fileName;

    // 获取文件名（不包括扩展名）
    QString baseName = fileInfo.baseName();
    qDebug() << "File name without extension:" << baseName;

    // 获取文件扩展名
    QString extension = fileInfo.suffix();
*/

void ChatWindow::on_toolButton_8_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!imagePath.isEmpty()) {
        QTextCursor cursor = ui->plainTextEdit_2->textCursor();
        cursor.insertImage(imagePath);
    }
}

void ChatWindow::on_toolButton_9_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        // 插入文件名作为超链接
        QTextCursor cursor = ui->plainTextEdit_2->textCursor();
        cursor.movePosition(QTextCursor::End); // 移动到末尾
        cursor.insertText("File: "); // 插入文件前缀

        // 设置超链接格式
        QTextCharFormat linkFormat;
        linkFormat.setForeground(Qt::blue); // 设置字体颜色为蓝色
        linkFormat.setFontUnderline(true); // 设置字体为下划线

        // 插入可点击的文件名
        cursor.insertText(QFileInfo(filePath).fileName(), linkFormat);
        cursor.insertText("\n"); // 插入换行
        ui->plainTextEdit_2->setTextCursor(cursor); // 更新光标位置
    }
}


void ChatWindow::handleCursorPositionChange() {
//    // 处理文本框中的点击事件
//    QTextCursor cursor = ui->plainTextEdit_2->textCursor();
//    if (cursor.hasSelection()) {
//        return; // 如果有选中内容，不处理
//    }

//    QTextCharFormat format = cursor.charFormat();
//    if (format.foreground().color() == Qt::blue && format.fontUnderline()) {
//        QString fileName = cursor.selectedText();
//        QString filePath = QFileDialog::getOpenFileName(this, "Open File", fileName, "All Files (*)");
//        if (!filePath.isEmpty()) {
//            // 在这里可以处理打开文件的逻辑
//            // 例如，打开文件或执行其他操作
//            // QFile file(filePath);
//            // 进行读取或其他操作
//        }
//    }
}
