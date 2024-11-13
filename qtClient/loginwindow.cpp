#include "loginwindow.h".h"
#include "ui_mainwindow.h"
#include "mainpage.h"
#include "registerpage.h"
#include <qmessagebox.h>
#include <qshortcut.h>
#include <QPushButton>
#include <QKeyEvent>
#include <QPalette>
#include <QSystemTrayIcon>
#include "friendpage.h"
#include "qtmaterialautocomplete.h"
#include "processor.h"
#include "network.h"
#include "globalvaria.h"
#include "ftpsender.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mTrayIcon = nullptr;
    // 设置窗口图标
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));
    mCurWidget = this;
    mainPage = NULL;
    registerPage = NULL;
    mFriendPage = NULL;
    ui->pushButton_4->setShortcut(tr("return"));

    QShortcut *keyEnter = new QShortcut(QKeySequence("Enter"),ui->pushButton);
    keyEnter->setContext(Qt::WindowShortcut);
    connect(keyEnter, &QShortcut::activated,ui->pushButton,&QPushButton::click);

    ui->pushButton_3->setShortcut(tr("Esc"));
    ui->pushButton_2->setShortcut(tr("Ctrl+r"));

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/main/back.jpeg").scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setPalette(palette);

    //QPalette::setBrush(QPalette::Window, QBrush(QPixmap("path/to/image")))
    // 设置按钮的边框宽度为2像素，边框颜色为黄色
    //ui->pushButton->setStyleSheet("QPushButton { background-color: blue; color: white; border-radius: 5px; border: 2px solid yellow; }"
    //                      "QPushButton:hover { background-color: red; }");

    // 连接登录页面的登录成功信号到主页窗口的槽函数

    connect(ClientNetWork::GetInstance(), &ClientNetWork::loginSuccessful, this, &LoginWindow::onLoginSuccessful);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::loginFailure, this, &LoginWindow::onLoginFailure);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::LogoutSuccess, this, &LoginWindow::LogoutSuccess);
}

LoginWindow::~LoginWindow()
{
    delete ui;
    ClientNetWork::GetInstance()->close();
}

void LoginWindow::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if (name.compare("admin") == 0 && password.compare("123456") == 0 )
    {
        if (mainPage == NULL) {
            mainPage = new MainPage();
            mainPage->setReturn(this);
        }
        mainPage->show();
        this->hide();
    }
    else {
        if (name == "")
        {
            QMessageBox::information(this,"提示","请输入用户名");
            ui->lineEdit->setFocus();
        }
        else
        {
            if(password == ""){
                QMessageBox::information(this,"提示","请输入密码！");
                ui->lineEdit_2->setFocus();
            }
            else
            {
                QMessageBox::warning(this,"错误提示","用户名或密码错误");
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                ui->lineEdit->setFocus();
            }
        }
    }
}


void LoginWindow::on_pushButton_2_clicked()
{
    if (registerPage == NULL) {
        registerPage = new RegisterPage();
        registerPage->setReturn(this);
    }
    registerPage->show();
    this->hide();
}


void LoginWindow::on_pushButton_3_clicked()
{
    close();
}


void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Insert) // 当按下Return/Enter键时
    {
        QPushButton *button = qobject_cast<QPushButton *>(QApplication::focusWidget());
        if(button)
        {
            // 模拟按钮点击
            button->animateClick();
        }
    }
    else
    {
        // 继续处理其他按键事件
        QWidget::keyPressEvent(event);
    }
}

void LoginWindow::on_pushButton_4_clicked()
{
    std::string name = ui->lineEdit->text().toStdString();
    std::string password = ui->lineEdit_2->text().toStdString();
    if (name != "" && password != "") {
        if (name.compare("admin") != 0) {
            bool ret = Processor::Login(name, password);
            if (!ret) {
                QMessageBox::information(this,"提示","网络不可达！");
            }
        }
        //测试账号
        else if (name.compare("admin") == 0 && password.compare("123456") == 0 )
        {
            if (mFriendPage == NULL) {
                mFriendPage = new FriendPage();
                mFriendPage->setReturn(this);
                connect(mFriendPage, &FriendPage::logoutUser, this, &LoginWindow::logoutUser);
            }
            mFriendPage->show();
            //        for (int i = 0; i < 4; i++) {
            //            friendPage->addFriendToPage(i);
            //        }
            this->hide();
        }
    }
    else {
        if (name == "")
        {
            QMessageBox::information(this,"提示","请输入用户名");
            ui->lineEdit->setFocus();
        }
        else
        {
            if(password == ""){
                QMessageBox::information(this,"提示","请输入密码！");
                ui->lineEdit_2->setFocus();
            }
            else
            {
                QMessageBox::warning(this,"错误提示","用户名或密码错误");
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                ui->lineEdit->setFocus();
            }
        }
    }
}

void LoginWindow::onLoginSuccessful(UserInfo info)
{
    if (mFriendPage == NULL) {
        ::user_id = info.user_id;
        mFriendPage = new FriendPage(info);
        ClientPersonInfo* ci = ClientPersonInfo::GetInstance();
        ci->init(info);
        mFriendPage->init();
        mFriendPage->setReturn(this);
        connect(mFriendPage, &FriendPage::logoutUser, this, &LoginWindow::logoutUser);
    }
    mCurWidget = mFriendPage;
    mFriendPage->show();
    this->hide();
}

void LoginWindow::onLoginFailure()
{
    QMessageBox::warning(this,"错误提示","用户名或密码错误");
    ui->lineEdit->setFocus();
}

void LoginWindow::LogoutSuccess(Response response)
{
    mFriendPage->hide();
    show();
    mFriendPage->reset();
    delete mFriendPage;
    mFriendPage = NULL;
    ClientPersonInfo::GetInstance()->reset();
    mCurWidget = this;

    if (response.mCode) {
    }
    else {
        logoutByNetClose();
    }
}

void LoginWindow::logoutUser()
{
    qDebug() << "logout";
    if (mFriendPage != NULL) {
        bool ret = Processor::Logout();
        if (!ret) {
            logoutByNetClose();
        }
    }
    else {
        show();
        ClientPersonInfo::GetInstance()->reset();
        mCurWidget = this;
    }
}

//服务器退出session失败,无法复用连接。
//通过重启网络来退出用户，撤销服务器session，重置连接
void LoginWindow::logoutByNetClose() {
    //重启网络连接
    ClientNetWork::GetInstance()->close();
    ClientNetWork::GetInstance()->Client();

    FtpSender::GetInstance()->close();
    FtpSender::GetInstance()->start();
}

bool LoginWindow::initTrayIcon(QApplication* app) {
    // 创建系统托盘图标
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/main/icon.jpeg"), app);
    mTrayIcon = trayIcon;
    trayIcon->setToolTip("QFei");
    // 创建菜单项
    QAction *showAction = new QAction("Show", app);
    QAction *quitAction = new QAction("Quit", app);

    // 创建菜单并添加菜单项
    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    // 将菜单设置给系统托盘图标
    trayIcon->setContextMenu(trayMenu);

    // 显示系统托盘图标
    trayIcon->show();

    // 响应菜单项的槽函数
    QObject::connect(showAction, &QAction::triggered, [&](){
        // 执行显示主窗口的操作
        // 例如：mainwindow->show();
        mCurWidget->showNormal(); // 显示正常窗口
        mCurWidget->activateWindow(); // 激活窗口
    });

    QObject::connect(quitAction, &QAction::triggered, [&](){
        // 执行退出应用程序的操作
        QApplication::quit();
    });

    // 当用户双击系统托盘图标时，显示程序界面
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            mCurWidget->showNormal(); // 显示正常窗口
            mCurWidget->activateWindow(); // 激活窗口
        }
    });

    // 当窗口要被最小化时，隐藏窗口并显示托盘图标
    QObject::connect(this, &QWidget::windowIconChanged, [&](const QIcon &icon){
        if (icon.isNull()) {
            trayIcon->show();
            mCurWidget->hide();
            this->hide();
        }
    });
    // 当点击托盘图标时，恢复窗口
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            // 鼠标单击托盘图标
            mCurWidget->showNormal(); // 或 window.show();
            mCurWidget->activateWindow();
        } else if (reason == QSystemTrayIcon::Context) {
            // 鼠标右键点击托盘图标
            qDebug() << "Tray icon context menu activated";
        } else if (reason == QSystemTrayIcon::MiddleClick) {
            // 鼠标中键点击托盘图标
            qDebug() << "Tray icon middle clicked";
        } else if (reason == QSystemTrayIcon::Unknown) {
            // 其他原因
            qDebug() << "Unknown reason for tray icon activation";
        }
    });
    return true;
}
