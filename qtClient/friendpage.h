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
class MainPage;
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
    bool initGroupRequest();
    bool initMyPhoto();
    bool initFriendPhoto();
    bool initAllOfflineFile();
    bool initPage();
    void resetFriendNewMessage(int uid);
    UserInfo mInfo;
    void GetFileSuccess(Response response);
    void SendFileSuccess(Response response);

    void StartUpLoadFileSuccess(Response response);
    void StoreFileAllow(Response response);

    int getFriendPhoto(UserInfo &userinfo);
    void ProcessFriendRequestResult(Response response);
    void initFriendState();

    void addFriendToPage(int index, UserInfo info);
    bool addGroupToPage(GroupInfo info);

    void notifyFriendNewMessage(int userId);
    void notifyGroupNewMessage(int groupId);
protected:
    void keyPressEvent(QKeyEvent *event) override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


signals:
    void StoreFileSuccess(FileInfo info);
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
    void getAllGroupRequestSuccess(Response);
    void UpDateUserStateSuccess(Response response);
    void MessageArriveClient(Response response);
    void on_comboBox_currentIndexChanged(int index);
    void friendRequestArrive(FriendRequest info);
    void on_toolButton_2_clicked();
    void NofifyFileComing(Response response);
    void ChangeUserPic(FileInfo info);
    void ChangeUserPicBySend(FileInfo info);
    void GetFileFirstSuccess(Response response);
    void on_toolButton_5_clicked();

    void on_toolButton_4_clicked();

private:
    Ui::FriendPage *ui;
    QWidget* returnWindow;
    FindFriendPage* m_FindFriendPage;
    CreateGroupPage* m_CreateGroupPage;
    int mUserId;


    std::map<int, UserInfo> mFriendMap;
    std::map<int, GroupInfo> mGroupList;

    std::map<int, ChatWindow*> mChatWindowMap;
    std::map<int, GroupChatWindow*> mGroupWindowMap;

    std::map<int, QToolButton*> mFriendButton;
    std::map<int, QToolButton*> mGroupButton;

    std::map<int, QIcon*> mPhotoMap;
    std::map<int, QIcon*> mGroupPhotoMap;

    std::unordered_set<FriendRequest, friendHasher, friendEqual> mFriendRequestSet;
    std::unordered_set<GroupJoinRequest, groupHasher, groupEqual> mGroupRequestSet;
    QTimer *mFriendRequestTimer;

    std::map<int, QTimer*> mUnReadMessageTimerMap;
    MainPage* mSpacePage;
};

#endif // FRIENDPAGE_H
