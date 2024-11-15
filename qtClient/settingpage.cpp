#include "settingpage.h"
#include "ui_settingpage.h"

#include <QShortcut>

SettingPage::SettingPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingPage)
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

SettingPage::~SettingPage()
{
    delete ui;
}

void SettingPage::setReturn(QWidget *widget)
{
    this->returnWindow = widget;
}
