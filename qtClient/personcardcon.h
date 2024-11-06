#ifndef PERSONCARDCON_H
#define PERSONCARDCON_H

#include "Protocol.h"

#include <QDialog>
#include <QMouseEvent>
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QWidget>

class IconProvider : public QObject {
    Q_OBJECT

public:
    explicit IconProvider(QObject *parent = nullptr) : QObject(parent) {}

    QImage iconToImage(const QIcon &icon) {
        return icon.pixmap(64, 64).toImage(); // 你可以设置你需要的大小
    }
};

class PersonCardCon : public QDialog
{
    Q_OBJECT
public:
    explicit PersonCardCon(QWidget *parent = nullptr);
    explicit PersonCardCon(UserInfo info, QWidget *parent = nullptr);
    ~PersonCardCon();
protected:
    void focusOutEvent(QFocusEvent *event) override;
    Q_INVOKABLE void myFunction() {
        // 你可以在这里添加 C++ 逻辑
    }

signals:

private:
    IconProvider* iconProvider;
    QString id;
    QString name;
    QString email;
    QString avatar;
};

#endif // PERSONCARDCON_H
