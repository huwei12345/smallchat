#include "creategrouppage.h"
#include "ui_creategrouppage.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "network.h"
#include "processor.h"

extern int user_id;
CreateGroupPage::CreateGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroupPage)
{
    ui->setupUi(this);
    returnWindow = NULL;
    connect(ClientNetWork::GetInstance(), &ClientNetWork::createGroupSuccess, this, &CreateGroupPage::createGroupSuccess);
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
    if (rsp.mCode) {
        QMessageBox::information(this, "群组通知", "创建群成功！");
    }
    else {
        QMessageBox::information(this, "群组通知", "创建群失败！");
    }
}

void CreateGroupPage::on_pushButton_clicked()
{
    GroupInfo info;
    info.group_name = ui->lineEditName->text().toStdString();
    info.gtype = ui->lineEditType->text().toStdString();
    info.admin_id = user_id;
    info.description = ui->lineEditIntro->text().toStdString();
    info.tips = ui->lineEditTips->text().toStdString();
    Processor::CreateGroup(info);

}
