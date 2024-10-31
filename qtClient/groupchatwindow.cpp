#include "clientpersoninfo.h"
#include "friendpage.h"
#include "ftpsender.h"
#include "groupchatwindow.h"
#include "network.h"
#include "personcache.h"
#include "ui_groupchatwindow.h"

#include <QDateTime>
#include <QIcon>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextBlock>
#include <QFileInfo>
#include <QDesktopServices>
#include <QSplitter>
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
    mEmojiSelector = new EmojiSelector;
    connect(ClientNetWork::GetInstance(), &ClientNetWork::findAllGroupMemberSuccess, this, &GroupChatWindow::findAllGroupMemberSuccess);
    connect(mEmojiSelector, &EmojiSelector::emojiSelected, this, &GroupChatWindow::emojiSelected);
    connect(this, &GroupChatWindow::resetGroupNewMessage, (FriendPage*)this->parent(), &FriendPage::resetFriendNewMessage);

    ui->plainTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑内容

    connect(ui->plainTextEdit, &QTextEdit::cursorPositionChanged, this, &GroupChatWindow::handleCursorPositionChange);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ChangeGroupUserPic, this, &GroupChatWindow::ChangeGroupUserPic);
    //TODO:
    //    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);


//    connect(this, &ChatWindow::sendFiletoPersonSuccess, this, &ChatWindow::sendFiletoPersonSucc);


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
    //设置主窗口的布局
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
    initGroupMember();
    //初始化群消息

    //初始化通知

    //初始化群文件列表

    //持久化本地

    return true;
}

bool GroupChatWindow::initGroupMember() {
    bool ret = Processor::findAllgroupMember(mInfo.id);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
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

void GroupChatWindow::findAllGroupMemberSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0;
    stream2 >> size;
    qDebug() << "lllllllllllll " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        UserInfo info;
        stream2 >> info.user_id >> info.friendStatus >> info.username >> info.email >> info.avatar_url >> info.status >> info.role >> info.joined_at;
        addGroupMemberToPage(i, info);//
        info.print();
        mGroupMemberMap[info.user_id] = info;
        PersonCache::GetInstance()->addPerson(info);
    }
    initGroupMemberPhoto();
    //initGroupMemberState();
}

void GroupChatWindow::addGroupMemberToPage(int index, UserInfo info) {
    Q_UNUSED(index);

    QToolButton* button = new QToolButton();
    QString str = QString::fromStdString(info.username);
    //str += QString::number(i);
    button->setText(str);
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QIcon *userIcon = new QIcon(QPixmap(":/friend/touxiang.jpeg"));
    button->setIcon(*userIcon);
    mPhotoMap[info.user_id] = userIcon;
    //button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    button->setMinimumHeight(25);
    button->setMinimumWidth(25);
    button->setIconSize(QSize(25, 25));
    QString str2("item");
    str2 += QString::number(index);
    ui->memberListWidget->addItem(str2);
    ui->memberListWidget->setItemWidget(ui->memberListWidget->item(index), button);
    ui->memberListWidget->item(index)->setWhatsThis(QString::number(info.user_id));
    //installEventFilter + eventFilter overide可以实现防覆
    button->installEventFilter(this); // 在当前类中实现 eventFilter 方法
    mFriendButton[info.user_id] =button;
    button->setProperty("type", "Friend");
    // 连接信号和槽
    connect(ui->memberListWidget, &QListWidget::itemClicked, this, &GroupChatWindow::showPersonCard);
    QSize size;
    size.setHeight(25);
    size.setWidth(60);
    ui->memberListWidget->item(index)->setSizeHint(size);
}

bool GroupChatWindow::showPersonCard(QListWidgetItem* item) {

}

int GroupChatWindow::getFriendPhoto(UserInfo& userinfo) {
    if (userinfo.avatar_url == "avatar_url") {
        return 0;
    }
    FileInfo info;
    QString serverPath = QString::fromStdString(userinfo.avatar_url);
    QFileInfo fileInfo(serverPath);
    // 获取文件名（包括扩展名）
    QString fileName = fileInfo.fileName();

    QRegularExpression pattern(R"(^mr(1[0-9]|20|[1-9])\.jpg$)");
    bool regularPic = pattern.match(fileName).hasMatch();
    if (regularPic) {
        QString filePath = QCoreApplication::applicationDirPath() + "/userPhoto/regular/" + fileName;
        qDebug() << "修改朋友头像，当前工作目录:" << filePath;
        if (QFile::exists(filePath)) {
            // 如果文件存在，加载图像并设置为头像
            QIcon *icon = new QIcon(filePath);
            mFriendButton[userinfo.user_id]->setIcon(*icon);
            mPhotoMap[userinfo.user_id] = icon;
            mFriendButton[userinfo.user_id]->setIconSize(QSize(50, 50));
            PersonCache::GetInstance()->setPersonPhoto(userinfo.user_id, *icon);
        } else {
            // 文件不存在，给用户提示
            QMessageBox::warning(nullptr, "警告", "常规头像文件不存在[客户端错误1]");
        }
        return 2;
    }
    info.Generate();
    info.serviceType = FileServerType::TOUXIANG;
    //截取服务器发来的图片路径和名字
    info.serverPath = fileInfo.path().toStdString() + "/";
    info.serverFileName = fileInfo.fileName().toStdString();
    info.owner = userinfo.user_id;
    qDebug() << QString::fromStdString(userinfo.username) << " 头像： " << QString::fromStdString(info.serverFileName);
    //客户端名字统一为userPhoto/tx...[.suffex]
    info.ClientPath = QCoreApplication::applicationDirPath().toStdString() + "/userPhoto/tx" + userinfo.username + "." + fileInfo.suffix().toStdString();
    FtpSender::GetInstance()->addFile(info);
    return Processor::GetFile(info);
}

bool GroupChatWindow::initGroupMemberPhoto() {
    //发送一大堆图片获取命令，根据在获取朋友列表里得到的url
    for (auto person : mGroupMemberMap) {
        getFriendPhoto(person.second);
    }
    return true;
}

//发送文字消息
bool GroupChatWindow::sendMessage(const QString &content) {
    if (content != "") {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeStr = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        MessageInfo* info = new TextMessageInfo;
        info->recv_id = mInfo.id;
        info->send_id = user_id;
        info->timestamp = currentDateTimeStr.toStdString();
        info->message_text = content.toStdString();
        info->flag = MessageInfo::Group;
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

void GroupChatWindow::addMessage(MessageInfo* info)
{
    //单条消息，显示 切换 显示
    if (this->isVisible()) {
        showMessage(info);
        emit resetGroupNewMessage(mUserId);
    }
    else {
        mUnReadMessageList.push_back(info);
    }
}

void GroupChatWindow::showMessage(MessageInfo* messageInfo) {
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
        cursor.insertText(QString::number(messageInfo->send_id) + "           " + QString::fromStdString(messageInfo->timestamp), defaultFormat);
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
void GroupChatWindow::showChatContent()
{
    printf("mUnReadMessageList size = %d\n", (int)mUnReadMessageList.size());
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
        showMessage(mUnReadMessageList[i]);
    }
    //TODO:群消息如何确认
    //messageUpdate();
    emit resetGroupNewMessage(mUserId);
    mUnReadMessageList.clear();
}

//显示文字消息，解析表情
void GroupChatWindow::showContentWithEmoji(QString s) {
    ui->plainTextEdit->append("");
//    if (s == "0xa500") {
//        ui->plainTextEdit->insertPlainText("抖动窗口！");
//        sharkWindow();
//        return;
//    }
    mEmojiSelector->showContentWithEmoji(ui->plainTextEdit, s);
    ui->plainTextEdit->append("");
}

//图片显示，但考虑窗口是否打开
void GroupChatWindow::showPictureInEdit(QTextEdit* textEdit, MessageInfo* info) {
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
void GroupChatWindow::showFileMessageInEdit(QTextEdit* textEdit, MessageInfo* messageInfo) {
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

void GroupChatWindow::on_sendMsgBtn_clicked()
{
    QString content = mEmojiSelector->extractContentWithImages(ui->plainTextEdit_2);
    sendMessage(content);
}


void GroupChatWindow::on_imageBtn_clicked()
{

}


void GroupChatWindow::on_fileBtn_clicked()
{

}

void GroupChatWindow::ChangeGroupUserPic(FileInfo info)
{
    if (!mGroupMemberMap.count(info.owner)) {
        return;
    }
    QString clientPath = QString::fromStdString(info.ClientPath);//需要改为的名字
    size_t regular = info.ClientPath.find("regular/");

    if (QFile::exists(clientPath)) {
            // 如果文件存在，加载图像并设置为头像
            QIcon *icon = new QIcon(clientPath);
            mFriendButton[info.owner]->setIcon(*icon);
            mFriendButton[info.owner]->setIconSize(QSize(25, 25));
            ClientPersonInfo::GetInstance()->photo = *icon;
    }
    else {
        if (regular != std::string::npos) {
            return;
        }
        //修改其他用户头像
        QToolButton* button = mFriendButton[info.owner];
        if (QFile::exists(clientPath)) {
            // 如果文件存在，加载图像并设置为头像
            QIcon *icon = new QIcon(clientPath);
            button->setIcon(*icon);
            button->setIconSize(QSize(25, 25));
            PersonCache::GetInstance()->setPersonPhoto(info.owner, *icon);
        }
    }
}

//从edit里读取出QString,然后在位置中加入表情的代号如#0xa301，然后清空edit，重新绘制，
//绘制的时候如果是字符直接写，如果是表情，那么替换为表情图片。
void GroupChatWindow::emojiSelected(QString emoji) {
    mEmojiSelector->explain(ui->plainTextEdit_2, emoji);
}

//TODO:实现超链接功能
void GroupChatWindow::handleCursorPositionChange() {
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

bool GroupChatWindow::openFileDirectory(const QString &filePath) {
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
