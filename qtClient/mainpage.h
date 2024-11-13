#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QFile>
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
    void StoreFileSuccess(Response response);
    bool initSpaceFileTree();
    int init();

    TreeNode* addAllSpaceFileToPage();
    TreeNode* addSpaceFileToPage(FileInfo info);
    bool deleteSpaceFileFromPage(FileInfo info);
    bool updateSpaceFileInPage(FileInfo info, bool moved = false);

    bool deleteFile(QTreeWidgetItem *item);
    bool addFile(QTreeWidgetItem *item);
    bool renameFile(QTreeWidgetItem *item);
    bool openFile(QTreeWidgetItem *item);
    bool openDir(QTreeWidgetItem *item);
    bool push(QTreeWidgetItem *item);
    bool pull(QTreeWidgetItem *item);
    bool openType(QTreeWidgetItem *item);
    bool copy(QTreeWidgetItem *item);
    bool cut(QTreeWidgetItem *item);
    bool put(QTreeWidgetItem *item);
    bool close(QTreeWidgetItem *item);
    bool createDir(QTreeWidgetItem *item);

    bool spaceHasFile(QString ClientPath);
    bool spaceHasFile(FileInfo* info);
    bool showInEdit(FileInfo* info);

    bool createDirectoryIfNotExist(const QString &dirPath);
    bool addLoaclFile(const QString &filePath);
    bool deleteLocalSpaceFile(FileInfo info);
    bool setClientDir(FileInfo &info);
    bool moveClientLocalDir(FileInfo &info);

private slots:
    void on_returnBtn_clicked();
    void on_updateBtn_clicked();
    void on_testBtn_clicked();
    void on_diffBtn_clicked();

    void findSpaceFileTreeSuccess(Response response);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void SpaceFileGetSuccess(FileInfo info);
    void ProcessDeleteFileSuccess(Response response);
    void ProcessRenameFileSuccess(Response response);

private:
    Ui::MainPage *ui;
    QWidget* returnWindow;
    UserInfo mInfo;
    int mUserId;
    std::map<int, FileInfo> mSpaceFileMap;
    TreeNode* mRoot;
    std::map<int, TreeNode*> idBook;//暂时这么写，后续肯定是只有TreeNode，每次都查找


    QString mSpaceRootDir;

    QFile* mCurrentFile;
    QTreeWidgetItem* mCurrentItem;

    QTreeWidgetItem* mCurrentCopyItem;
    QTreeWidgetItem* mCurrentCutItem;
    vector<QTreeWidgetItem*> mCurrentCopyItemList;
    vector<QTreeWidgetItem*> mCurrentCutItemList;
};

#endif // MAINPAGE_H
