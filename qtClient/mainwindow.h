#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Protocol.h"
class MainPage;
class RegisterPage;
class FriendPage;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


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


public slots:


private:
    Ui::MainWindow *ui;
    MainPage* mainPage;
    FriendPage* friendPage;
    RegisterPage* registerPage;
};
#endif // MAINWINDOW_H
