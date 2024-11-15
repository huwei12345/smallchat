#include "loginsettingpage.h"
#include "loginwindow.h"
#include "ui_loginsettingpage.h"

#include <QSettings>
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
    QSettings settings("config.ini", QSettings::IniFormat);
    // 打印读取的值
    QString mServerIp = settings.value("global/SERVERIP").toString();
    ui->serverIpEdit->setText(mServerIp);
    QString mFtpIp = settings.value("global/FTPIP").toString();
    ui->ftpIpEdit->setText(mFtpIp);
    int mFtpPort = settings.value("global/FTPPORT").toUInt();
    ui->ftpPortEdit->setText(QString::number(mFtpPort));
    QString mFtpUser = settings.value("global/FTPUSER").toString();
    ui->ftpUserEdit->setText(mFtpUser);
    QString mFtpPassword = settings.value("global/FTPPASSWORD").toString();
    ui->ftpPasswdEdit->setText(mFtpPassword);
    QString mFtpType = settings.value("global/FTPTYPE").toString();
}

void LoginSettingPage::resetLocalSetting() {

}

void LoginSettingPage::clearEditing() {

}

void LoginSettingPage::storeSettingLocal() {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("global/SERVERIP", ui->serverIpEdit->text());
    settings.setValue("global/FTPIP", ui->ftpIpEdit->text());
    settings.setValue("global/FTPPORT", ui->ftpPortEdit->text());
    settings.setValue("global/FTPUSER", ui->ftpUserEdit->text());
    settings.setValue("global/FTPPASSWORD", ui->ftpPasswdEdit->text());
    //TODO:其实应该在点击登录后再进行网络连接，或者在进行注册时，或者在应用修改后重启网络连接
    //网络连接，应尽量简单
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
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("global/SERVERIP", "192.168.58.132");
    settings.setValue("global/FTPIP", "192.168.58.132");
    settings.setValue("global/FTPPORT", 21);
    settings.setValue("global/FTPUSER", "huwei");
    settings.setValue("global/FTPPASSWORD", "123456");
    getLocalSetting();
    ((LoginWindow*)returnWindow)->loadSetting();
}


void LoginSettingPage::on_applyBtn_clicked()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("global/SERVERIP", ui->serverIpEdit->text());
    settings.setValue("global/FTPIP", ui->ftpIpEdit->text());
    settings.setValue("global/FTPPORT", ui->ftpPortEdit->text());
    settings.setValue("global/FTPUSER", ui->ftpUserEdit->text());
    settings.setValue("global/FTPPASSWORD", ui->ftpPasswdEdit->text());

    ((LoginWindow*)returnWindow)->loadSetting();
}


void LoginSettingPage::on_openProxyBtn_clicked()
{
    ui->proxyTypeEdit->setEnabled(true);
    ui->proxyIPEdit->setEnabled(true);
}


void LoginSettingPage::on_closeProxyBtn_clicked()
{
    ui->proxyTypeEdit->setEnabled(false);
    ui->proxyIPEdit->setEnabled(false);
}

