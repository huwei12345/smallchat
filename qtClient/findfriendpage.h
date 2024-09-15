#ifndef FINDFRIENDPAGE_H
#define FINDFRIENDPAGE_H

#include <QListWidgetItem>
#include <QWidget>
#include "Protocol.h"
namespace Ui {
class FindFriendPage;
}

class FindFriendPage : public QWidget
{
    Q_OBJECT
public:
    explicit FindFriendPage(QWidget *parent = nullptr);
    QWidget* returnWindow;
    ~FindFriendPage();
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void findFriendSuccess(Response response);
    void addFriendSuccess(int mCode);
    void on_listWidget_itemSelectionChanged();

private:
    Ui::FindFriendPage *ui;
};

#endif // FINDFRIENDPAGE_H
