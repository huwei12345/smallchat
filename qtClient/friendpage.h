#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include "Protocol.h"
#include <map>
#include <set>
#include <unordered_set>
class FindFriendPage;
class CreateGroupPage;
class ChatWindow;
namespace Ui {
class FriendPage;
}

class FriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendPage(QWidget *parent = nullptr);
    explicit FriendPage(UserInfo& info, QWidget *parent = nullptr);
    void setReturn(QWidget* widget);
    void addFriendToPage(int index, UserInfo info);
    int m_index;
    ~FriendPage();

    int init();
    bool initFriendList();
    bool initMessageList();
    bool initFriendRequest();
    bool initMyPhoto();

    bool initPage();
    void friendPageUpdate(int uid);
    UserInfo mInfo;
    void ftpGetFileSuccess(string filename);
    void GetFileSuccess(Response response);
protected:
    void keyPressEvent(QKeyEvent *event) override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


signals:
    void userMessageUnRead();

private slots:
    void on_toolButton_4_clicked();

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void chatWithFriend(QListWidgetItem* item);
    void findAllFriendSuccess(Response response);
    void getAllMessageSuccess(Response);
    void getAllFriendRequestSuccess(Response);
    void UpDateUserStateSuccess(Response response);
    void ReciveMessageSuccess(Response response);
    void MessageArriveClient(Response response);
    void on_comboBox_currentIndexChanged(int index);
    void friendMessageArrive(FriendRequest info);
    void storeFileSuccess(Response response);
    void on_toolButton_2_clicked();
    void NofifyFileComing(Response response);
    void ChangeOwnerPic();
    void GetFileFirstSuccess(Response response);
private:
    Ui::FriendPage *ui;
    QWidget* returnWindow;
    FindFriendPage* m_FindFriendPage;
    CreateGroupPage* m_CreateGroupPage;
    int mUserId;


    std::vector<UserInfo> mFriendList;
    std::map<int, ChatWindow*> mChatWindowMap;
    std::map<int, QToolButton*> mFriendButton;
    std::unordered_set<FriendRequest, friendHasher, friendEqual> mFriendRequestSet;
    QTimer *mFriendRequestTimer;
    std::map<int, QTimer*> mUnReadMessageTimerMap;
};

#endif // FRIENDPAGE_H
