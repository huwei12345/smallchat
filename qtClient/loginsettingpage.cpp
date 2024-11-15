#include "loginsettingpage.h"
#include "ui_loginsettingpage.h"

#include <QShortcut>

LoginSettingPage::LoginSettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginSettingPage)
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

LoginSettingPage::~LoginSettingPage()
{
    delete ui;
}

void LoginSettingPage::getLocalSetting() {

}

void LoginSettingPage::resetLocalSetting() {

}

void LoginSettingPage::clearEditing() {

}

void LoginSettingPage::storeSettingLocal() {

}

void LoginSettingPage::setReturn(QWidget *widget)
{
    this->returnWindow = widget;
}


void LoginSettingPage::on_returnBtn_clicked()
{
    this->hide();
    returnWindow->show();
}


void LoginSettingPage::on_restoreBtn_clicked()
{

}


void LoginSettingPage::on_applyBtn_clicked()
{

}

