#ifndef CHATWINDOW_H
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
    void messageUpdate();
    vector<MessageInfo> mUnReadMessageList;
    int mUserId;
protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_clicked();

signals:
    void confirmMessage(int, int start, int end);
    void friendPageUpdate(int);
public slots:
    void userMessageRead();
private:
    Ui::ChatWindow *ui;
    UserInfo mInfo;
};

#endif // CHATWINDOW_H
