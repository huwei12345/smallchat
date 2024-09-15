#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    void setReturn(QWidget* widget);
    ~MainPage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainPage *ui;
    QWidget* returnWindow;
};

#endif // MAINPAGE_H
