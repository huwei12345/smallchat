#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "Protocol.h"
namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    explicit MainPage(UserInfo info, QWidget *parent = nullptr);
    void setReturn(QWidget* widget);
    ~MainPage();
    void StoreFileAllow(Response response);
    void StoreFileSuccess(FileInfo info);

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();
private:
    Ui::MainPage *ui;
    QWidget* returnWindow;
    UserInfo mInfo;
};

#endif // MAINPAGE_H
