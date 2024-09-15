#include "creategrouppage.h"
#include "ui_creategrouppage.h"
#include <QKeyEvent>

CreateGroupPage::CreateGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGroupPage)
{
    ui->setupUi(this);
    returnWindow = NULL;
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
