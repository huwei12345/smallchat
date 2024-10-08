﻿#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "Protocol.h"
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    QWidget* returnWindow;
    explicit ChatWindow(QWidget *parent = nullptr);
    explicit ChatWindow(UserInfo info, QWidget *parent = nullptr);
    ~ChatWindow();
    void addMessage(MessageInfo info);
    void addFileArrive(FileInfo info);
    void messageUpdate();
    vector<MessageInfo> mUnReadMessageList;
    vector<MessageInfo> mCurrentMessageList;//包括已读取和已发送和一些历史记录，可能需要持久化
    int mUserId;
    void showChatContent();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_clicked();
    void offlineTransFileSuccess(Response rsp);
    void on_toolButton_5_clicked();

    void on_toolButton_8_clicked();

    void on_toolButton_9_clicked();
    void handleCursorPositionChange();

signals:
    void confirmMessage(int sender, int reciver, int start, int end);
    void friendPageUpdate(int);
public slots:
    void userMessageRead();
private:
    Ui::ChatWindow *ui;
    UserInfo mInfo;
UserInfo* clientInfo;
};

#endif // CHATWINDOW_H
