#include "creategrouppage.h"
#include "ui_creategrouppage.h"
#include <QKeyEvent>
#include "network.h"
CreateGroupPage::CreateGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroupPage)
{
    ui->setupUi(this);
    returnWindow = NULL;
    connect(ClientNetWork::GetInstance(), &ClientNetWork::createGroupSuccess, this, &CreateGroupPage::createGroupSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::applyJoinGroupSuccess, this, &CreateGroupPage::applyJoinGroupSuccess);
}

CreateGroupPage::~CreateGroupPage()
{
    delete ui;
}

void CreateGroupPage::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) // 当按下Return/Enter键时
    {
        hide();
        returnWindow->show();
    }
    else
    {
        // 继续处理其他按键事件
        QWidget::keyPressEvent(event);
    }
}

void CreateGroupPage::createGroupSuccess(Response rsp)
{

}

void CreateGroupPage::applyJoinGroupSuccess(Response rsp)
{

}
