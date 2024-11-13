#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "Protocol.h"
class MainPage;
class RegisterPage;
class FriendPage;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    bool initTrayIcon(QApplication *app);
    QSystemTrayIcon *mTrayIcon;

    void logoutByNetClose();
protected:
    // 在你的QApplication或者QWidget子类中重载keyPressEvent
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    //handleLoginRequest();
    void on_pushButton_4_clicked();

    void onLoginSuccessful(UserInfo);
    void onLoginFailure();
    void LogoutSuccess(Response response);
    void logoutUser();

public slots:


private:
    Ui::MainWindow *ui;
    MainPage* mainPage;
    FriendPage* mFriendPage;
    RegisterPage* registerPage;
    QWidget* mCurWidget;
};
#endif // LOGINWINDOW_H
