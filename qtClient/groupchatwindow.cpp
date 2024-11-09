#include "clientpersoninfo.h"
#include "friendpage.h"
#include "ftpsender.h"
#include "groupchatwindow.h"
#include "network.h"
#include "personcache.h"
#include "personcardcon.h"
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
#include <QSettings>
#include "processor.h"

GroupChatWindow::GroupChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChatWindow)
{
    ui->setupUi(this);
    returnWindow = NULL;
    mUserId = 1;
    mInited = false;
    mLocalConfirmId = 0;
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));
    mEmojiSelector = new EmojiSelector;
    connect(ClientNetWork::GetInstance(), &ClientNetWork::findAllGroupMemberSuccess, this, &GroupChatWindow::findAllGroupMemberSuccess);
    connect(mEmojiSelector, &EmojiSelector::emojiSelected, this, &GroupChatWindow::emojiSelected);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::getAllGroupMessageSuccess, this, &GroupChatWindow::getAllGroupMessageSuccess);
    ui->plainTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑内容

    connect(ui->plainTextEdit, &QTextEdit::cursorPositionChanged, this, &GroupChatWindow::handleCursorPositionChange);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ChangeGroupUserPic, this, &GroupChatWindow::ChangeGroupUserPic);
    //TODO:
    //    connect(this, &ChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmMessage);
    connect(this, &GroupChatWindow::confirmMessage, ClientNetWork::GetInstance(), &ClientNetWork::confirmgroupMessage);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ProcessGroupMessageSuccess, this, &GroupChatWindow::ProcessGroupMessageSuccess);
//    connect(this, &ChatWindow::sendFiletoPersonSuccess, this, &ChatWindow::sendFiletoPersonSucc);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::GroupMessageArriveClient, this, &GroupChatWindow::GroupMessageArriveClient);
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
    mRemoteConfirmId = info.confirmId;
    QString labelText = ui->label->text();
    labelText += QString::fromStdString(mInfo.group_name);
    ui->label->setText(labelText);
}

GroupChatWindow::~GroupChatWindow()
{
    delete ui;
}

bool GroupChatWindow::hasInited() {
    return mInited;
}

bool GroupChatWindow::initGroupMessage()
{
    bool ret = Processor::getAllGroupMessage(mInfo.id, mLocalConfirmId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool GroupChatWindow::init()
{
    getGroupLocalConfirmId();
    if (returnWindow) {
        connect(this, &GroupChatWindow::notifyGroupNewMessage, (FriendPage*)this->returnWindow, &FriendPage::notifyGroupNewMessage);
        connect(this, &GroupChatWindow::resetGroupNewMessage, (FriendPage*)this->returnWindow, &FriendPage::resetGroupNewMessage);
    }
    //初始化群成员列表
    initGroupMember();
    //初始化群消息
    initGroupMessage();
    //初始化通知

    //初始化群文件列表

    //持久化本地
    mInited = true;
    return true;
}

bool GroupChatWindow::initGroupMember() {
    bool ret = Processor::findAllgroupMember(mInfo.id);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

bool GroupChatWindow::eventFilter(QObject *obj, QEvent *event)
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
                // 判断鼠标点击位置是否在按钮的矩形区域内
                if (buttonRect.contains(mousePos))
                {
                    QString type = button->property("type").toString();

                    // 处理点击事件
                    if (type == "Friend") {
                        emit showPersonCard(ui->memberListWidget->itemAt(button->pos()));
                    }
                    // 标记事件已处理
                    event->accept();
                    return true; // 表示事件已处理
                }
            }
        }
    }
    return false; // 没有处理该事件，继续传递
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
    int groupId;
    stream2 >> groupId >> size;
    if (groupId != mInfo.id) {
        return;
    }
    qDebug() << "lllllllllllll " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        UserInfo info;
        stream2 >> info.user_id >> info.friendStatus >> info.username >> info.email >> info.avatar_url >> info.status >> info.role >> info.joined_at;
        addGroupMemberToPage(i, info);//
        info.print();
        mGroupMemberMap[info.user_id] = info;
        if (PersonCache::GetInstance()->count(info.user_id) == false) {
            PersonCache::GetInstance()->addPerson(info, false);
        }
    }
    initGroupMemberPhoto();
    //initGroupMemberState();
}

void GroupChatWindow::getAllGroupMessageSuccess(Response response)
{
    //TODO:闪烁几个窗口，右下角图标添加几个提示
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int size = 0, groupId = 0;
    stream2 >> groupId >> size;
    if (groupId != mInfo.id) {
        return;
    }
    qDebug() << "sizeMessage: " << size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        MessageInfo* info = new TextMessageInfo;
        stream2 >> info->id >> info->send_id >> info->message_text >> info->timestamp;
        qDebug() << info->send_id;
        qDebug() << QString::fromStdString(info->message_text);
        info->recv_id = user_id;
        info->print();
        emit notifyGroupNewMessage(mInfo.id);
        addMessage(info);
    }
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
    mFriendButton[info.user_id] = button;
    button->setProperty("type", "Friend");
    // 连接信号和槽
    connect(ui->memberListWidget, &QListWidget::itemClicked, this, &GroupChatWindow::showPersonCard);
    QSize size;
    size.setHeight(25);
    size.setWidth(60);
    ui->memberListWidget->item(index)->setSizeHint(size);
}

bool GroupChatWindow::showPersonCard(QListWidgetItem* item) {
    int id = item->whatsThis().toInt();

    if (mGroupMemberMap.count(id)) {
        PersonCardCon *card = new PersonCardCon(mGroupMemberMap[id]);
        card->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
        // 获取按钮的几何信息，设置对话框的位置
        QListWidget *widget = qobject_cast<QListWidget*>(sender());
        if (widget) {
            QWidget* buttonWidget = widget->itemWidget(item);
            QPoint buttonPos = buttonWidget->mapToGlobal(buttonWidget->rect().center());
            card->move(buttonPos); // 设置对话框的位置
        }
        card->show();
    }
    return true;
}

int GroupChatWindow::getFriendPhoto(UserInfo& userinfo) {
    qDebug() << "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz" << userinfo.user_id << QString::fromStdString(userinfo.avatar_url);
    qDebug() << PersonCache::GetInstance()->getPersonPhoto(userinfo.user_id);
    QIcon* icon = PersonCache::GetInstance()->getPersonPhoto(userinfo.user_id);
    if (icon != nullptr && !icon->isNull()) {
        QToolButton* button = mFriendButton[userinfo.user_id];
        button->setIcon(*icon);
        button->setIconSize(QSize(25, 25));
        return true;
    }
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
        qDebug() << "修改群朋友头像，当前工作目录:" << filePath;
        if (QFile::exists(filePath)) {
            // 如果文件存在，加载图像并设置为头像
            QIcon *icon = new QIcon(filePath);
            mFriendButton[userinfo.user_id]->setIcon(*icon);
            mPhotoMap[userinfo.user_id] = icon;
            mFriendButton[userinfo.user_id]->setIconSize(QSize(50, 50));
            PersonCache::GetInstance()->setPersonPhoto(userinfo.user_id, *icon, filePath);
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
        bool ret = Processor::SendMessage(info);
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
        emit resetGroupNewMessage(mInfo.id);
        messageUpdate(info);
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
    messageUpdate();
    emit resetGroupNewMessage(mInfo.id);
    mUnReadMessageList.clear();
}

//发送已累积确认的最大值，如果中间有空缺就不行
//一个数据包里的message一定是在表中顺序的
void GroupChatWindow::messageUpdate() {
//    int smallNum = INT_MAX;
//    int bigNum = 0;
    QVector<int> confirmVec;
    for (int i = 0; i < (int)mUnReadMessageList.size(); i++) {
        if (mUnReadMessageList[i]->type == MessageInfo::Text) {
//            smallNum = min(smallNum, mUnReadMessageList[i]->id);
//            bigNum = max(bigNum, mUnReadMessageList[i]->id);
            confirmVec.push_back(mUnReadMessageList[i]->id);
        }
    }

    //TODO:更新local
    if (!mUnReadMessageList.empty()) {
        emit confirmMessage(mInfo.id, ::user_id, mLocalConfirmId, confirmVec);//网络发送给服务器确认收到【start，end】的消息
    }
}


void GroupChatWindow::messageUpdate(MessageInfo* info) {
    if (info->type != MessageInfo::Text) {
        return;
    }
    QVector<int> vec;
    vec.push_back(info->id);
    emit confirmMessage(mInfo.id, ::user_id, mLocalConfirmId, vec);//网络发送给服务器确认收到【start，end】的消息
}

void GroupChatWindow::ProcessGroupMessageSuccess(Response response) {
    std::string mdata = response.mData;
    MyProtocolStream stream(mdata);
    int groupId = 0, userId = 0, conId = 0;
    stream >> groupId >> userId >> conId;
    if (groupId != mInfo.id) {
        return;
    }
    qDebug() << "ProcessGroupMessageSuccess " << groupId << " " << userId;
    if (groupId == mInfo.id && user_id == userId) {
        mLocalConfirmId = conId;
        //TODO:写本地配置文件
        setGroupLocalConfirmId();
        qDebug() << "flush localConfirmId : " << mLocalConfirmId;
    }
}


int GroupChatWindow::setGroupLocalConfirmId()
{
    QString confName = "p" + QString::number(user_id) + ".ini";
    QSettings settings(confName, QSettings::IniFormat);
    QString groupItem = "group" + QString::number(mInfo.id) + QString("/localConfirmId");
    settings.setValue(groupItem, mLocalConfirmId);
    return mLocalConfirmId;
}

int GroupChatWindow::getGroupLocalConfirmId()
{
    QString confName = "p" + QString::number(user_id) + ".ini";
    QSettings settings(confName, QSettings::IniFormat);
    QString groupItem = "group" + QString::number(mInfo.id) + QString("/localConfirmId");
    // 如果配置项不存在，则创建该项并设置默认值 0
    if (!settings.contains(groupItem)) {
        settings.setValue(groupItem, 0); // 创建并设置默认值
        qDebug() << groupItem << "配置项不存在，已创建并设置为 0";
    }
    int confirmId = settings.value(groupItem, 0).toInt();
    mLocalConfirmId = confirmId;
    qDebug() << "fffffffffffffffffffffffffffffffff" << groupItem << " : " << confirmId;
    return mLocalConfirmId;
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
    qDebug() << "oooooooooooooooooooooooooooooo" << info.owner;
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
            PersonCache::GetInstance()->setPersonPhoto(info.owner, *icon, clientPath);
        }
    }
}

void GroupChatWindow::GroupMessageArriveClient(Response response)
{
    //在线消息到达
    std::string &mData = response.mData;
    MyProtocolStream stream(mData);
    MessageInfo* info = new TextMessageInfo;
    stream >> info->flag >> info->id >> info->send_id >> info->recv_id >> info->timestamp >> info->message_text;
    if (info->recv_id != mInfo.id) {
        delete info;
        return;
    }
    qDebug() << "message arrive" << info->send_id << "  " << info->recv_id;
    if (info->send_id == ::user_id) {
        messageUpdate(info);
        delete info;
        return;
    }
    emit notifyGroupNewMessage(info->recv_id);
    addMessage(info);
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
