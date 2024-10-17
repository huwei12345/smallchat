#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QTreeWidgetItem>
#include <QWidget>
#include "Protocol.h"
namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    explicit MainPage(UserInfo info, QWidget *parent = nullptr);
    void setReturn(QWidget* widget);
    ~MainPage();
    void StoreFileAllow(Response response);
    void StoreFileSuccess(FileInfo info);
    bool initSpaceFileTree();
    int init();
    void addSpaceFileToPage(int i, FileInfo info);

private slots:
    void on_pushButton_clicked();
    void findSpaceFileTreeSuccess(Response response);

    void on_toolButton_clicked();
    void on_toolButton_3_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainPage *ui;
    QWidget* returnWindow;
    UserInfo mInfo;
    int mUserId;
    std::map<int, FileInfo> mSpaceFileMap;
};

#endif // MAINPAGE_H
