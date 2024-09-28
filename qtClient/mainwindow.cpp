#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainpage.h"
#include "registerpage.h"
#include <qmessagebox.h>
#include <qshortcut.h>
#include <QPushButton>
#include <QKeyEvent>
#include <QPalette>
#include "friendpage.h"
#include "qtmaterialautocomplete.h"
#include "processor.h"
#include "network.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口图标
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));

    mainPage = NULL;
    registerPage = NULL;
    friendPage = NULL;
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

    connect(ClientNetWork::GetInstance(), &ClientNetWork::loginSuccessful, this, &MainWindow::onLoginSuccessful);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::loginFailure, this, &MainWindow::onLoginFailure);
}

MainWindow::~MainWindow()
{
    delete ui;
    ClientNetWork::GetInstance()->close();
}

void MainWindow::on_pushButton_clicked()
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


void MainWindow::on_pushButton_2_clicked()
{
    if (registerPage == NULL) {
        registerPage = new RegisterPage();
        registerPage->setReturn(this);
    }
    registerPage->show();
    this->hide();
}


void MainWindow::on_pushButton_3_clicked()
{
    close();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
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

void MainWindow::on_pushButton_4_clicked()
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
            if (friendPage == NULL) {
                friendPage = new FriendPage();
                friendPage->setReturn(this);
            }
            friendPage->show();
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

void MainWindow::onLoginSuccessful(UserInfo info)
{
    if (friendPage == NULL) {
        ::user_id = info.user_id;
        friendPage = new FriendPage(info);
        friendPage->init();
        friendPage->setReturn(this);
    }
    friendPage->show();
    this->hide();
}

void MainWindow::onLoginFailure()
{
    QMessageBox::warning(this,"错误提示","用户名或密码错误");
    ui->lineEdit->setFocus();
}

