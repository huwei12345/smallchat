#ifndef CREATEGROUPPAGE_H
#define CREATEGROUPPAGE_H

#include <QWidget>
#include "Protocol.h"
namespace Ui {
class CreateGroupPage;
}

class CreateGroupPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroupPage(QWidget *parent = nullptr);
    ~CreateGroupPage();
    QWidget* returnWindow;
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::CreateGroupPage *ui;

private slots:
    void createGroupSuccess(Response rsp);
    void on_pushButton_clicked();
};

#endif // CREATEGROUPPAGE_H
