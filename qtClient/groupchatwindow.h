#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QWidget>
#include "Protocol.h"

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
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    GroupInfo mInfo;
    int mUserId;
    Ui::GroupChatWindow *ui;
};

#endif // GROUPCHATWINDOW_H
