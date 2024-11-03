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
#include <QDesktopServices>
#include <QTextBlock>
#include "emojiselector.h"
#include "clientpersoninfo.h"
#include "personcache.h"
#include "personcardcon.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    returnWindow = NULL;
    mUserId = 1;
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));

    ui->plainTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑内容
    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
    connect(this, &ChatWindow::resetFriendNewMessage, (FriendPage*)this->parent(), &FriendPage::resetFriendNewMessage);

    //这两个应该放在网络线程（通用 通信确认业务），但同时，发送完文件也应该通知聊天框，看具体业务。
    //connect(FtpSender::GetInstance(), &FtpSender::ftpFileSendOver, this, &ChatWindow::ftpSendFileSuccess);
    mEmojiSelector = new EmojiSelector;
    QObject::connect(ui->plainTextEdit, &QTextEdit::cursorPositionChanged, this, &ChatWindow::handleCursorPositionChange);

    connect(mEmojiSelector, &EmojiSelector::emojiSelected, this, &ChatWindow::emojiSelected);
    connect(this, &ChatWindow::sendFiletoPersonSuccess, this, &ChatWindow::sendFiletoPersonSucc);


    // 创建一个垂直布局
    QWidget* widget = ui->widget_2;

    // 创建一个 QSplitter
    QSplitter *splitter = new QSplitter(Qt::Vertical);

    // 添加左侧和右侧的部件
    QWidget *upWidget = ui->plainTextEdit;
    QWidget *downWidget = ui->widget_5;
    ui->widget_5->setMinimumWidth(80); // 设置最小宽度
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

    QString labelText = ui->label->text();
    labelText += QString::fromStdString(mInfo.username);
    ui->label->setText(labelText);
    updateUserPhoto();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

//调用了两次
void ChatWindow::updateUserPhoto() {
    QIcon *icon = PersonCache::GetInstance()->getPersonPhoto(mUserId);
    if (icon != nullptr && !icon->isNull()) {
        qDebug() << "friend " << mInfo.user_id << " " << QString::fromStdString(mInfo.username) << "has photo";
        QIcon copy = *icon;
        qDebug() << copy;
        ui->toolButton->setIcon(copy);
        ui->toolButton->setIconSize(QSize(40, 40));
    }
    else {
        qDebug() << "friend " << mInfo.user_id << " " << QString::fromStdString(mInfo.username) << "has no photo";
        QIcon *icon = PersonCache::GetInstance()->getPersonPhoto(0);
        if (icon != nullptr && !icon->isNull()) {
            ui->toolButton->setIcon(*icon);
            ui->toolButton->setIconSize(QSize(40, 40));
        }
    }
}

//TOSEE:屏幕所见才算消息到达，可以ack,否则下次登录还按未收到消息从服务器发送
void ChatWindow::messageUpdate() {
    int smallNum = INT_MAX;
    int bigNum = 0;
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
        if (mUnReadMessageList[i]->type == MessageInfo::Text) {
            smallNum = min(smallNum, mUnReadMessageList[i]->id);
            bigNum = max(bigNum, mUnReadMessageList[i]->id);
        }
    }
    if (!mUnReadMessageList.empty()) {
        emit confirmMessage(mUserId, ::user_id, smallNum, bigNum);//网络发送给服务器确认收到【start，end】的消息
    }
}


void ChatWindow::addMessage(MessageInfo* info)
{
    //单条消息，显示 切换 显示
    if (this->isVisible()) {
        showMessage(info);
        emit resetFriendNewMessage(mUserId);
    }
    else {
        mUnReadMessageList.push_back(info);
    }
}


void ChatWindow::showMessage(MessageInfo* messageInfo) {
    if (messageInfo == nullptr)
        return;
    qDebug() << "oooooooooooooooooooo" << QString::fromStdString(messageInfo->path) << " : " << messageInfo->type;
    QTextCharFormat defaultFormat;
    ui->plainTextEdit->setCurrentCharFormat(defaultFormat); // 恢复默认格式
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End); // 移动到末尾
    if (messageInfo->send_id == user_id) {
        cursor.insertText(QString::fromStdString(ClientPersonInfo::GetInstance()->username) + "           " + QString::fromStdString(messageInfo->timestamp), defaultFormat);
    }
    else {
        cursor.insertText(QString::fromStdString(mInfo.username) + "           " + QString::fromStdString(messageInfo->timestamp), defaultFormat);
    }
    ui->plainTextEdit->setTextCursor(cursor);
    if (messageInfo->type == MessageInfo::Text) {
        showContentWithEmoji(QString::fromStdString(messageInfo->message_text));
    }
    else if (messageInfo->type == MessageInfo::Picture) {
        showPictureInEdit(ui->plainTextEdit, messageInfo);
    }
    else if (messageInfo->type == MessageInfo::File) {
        showFileMessageInEdit(ui->plainTextEdit, messageInfo);
    }
    mCurrentMessageList.push_back(messageInfo);
}

//非显示 切换 显示
void ChatWindow::showChatContent()
{
    printf("mUnReadMessageList size = %d\n", (int)mUnReadMessageList.size());
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
        showMessage(mUnReadMessageList[i]);
    }
    messageUpdate();
    emit resetFriendNewMessage(mUserId);
    mUnReadMessageList.clear();
}

//显示文字消息，解析表情
void ChatWindow::showContentWithEmoji(QString s) {
    ui->plainTextEdit->append("");
    if (s == "0xa500") {
        ui->plainTextEdit->insertPlainText("抖动窗口！");
        sharkWindow();
        return;
    }
    mEmojiSelector->showContentWithEmoji(ui->plainTextEdit, s);
    ui->plainTextEdit->append("");
}

//图片显示，但考虑窗口是否打开
void ChatWindow::showPictureInEdit(QTextEdit* textEdit, MessageInfo* info) {
    QString imagePath = QString::fromStdString(info->path);
    textEdit->append("");
    if (!imagePath.isEmpty() && info->type == MessageInfo::Picture) {
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End); // 移动到末尾
        QTextImageFormat format;
        format.setName(imagePath);
        format.setWidth(80);
        format.setHeight(80);
        format.setToolTip(imagePath);
        cursor.insertImage(format);
        // 插入可点击的文件名
        textEdit->setTextCursor(cursor); // 更新光标位置
    }
    textEdit->append("");
}

//显示文件消息
void ChatWindow::showFileMessageInEdit(QTextEdit* textEdit, MessageInfo* messageInfo) {
    //textEdit->append(QString::fromStdString(info->message_text));
    QString str = QString::fromStdString(messageInfo->message_text);
    QString completeTip = "Complete Trans Coming in ";
    int index = str.indexOf(completeTip);
    //如果是文件传输完毕消息
    if (index != -1) {
        QString filePath = str.mid(index + completeTip.size());
        QString frontText = str.mid(0, index + completeTip.size());
        qDebug() << "filePathx " << filePath;
        textEdit->append(frontText);
        if (!filePath.isEmpty()) {
            // 插入文件名作为超链接
            QTextCursor cursor = textEdit->textCursor();
            cursor.movePosition(QTextCursor::End); // 移动到末尾
            // 设置超链接格式
            QTextCharFormat linkFormat;
            linkFormat.setForeground(Qt::blue); // 设置字体颜色为蓝色
            linkFormat.setFontUnderline(true); // 设置字体为下划线
            // 插入可点击的文件名
            QTextCharFormat defaultFormat;
            ui->plainTextEdit->setCurrentCharFormat(defaultFormat); // 恢复默认格式
            cursor.insertText(filePath, linkFormat);
            cursor.insertText("\n", defaultFormat); // 插入换行
            textEdit->setTextCursor(cursor); // 更新光标位置
        }
    }
    else {
        textEdit->append(str);
        textEdit->append("");
    }
}

bool isImageFile(const std::string& fileName) {
    // 定义常见的图片文件扩展名
    const std::set<std::string> imageExtensions = {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".tif", ".webp"
    };

    // 获取文件名的扩展名
    std::string::size_type pos = fileName.rfind('.');
    if (pos == std::string::npos) {
        return false; // 没有扩展名
    }

    // 转换扩展名为小写，便于比较
    std::string extension = fileName.substr(pos);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    // 检查扩展名是否在预定义的图片扩展名中
    return imageExtensions.find(extension) != imageExtensions.end();
}

/*
FileMessageInfo
1. arrive begin
2. arrive end
3. send begin
4. send end
*/

//将要到达
void ChatWindow::notifyFileWillArrive(FileInfo fileInfo)
{
    if (QString::fromStdString(fileInfo.fileType).compare("PicTure") == 0 && isImageFile(fileInfo.serverFileName)) {
        // 图片不提示将要到达
        return;
    }
    MessageInfo *messageInfo = nullptr;
    messageInfo = new FileMessageInfo;
    messageInfo->path = fileInfo.ClientPath;
    messageInfo->message_text = "[" + fileInfo.serverFileName + " Beging Trans Coming in " + fileInfo.ClientPath + "]";
    messageInfo->timestamp = fileInfo.timestamp;
    if (this->isVisible()) {
        addMessage(messageInfo);
    }
    else {
        mUnReadMessageList.push_back(messageInfo);
    }
}

//图片直接显示，文件告知传输完成
void ChatWindow::notifyFileAlreadyArrive(FileInfo fileInfo)
{
    MessageInfo *messageInfo = nullptr;
    qDebug() << "llllllllllllllllllllllllllllllllllllllz" << QString::fromStdString(fileInfo.serverFileName);
    if (QString::fromStdString(fileInfo.fileType).compare("PicTure") == 0 && isImageFile(fileInfo.serverFileName)) {
        messageInfo = new PictureMessageInfo;
        messageInfo->path = fileInfo.ClientPath;
        //图片显示发送时的时间
        messageInfo->timestamp = fileInfo.timestamp;
    }
    else {
        messageInfo = new FileMessageInfo;
        messageInfo->path = fileInfo.ClientPath;
        messageInfo->message_text = "[" + fileInfo.serverFileName + "] Complete Trans Coming in " + fileInfo.ClientPath;
        //文件显示下载完成时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        messageInfo->timestamp = currentDateTimeStr.toStdString();
    }
    if (this->isVisible()) {
        addMessage(messageInfo);
    }
    else {
        mUnReadMessageList.push_back(messageInfo);
    }
}

void ChatWindow::emitSendFiletoPerson(FileInfo info) {
    emit sendFiletoPersonSuccess(info);
}

void ChatWindow::sendFiletoPersonSucc(FileInfo info) {
    qDebug() << "sendFiletoPersonSucc " << QString::fromStdString(info.serverFileName) << " : " << QString::fromStdString(info.fileType);
    MessageInfo *messageInfo = nullptr;
    if (QString::fromStdString(info.fileType).compare("PicTure") == 0 && isImageFile(info.serverFileName)) {
        messageInfo = new PictureMessageInfo;
        messageInfo->path = info.ClientPath;
    }
    else {
        messageInfo = new FileMessageInfo;
        messageInfo->message_text = "[" + info.ClientPath + " Send Success  " + "]";
    }
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    messageInfo->timestamp = currentDateTimeStr.toStdString();
    messageInfo->send_id = user_id;
    if (this->isVisible()) {
        addMessage(messageInfo);
    }
    else {
        mUnReadMessageList.push_back(messageInfo);
    }
}


//开始发送给好友文件,作为文件发送的图片不显示
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
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }

    MessageInfo* messageInfo = new FileMessageInfo;
    messageInfo->message_text = "[" + info.ClientPath + " Send Begin... " + "]";
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    messageInfo->timestamp = currentDateTimeStr.toStdString();

    if (this->isVisible()) {
        addMessage(messageInfo);
    }
    else {
        mUnReadMessageList.push_back(messageInfo);
    }
}

void ChatWindow::keyPressEvent(QKeyEvent *event)
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

//发送文字消息
bool ChatWindow::sendMessage(const QString &content) {
    if (content != "") {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        std::string name = ((FriendPage*)returnWindow)->mInfo.username;
        // 将日期和时间转换为字符串
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        MessageInfo* info = new TextMessageInfo;
        info->recv_id = mInfo.user_id;
        info->send_id = user_id;
        info->timestamp = currentDateTimeStr.toStdString();
        info->message_text = content.toStdString();
        //此处应优化为以info为参数
        bool ret = Processor::SendMessage(mUserId, content.toStdString());
        if (!ret) {
            addMessage(info);
            QMessageBox::information(this,"提示","网络不可达！");
            return false;
        }
        addMessage(info);
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
    if (isImageFile(info.serverFileName) == true)
        info.fileType = std::string("PicTure");
    else {
        info.fileType = std::string("Other");
    }
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::SendFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
}

//文件2 废弃
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


bool openFileDirectory(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile()) {
        QString directoryPath = fileInfo.absolutePath();
        QUrl url = QUrl::fromLocalFile(directoryPath);
        // 打开文件所在目录
        QDesktopServices::openUrl(url);
        return true;
    } else {
        qWarning() << "File does not exist or is not a valid file.";
        return false;
    }
    return false;
}

//TODO:实现超链接功能
void ChatWindow::handleCursorPositionChange() {
    // 处理文本框中的点击事件
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    if (cursor.hasSelection()) {
        return; // 如果有选中内容，不处理
    }
    QTextCharFormat format = cursor.charFormat();
    if (format.foreground().color() == Qt::blue && format.fontUnderline()) {
        QString str = cursor.block().text();
        QString completeTip = "Complete Trans Coming in ";
        int index = str.indexOf(completeTip);
        QString filePath;
        if (index != -1) {
            filePath = str.mid(index + completeTip.size());
        }
        qDebug() << filePath;
        if (!filePath.isEmpty()) {
            // 在这里可以处理打开文件的逻辑
            // 例如，打开文件或执行其他操作
            // QFile file(filePath);
            // 进行读取或其他操作
            bool ret = openFileDirectory(filePath);
            if (ret) {
                qDebug() << "Open " << filePath << " Success";
            }
        }
    }
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

void ChatWindow::on_toolButton_clicked()
{
    PersonCardCon *card = new PersonCardCon(mInfo);
    card->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    // 获取按钮的几何信息，设置对话框的位置
    QToolButton *button = qobject_cast<QToolButton*>(sender());
    if (button) {
        QPoint buttonPos = button->mapToGlobal(button->rect().center());
        card->move(buttonPos); // 设置对话框的位置
    }
    card->show();
}

