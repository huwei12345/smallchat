#ifndef CREATEGROUPPAGE_H
#define CREATEGROUPPAGE_H

#include <QWidget>

namespace Ui {
class CreateGroupPage;
}

class CreateGroupPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroupPage(QWidget *parent = nullptr);
    ~CreateGroupPage();
    QWidget* returnWindow;
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::CreateGroupPage *ui;
};

#endif // CREATEGROUPPAGE_H
