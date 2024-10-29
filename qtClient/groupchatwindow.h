#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

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

    void addGroupMemberToPage(int index, UserInfo info);
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

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void resetGroupNewMessage(int);

private slots:
    void findAllGroupMemberSuccess(Response response);
    void on_sendMsgBtn_clicked();

    void on_imageBtn_clicked();

    void on_fileBtn_clicked();

private:
    GroupInfo mInfo;
    int mUserId;
    std::map<int, UserInfo> mGroupMemberMap;
    Ui::GroupChatWindow *ui;
    EmojiSelector *mEmojiSelector;
};

#endif // GROUPCHATWINDOW_H
