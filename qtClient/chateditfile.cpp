#include "chateditfile.h"
#include "ui_editfile.h"
#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QMenu>
#include <QToolButton>
#include <QUrl>
#include <QDesktopServices>
ChatEditFile::ChatEditFile(QWidget *parent) :
    QToolButton(parent)
{
    fileName = "C:\\Users\\weiqi\\Desktop\\testAllmem44094.zip";
    fileType = "zip";
    QFileInfo fileInfo(fileName);
    setText(fileInfo.fileName());
    setIcon(QIcon(":/file.jpeg"));
    setIconSize(size());
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QToolButton::customContextMenuRequested, this, &ChatEditFile::showContextMenu);
}

ChatEditFile::~ChatEditFile()
{
}

void ChatEditFile::on_FileBtn_clicked()
{
    //打开文件存储路径

    //个别文件类型直接用某种方式打开
    qDebug() << "open : " << fileName;
    openFileDirectory(fileName);
}

void ChatEditFile::showContextMenu(const QPoint &pos) {
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


bool ChatEditFile::openFileDirectory(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile()) {
        QString directoryPath = fileInfo.absolutePath();
        QUrl url = QUrl::fromLocalFile(directoryPath);
        // 打开文件所在目录
        QDesktopServices::openUrl(url);
        return true;
    } else {
        qWarning() << "File does not exist or is not a valid file.";
        return false;
    }
    return false;
}

void ChatEditFile::resizeEvent(QResizeEvent *event)
{
    // 动态调整图标大小
    setIconSizeBasedOnButtonSize();
    QWidget::resizeEvent(event);
}

void ChatEditFile::setIconSizeBasedOnButtonSize()
{
    // 让图标大小根据 QToolButton 的大小来调整
    QSize buttonSize = size();
    buttonSize = buttonSize * 4 / 5;
    setIconSize(buttonSize);  // 设置图标为按钮尺寸的一半
}

bool ChatEditFile::build(MessageInfo *info)
{

}
