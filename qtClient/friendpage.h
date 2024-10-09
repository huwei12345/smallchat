#ifndef FRIENDPAGE_H
#define FRIENDPAGE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include "Protocol.h"
#include <map>
#include <set>
#include <unordered_set>
#include "groupchatwindow.h"

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
    int m_index;
    ~FriendPage();

    int init();
    bool initFriendList();
    bool initGroupList();
    bool initMessageList();
    bool initFriendRequest();
    bool initMyPhoto();
    bool initFriendPhoto();
    bool initAllOfflineFile();
    bool initPage();
    void friendPageUpdate(int uid);
    UserInfo mInfo;
    void GetFileSuccess(Response response);
    void SendFileSuccess(Response response);

    void StartUpLoadFileSuccess(Response response);

    int getFriendPhoto(UserInfo &userinfo);
    void ProcessFriendRequestResult(Response response);
    void initFriendState();

    void addFriendToPage(int index, UserInfo info);
    bool addGroupToPage(GroupInfo info);

protected:
    void keyPressEvent(QKeyEvent *event) override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


signals:
    void userMessageUnRead();

private slots:

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void chatWithFriend(QListWidgetItem* item);
    void chatWithGroup(QListWidgetItem *item);
    void findAllFriendSuccess(Response response);
    void findAllGroupSuccess(Response response);
    void getAllMessageSuccess(Response);
    void getAllOfflineFileSuccess(Response);
    void getAllFriendRequestSuccess(Response);
    void UpDateUserStateSuccess(Response response);
    void MessageArriveClient(Response response);
    void on_comboBox_currentIndexChanged(int index);
    void friendMessageArrive(FriendRequest info);
    void on_toolButton_2_clicked();
    void NofifyFileComing(Response response);
    void ChangeUserPic(FileInfo info);
    void ChangeUserPicBySend(FileInfo info);
    void GetFileFirstSuccess(Response response);
    void on_toolButton_5_clicked();

private:
    Ui::FriendPage *ui;
    QWidget* returnWindow;
    FindFriendPage* m_FindFriendPage;
    CreateGroupPage* m_CreateGroupPage;
    int mUserId;


    std::vector<UserInfo> mFriendList;
    std::vector<GroupInfo> mGroupList;

    std::map<int, ChatWindow*> mChatWindowMap;
    std::map<int, GroupChatWindow*> mGroupWindowMap;

    std::map<int, QToolButton*> mFriendButton;
    std::map<int, QToolButton*> mGroupButton;

    std::map<int, QIcon*> mPhotoMap;
    std::map<int, QIcon*> mGroupPhotoMap;

    std::unordered_set<FriendRequest, friendHasher, friendEqual> mFriendRequestSet;
    QTimer *mFriendRequestTimer;

    std::map<int, QTimer*> mUnReadMessageTimerMap;
};

#endif // FRIENDPAGE_H
