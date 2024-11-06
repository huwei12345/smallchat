#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QListWidget>
#include <QToolButton>
#include <QWidget>
#include "Protocol.h"
#include "emojiselector.h"

namespace Ui {
class GroupChatWindow;
}

class GroupChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChatWindow(QWidget *parent = nullptr);
    explicit GroupChatWindow(GroupInfo info, QWidget *parent = nullptr);
    ~GroupChatWindow();
    QWidget* returnWindow;
    bool init();
    bool initGroupMember();

    bool initGroupMemberPhoto();
    bool sendMessage(const QString &content);
    void addMessage(MessageInfo *info);
    void showMessage(MessageInfo *messageInfo);
    vector<MessageInfo*> mUnReadMessageList;
    vector<MessageInfo*> mCurrentMessageList;//包括已读取和已发送和一些历史记录，可能需要持久化

    void showChatContent();
    void showContentWithEmoji(QString s);
    void showFileMessageInEdit(QTextEdit *textEdit, MessageInfo *messageInfo);
    void showPictureInEdit(QTextEdit *textEdit, MessageInfo *info);

    void emojiSelected(QString emoji);
    void handleCursorPositionChange();
    bool openFileDirectory(const QString &filePath);

    void addGroupMemberToPage(int index, UserInfo info);
    bool showPersonCard(QListWidgetItem *item);
    int getFriendPhoto(UserInfo &userinfo);
    bool hasInited();

    bool initGroupMessage();
    void messageUpdate();
    void messageUpdate(MessageInfo *info);
    int getGroupLocalConfirmId();
    void ProcessGroupMessageSuccess(Response response);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void resetGroupNewMessage(int);
    void notifyGroupNewMessage(int);
    void confirmMessage(int, int, int, QVector<int>);
private slots:
    void findAllGroupMemberSuccess(Response response);
    void getAllGroupMessageSuccess(Response response);
    void on_sendMsgBtn_clicked();

    void on_imageBtn_clicked();

    void on_fileBtn_clicked();
    void ChangeGroupUserPic(FileInfo info);
private:
    GroupInfo mInfo;
    int mUserId;
    std::map<int, UserInfo> mGroupMemberMap;
    Ui::GroupChatWindow *ui;
    EmojiSelector *mEmojiSelector;

    std::map<int, QToolButton*> mFriendButton;

    std::map<int, QIcon*> mPhotoMap;
    bool mInited;
    //记录在配置文件
    int localConfirmId;//[localConfirmId, remoteConfirmId]之间需要获取，但不需要为未读，是已读的，但当前设备需要显示，并且显示为已读。
    int remoteConfirmId;//[remoteConfirmId, nowMaxId]之间为未读消息，需要提醒用户。
};

#endif // GROUPCHATWINDOW_H
