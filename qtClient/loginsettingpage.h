#ifndef LOGINSETTINGPAGE_H
#define LOGINSETTINGPAGE_H

#include <QWidget>

namespace Ui {
class LoginSettingPage;
}

class LoginSettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginSettingPage(QWidget *parent = nullptr);
    ~LoginSettingPage();
    void setReturn(QWidget *widget);

    void getLocalSetting();
    void resetLocalSetting();
    void clearEditing();
    void storeSettingLocal();
private slots:
    void on_returnBtn_clicked();

    void on_restoreBtn_clicked();

    void on_applyBtn_clicked();

private:
    Ui::LoginSettingPage *ui;
    QWidget* returnWindow;

    std::map<int, QVariant> mSetting;//当前配置 = local配置
    std::map<int, QVariant> mInitSetting;//出厂设置配置
    std::map<int, QVariant> editSeting;//编辑器配置
};

#endif // LOGINSETTINGPAGE_H
