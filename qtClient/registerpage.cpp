#include "registerpage.h"
#include "ui_registerpage.h"
#include <QShortcut>
#include <QMessageBox>
#include "processor.h"
RegisterPage::RegisterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));
    returnWindow = NULL;
    QShortcut* shortcut = new QShortcut(this);
    //设置键值，也就是设置快捷键.
    shortcut->setKey(QKeySequence("Esc"));
    //设置是否会自动反复按键,也就是说:当你一直按住键盘Ctrl+1时,会一直不停的调用对应的槽函数.
    shortcut->setAutoRepeat(false);
    connect(shortcut, &QShortcut::activated, this, [=]() {
            this->hide();
            returnWindow->show();
        });
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::setReturn(QWidget *widget)
{
    this->returnWindow = widget;
}

void RegisterPage::on_pushButton_2_clicked()
{
    this->hide();
    returnWindow->show();
}


void RegisterPage::on_pushButton_clicked()
{
    std::string name = ui->lineEdit->text().toStdString();
    std::string password = ui->lineEdit_2->text().toStdString();
    std::string passwordCopy = ui->lineEdit_3->text().toStdString();
    std::string email = ui->lineEdit_4->text().toStdString();
    if (name == "")
    {
        QMessageBox::information(this,"提示","请输入用户名!");
        ui->lineEdit->setFocus();
    }
    else if (password == "") {
        QMessageBox::information(this,"提示","请输入密码!");
        ui->lineEdit_2->setFocus();
    }
    else if (passwordCopy == "") {
        QMessageBox::information(this,"提示","请重复输入密码！");
        ui->lineEdit_3->setFocus();
    }
    else if (passwordCopy != password) {
        QMessageBox::information(this,"提示","两次输入密码不一致！");
        ui->lineEdit_3->setFocus();
    }
    else if (email == "") {
        QMessageBox::information(this,"提示","请输入邮箱！");
        ui->lineEdit_4->setFocus();
    }
    else if (!isalpha(name[0])) {
        QMessageBox::information(this,"提示","用户名必须以字母开头！");
        ui->lineEdit_4->setFocus();
    }
    else if (name.size() < 5) {
        QMessageBox::information(this,"提示","用户名必须大于5个字符！");
        ui->lineEdit_4->setFocus();
    }
    else {
        bool ret = Processor::Register(name, password, email);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
        }
    }
}

