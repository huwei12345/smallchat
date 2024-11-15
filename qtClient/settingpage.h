#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>

namespace Ui {
class settingPage;
}

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage();

    void setReturn(QWidget *widget);

private:
    Ui::settingPage *ui;
    QWidget* returnWindow;
};

#endif // SETTINGPAGE_H
