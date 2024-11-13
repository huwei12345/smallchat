#include "personcardcon.h"
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QUrl>
#include <QDialog>
#include <QMouseEvent>
#include <QQmlContext>
#include "personcache.h"
PersonCardCon::PersonCardCon(QWidget *parent)
    : QDialog{parent}
{
}

PersonCardCon::PersonCardCon(UserInfo info, QWidget *parent)
    : QDialog{parent}
{
    // 创建 QQuickWidget
    QQuickWidget *quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    // 加载 QML 文件
    quickWidget->setSource(QUrl(QStringLiteral("qrc:/PersonCard.qml")));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(quickWidget);
    setLayout(layout);
    setFixedSize(300, 150);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
    id = QString::number(info.user_id);
    name = QString::fromStdString(info.username);
    email = QString::fromStdString(info.email);
    avatar = PersonCache::GetInstance()->getPersonPhotoPath(info.user_id);
    avatar = QString("file:///") + avatar;
    bool isFriend = PersonCache::GetInstance()->isFriend(info.user_id);
    qDebug() << "ava " << avatar;
    quickWidget->rootContext()->setContextProperty("id", id);
    quickWidget->rootContext()->setContextProperty("username", name);
    quickWidget->rootContext()->setContextProperty("email", email);
    quickWidget->rootContext()->setContextProperty("avatar", avatar);
    quickWidget->rootContext()->setContextProperty("isFriend", isFriend);
    layout->setSpacing(0);
}

PersonCardCon::~PersonCardCon()
{
    qDebug() << "delete";
}

void PersonCardCon::focusOutEvent(QFocusEvent *event)
{
    // 当焦点离开此对话框时关闭
    close();
    QDialog::focusOutEvent(event);
}

