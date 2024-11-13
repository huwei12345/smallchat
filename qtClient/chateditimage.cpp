#include "chateditimage.h"
#include "ui_editimage.h"

#include <QAction>
#include <QFileInfo>
#include <QMenu>
#include <QToolButton>
#include <QDebug>

ChatEditImage::ChatEditImage(QWidget *parent) :
    QToolButton(parent)
{
    fileName = "D:\\back.jpeg";
    fileType = "picture";
    QFileInfo fileInfo(fileName);
    //ui->ImageBtn->setText(fileInfo.fileName());
    //显示缩略图
    setIcon(QIcon("D:\\back.jpeg"));
    setIconSize(size());
    setToolButtonStyle(Qt::ToolButtonIconOnly);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QToolButton::customContextMenuRequested, this, &ChatEditImage::showContextMenu);
}

ChatEditImage::~ChatEditImage()
{
}

void ChatEditImage::on_ImageBtn_clicked()
{
    //显示大图
    qDebug() << "open : " << fileName;
}

void ChatEditImage::showContextMenu(const QPoint &pos) {
    // 创建右键菜单
    QMenu *menu = new QMenu(this);

    // 添加菜单项
    QAction *action1 = new QAction("复制", this);
    QAction *action2 = new QAction("转发", this);
    QAction *action3 = new QAction("收藏", this);
    QAction *action4 = new QAction("多选", this);
    QAction *action5 = new QAction("引用", this);
    QAction *action6 = new QAction("编辑", this);
    QAction *action7 = new QAction("另存为", this);
    QAction *action8 = new QAction("在文件夹中显示", this);
    QAction *action9 = new QAction("打开方式", this);
    QAction *action10 = new QAction("删除", this);

    // 将菜单项添加到菜单
    menu->addAction(action1);
    menu->addAction(action2);
    menu->addAction(action3);
    menu->addAction(action4);
    menu->addAction(action5);
    menu->addSeparator();
    menu->addAction(action6);
    menu->addAction(action7);
    menu->addAction(action8);
    menu->addAction(action9);
    menu->addSeparator();
    menu->addAction(action10);

    // 连接菜单项的点击事件
    connect(action1, &QAction::triggered, this, [](){
        qDebug("菜单项 1 被点击");
    });
    connect(action2, &QAction::triggered, this, [](){
        qDebug("菜单项 2 被点击");
    });
    connect(action3, &QAction::triggered, this, [](){
        qDebug("菜单项 3 被点击");
    });
    // 弹出菜单并显示在右键点击的位置
    menu->exec(QCursor::pos());
}

bool ChatEditImage::build(MessageInfo *info)
{

}

void ChatEditImage::resizeEvent(QResizeEvent *event)
{
    // 动态调整图标大小
    setIconSizeBasedOnButtonSize();
    QWidget::resizeEvent(event);
}

void ChatEditImage::setIconSizeBasedOnButtonSize()
{
    // 让图标大小根据 QToolButton 的大小来调整
    QSize buttonSize = size();
    setIconSize(buttonSize);  // 设置图标为按钮尺寸的一半
}
