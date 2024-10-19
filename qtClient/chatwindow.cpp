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
#include <QPropertyAnimation>
#include <QSplitter>
#include <cmath>
#include "emojiselector.h"
#include "clientpersoninfo.h"
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
    mEmojiSelector = new EmojiSelector;
    connect(ClientNetWork::GetInstance(), &ClientNetWork::offlineTransFileSuccess, this, &ChatWindow::offlineTransFileSuccess);

    QObject::connect(ui->plainTextEdit_2, &QTextEdit::cursorPositionChanged, this, &ChatWindow::handleCursorPositionChange);

    connect(mEmojiSelector, &EmojiSelector::emojiSelected, this, &ChatWindow::emojiSelected);
    connect(this, &ChatWindow::sendFiletoPersonSuccess, this, &ChatWindow::sendFiletoPersonSucc);


    // 创建一个垂直布局
    QWidget* widget = ui->widget_2;

    // 创建一个 QSplitter
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    // 添加左侧和右侧的部件
    QWidget *upWidget = ui->plainTextEdit;
    QWidget *downWidget = ui->widget_5;

    // 将部件添加到分隔器
    splitter->addWidget(upWidget);
    splitter->addWidget(downWidget);

    // 设置分隔条的初始大小
    splitter->setSizes(QList<int>({240, 150}));  // 左右部件的初始宽度

    // 将分隔器添加到布局
    QLayout* layout = widget->layout();
    layout->addWidget(splitter);
    // 设置主窗口的布局
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
        showContentWithImages(QString::fromStdString(mUnReadMessageList[i].message_text));

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
        showContentWithImages(QString::fromStdString(info.message_text));
        messageUpdate();
    }
    else {
        mUnReadMessageList.push_back(info);
    }
}



void ChatWindow::addFileArrive(FileInfo info)
{
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

bool ChatWindow::sendMessage(const QString &content) {
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
        bool ret = Processor::SendMessage(mUserId, content.toStdString());
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
            ui->plainTextEdit->append(QString::fromStdString(name) + "           " + currentDateTimeStr);
            showContentWithImages(content);
            ui->plainTextEdit->append("Trans failure......Try Again\n");
            return false;
        }
        ui->plainTextEdit->append(QString::fromStdString(name) + "           " + currentDateTimeStr);
        showContentWithImages(content);
        mCurrentMessageList.push_back(info);
    }
    return true;
}

//发送消息
void ChatWindow::on_pushButton_clicked()
{
    QString content = mEmojiSelector->extractContentWithImages(ui->plainTextEdit_2);
    sendMessage(content);
}



//抖动窗口
void ChatWindow::sharkWindow() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(200);
    animation->setLoopCount(3);
    animation->setKeyValueAt(0, this->geometry());
    int x = this->x();
    int y = this->y();
    for (int i = 1; i <= 4; ++i) {
        animation->setKeyValueAt(0.25 * i, QRect(x + 2 * pow(-1, i), y + 10 * i, this->width(), this->height()));
        //animation->setKeyValueAt(0.5 * i + 0.25, QRect(button->x() + 5 * i, button->y(), button->width(), button->height()));
    }
    animation->start();
}

//解析表情
void ChatWindow::showContentWithImages(QString s) {
    ui->plainTextEdit->append("");
    if (s == "0xa500") {
        ui->plainTextEdit->insertPlainText("抖动窗口！");
        sharkWindow();
        return;
    }
    mEmojiSelector->showContentWithImages(ui->plainTextEdit, s);
}

void ChatWindow::offlineTransFileSuccess(Response rsp)
{
    Q_UNUSED(rsp);
}

void ChatWindow::userMessageRead()
{
    if (mUnReadMessageList.size() > 0) {
        //闪烁
    }
}

void ChatWindow::emitSendFiletoPerson(FileInfo info) {
    emit sendFiletoPersonSuccess(info);
}

void ChatWindow::sendFiletoPersonSucc(FileInfo info) {

    if (info.fileType == "PicTure") {
        //TODO:调试
        //    if (!imagePath.isEmpty()) {
        //        QTextCursor cursor = ui->plainTextEdit_2->textCursor();
        //        cursor.insertImage(imagePath);
        //    }
        QString imagePath = QString::fromStdString(info.ClientPath);
        if (!imagePath.isEmpty()) {
            // 插入文件名作为超链接
            QTextCursor cursor = ui->plainTextEdit->textCursor();
            cursor.movePosition(QTextCursor::End); // 移动到末尾
            cursor.insertText("File: "); // 插入文件前缀

            // 设置超链接格式
            QTextCharFormat linkFormat;
            linkFormat.setForeground(Qt::blue); // 设置字体颜色为蓝色
            linkFormat.setFontUnderline(true); // 设置字体为下划线
            cursor.insertText(QFileInfo(imagePath).fileName(), linkFormat);
            cursor.insertText("\n"); // 插入换行

            // 插入可点击的文件名
            QTextImageFormat format;
            format.setName(imagePath);
            format.setWidth(80);
            format.setHeight(80);
            format.setToolTip(imagePath);
            cursor.insertImage(format);
            // 插入可点击的文件名
            ui->plainTextEdit->setTextCursor(cursor); // 更新光标位置
        }
        return;
    }

    MessageInfo infom;
    infom.message_text = "[" + info.ClientPath + " Send Success " + "]";
    if (this->isVisible()) {
        printf("isVis");
        fflush(stdout);
        mCurrentMessageList.push_back(infom);
        //TODO:修改显示
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        ui->plainTextEdit->append(QString::fromStdString(ClientPersonInfo::GetInstance()->username) + "           " + currentDateTimeStr);
        ui->plainTextEdit->append(QString::fromStdString(infom.message_text));
        messageUpdate();
    }
    else {
        mUnReadMessageList.push_back(infom);
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
    info.fileType = "PicTure";
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::SendFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }

    MessageInfo infom;
    infom.message_text = "[" + info.ClientPath + " Send Begin... " + "]";
    if (this->isVisible()) {
        printf("isVis");
        fflush(stdout);
        mCurrentMessageList.push_back(infom);
        //TODO:修改显示
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        ui->plainTextEdit->append(QString::fromStdString(ClientPersonInfo::GetInstance()->username) + "           " + currentDateTimeStr);
        ui->plainTextEdit->append(QString::fromStdString(infom.message_text));
        messageUpdate();
    }
    else {
        mUnReadMessageList.push_back(infom);
    }
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

//发送图片
void ChatWindow::on_toolButton_8_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    //D:/easybcd.zip
    if (!imagePath.isEmpty()) {
        qDebug() << "Selected file:" << imagePath;
    } else {
        qDebug() << "No file selected.";
        return;
    }
    FileInfo info;
    info.Generate();
    info.ClientPath = imagePath.toStdString();
    info.serverPath = std::to_string(user_id) + string("/");
    QFileInfo fileInfo(QString::fromStdString(info.ClientPath));
    // 获取文件名（包括扩展名）
    info.serverFileName = fileInfo.fileName().toStdString();
    info.send_id = user_id;
    info.recv_id = this->mInfo.user_id;
    info.serviceType = FileServerType::SENDTOPERSON;
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::SendFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return;
}

//文件2
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


//从edit里读取出QString,然后在位置中加入表情的代号如#0xa301，然后清空edit，重新绘制，
//绘制的时候如果是字符直接写，如果是表情，那么替换为表情图片。
void ChatWindow::emojiSelected(QString emoji) {
    mEmojiSelector->explain(ui->plainTextEdit_2, emoji);
}

void ChatWindow::on_toolButton_4_clicked()
{
    mEmojiSelector->exec();
}

//抖动窗口
void ChatWindow::on_toolButton_7_clicked()
{
    sendMessage("0xa500");
}
