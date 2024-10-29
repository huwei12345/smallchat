#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QTreeWidgetItem>
#include <QWidget>
#include "Protocol.h"
namespace Ui {
class MainPage;
}

class TreeNode {
public:
    TreeNode() : parent(nullptr), fileId(-1), item(nullptr) {}
    TreeNode(int id) : parent(nullptr), fileId(id), item(nullptr) {}
    TreeNode* parent;
    std::vector<TreeNode*> son;
    int fileId;
    QTreeWidgetItem* item;
};

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
    TreeNode* addSpaceFileToPage(FileInfo info);
    TreeNode *addAllSpaceFileToPage();

    bool editFile(QTreeWidgetItem *item);
    bool deleteFile(QTreeWidgetItem *item);
    bool storeFile(QTreeWidgetItem *item);
    bool renameFile(QTreeWidgetItem *item);
    bool openFile(QTreeWidgetItem *item);
    bool openDir(QTreeWidgetItem *item);
    bool update(QTreeWidgetItem *item);
    bool openType(QTreeWidgetItem *item);
    bool copy(QTreeWidgetItem *item);
    bool cut(QTreeWidgetItem *item);
    bool put(QTreeWidgetItem *item);
    bool close(QTreeWidgetItem *item);
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
    TreeNode* mRoot;
    std::map<int, TreeNode*> idBook;//暂时这么写，后续肯定是只有TreeNode，每次都查找
};

#endif // MAINPAGE_H
