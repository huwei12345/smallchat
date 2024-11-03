#include "mainpage.h"
#include "ui_mainpage.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMessageBox>
#include <QSplitter>
#include <QInputDialog>
#include "processor.h"
#include "ftpsender.h"
#include "network.h"
#include <bits/stl_set.h>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));
    returnWindow = NULL;
    mRoot = nullptr;
    ui->pushButton->setShortcut(tr("Esc"));
    connect(ClientNetWork::GetInstance(), &ClientNetWork::storeFileResponse, this, &MainPage::StoreFileAllow);
    //ui->treeWidget->setHeaderLabel("Tree Example");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 设置上下文菜单策略

    connect(ClientNetWork::GetInstance(), &ClientNetWork::findSpaceFileTreeSuccess, this, &MainPage::findSpaceFileTreeSuccess);


    // 创建一个垂直布局
    QWidget* widget = ui->widget_2;

    // 创建一个 QSplitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // 添加左侧和右侧的部件
    QWidget *leftWidget = ui->treeWidget;
    QWidget *rightWidget = ui->widget;

    // 将部件添加到分隔器
    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);

    // 设置分隔条的初始大小
    splitter->setSizes(QList<int>({200, 200}));  // 左右部件的初始宽度

    // 将分隔器添加到布局
    QLayout* layout = widget->layout();
    layout->addWidget(splitter);
    // 设置主窗口的布局
}

MainPage::MainPage(UserInfo info, QWidget *parent) :
    MainPage(parent)
{
    mInfo = info;
    mUserId = info.user_id;
}

int MainPage::init() {
    int ret = initSpaceFileTree();
    return ret;
}

bool MainPage::initSpaceFileTree() {
    bool ret = Processor::findSpaceFileTree(mUserId);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return ret;
}

void MainPage::setReturn(QWidget *widget)
{
    this->returnWindow = widget;
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_pushButton_clicked()
{
    this->hide();
    returnWindow->show();
}

void MainPage::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),
        "",
        tr("All Files (*);;Text Files (*.txt)"));
    //D:/easybcd.zip
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
        return;
    }
    FileInfo info;
    info.Generate();
    info.ClientPath = fileName.toStdString();
    info.serverPath = std::string("userInfo/") + std::to_string(user_id) + std::string("/");
    QFileInfo fileInfo(QString::fromStdString(info.ClientPath));

    // 获取文件名（包括扩展名）
    info.serverFileName = fileInfo.fileName().toStdString();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.expiredTime = 3600 * 24 * 15;
    info.parentId = mInfo.storage_id;//TODO:
    info.serviceType = FileServerType::STOREFILE;
    info.fileType = "text";
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
}

//Response: 告知允许上传，及部分参数
void MainPage::StoreFileAllow(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int ret = response.mCode;
    int ftpTaskId = -1;
    stream2 >> ftpTaskId;
    FileInfo info = FtpSender::GetInstance()->file(ftpTaskId);
    stream2 >> info.id >> info.serverPath >> info.serverFileName;
    if (ret) {
        FtpSender::GetInstance()->SendFile(info);//ftp发送队列异步发送
    }
    else {
            QMessageBox::information(this, "提示", "Server is not allow SendFile");
    }
}


void MainPage::StoreFileSuccess(FileInfo info)
{
    QMessageBox::information(this, "提示", "SendFile %s Over");
    //根据info，显示在界面上
    QTreeWidgetItem* parentItem = nullptr;
    if (idBook.count(info.parentId)) {
        parentItem = idBook[info.parentId]->item;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, QString::fromStdString(info.serverFileName));
    item->setText(1, QString::fromStdString(info.serverPath));
    item->setText(2, QString::number(info.filesize));
    if (parentItem == nullptr) {
        ui->treeWidget->addTopLevelItem(item);
    }
    else {
        parentItem->addChild(item);
    }
}

void MainPage::on_toolButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),
        "",
        tr("All Files (*);;Text Files (*.txt)"));
    //D:/easybcd.zip
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
        return;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem;
    FileInfo info;
    info.ClientPath = fileName.toStdString();
    info.serverPath = std::string("userInfo/") + std::to_string(user_id) + std::string("/");
    QFileInfo fileInfo(QString::fromStdString(info.ClientPath));
    // 获取文件名（包括扩展名）
    info.serverFileName = fileInfo.fileName().toStdString();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.expiredTime = 3600 * 24 * 15;
    info.parentId = mInfo.storage_id;//TODO:
    info.serviceType = FileServerType::STOREFILE;
    info.fileType = "text";
    item->setText(0, QString::fromStdString(info.serverFileName));
    item->setText(1, QString::fromStdString(info.serverPath));
    item->setText(2, QString::number(info.filesize));
    ui->treeWidget->addTopLevelItem(item);
    QTreeWidgetItem* item2 = new QTreeWidgetItem;
    item2->setText(0, QString::fromStdString(info.ClientPath));
    item->addChild(item2);
//    ui->treeWidget->expandAll(); // 可选：展开所有项以查看新添加的子项
//    ui->treeWidget->update(); // 重新绘制树
}

void MainPage::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "Single";
    Q_UNUSED(item)
    Q_UNUSED(column)
    //QMessageBox::information(this, "提示", "Single");
}


void MainPage::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    QMessageBox::information(this, "提示", "Double");
}

void MainPage::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    qDebug() << "Right";
    if (item) {
        qDebug() << "Right2";
        QMenu contextMenu("Context Menu", ui->treeWidget);

        // 添加自定义菜单项
        QAction *openAction = contextMenu.addAction("打开");
        QAction *editAction = contextMenu.addAction("修改");
        QAction *deleteAction = contextMenu.addAction("删除");
        QAction *addAction = contextMenu.addAction("添加");
        QAction *createDirAction = contextMenu.addAction("创建目录");
        QAction *renameAction = contextMenu.addAction("重命名");
        QAction *openDirAction = contextMenu.addAction("打开所在目录");
        QAction *updateAction = contextMenu.addAction("更新");
        QAction *openTypeAction = contextMenu.addAction("用...方式打开");
        QAction *copyAction = contextMenu.addAction("复制");
        QAction *cutAction = contextMenu.addAction("剪切");
        QAction *putAction = contextMenu.addAction("粘贴");
        QAction *closeAction = contextMenu.addAction("关闭");
        // 执行菜单
        QAction *selectedAction = contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
        if (selectedAction == editAction) {
            editFile(item);
        } else if (selectedAction == deleteAction) {
            deleteFile(item);
        } else if (selectedAction == addAction) {
            storeFile(item);
        } else if (selectedAction == createDirAction) {
            createDir(item);
        } else if (selectedAction == renameAction) {
            renameFile(item);
        } else if (selectedAction == openAction) {
            openFile(item);
        } else if (selectedAction == openDirAction) {
            openDir(item);
        } else if (selectedAction == updateAction) {
            update(item);
        } else if (selectedAction == openTypeAction) {
            openType(item);
        } else if (selectedAction == copyAction) {
            copy(item);
        } else if (selectedAction == cutAction) {
            cut(item);
        } else if (selectedAction == putAction) {
            put(item);
        } else if (selectedAction == closeAction) {
            close(item);
        }
    }
}

TreeNode* MainPage::addAllSpaceFileToPage() {
    //O(n^2)遍历建树，层次

    TreeNode* node = nullptr;
    TreeNode* root = nullptr;
    for (int i = 0; i < (int)mSpaceFileMap.size(); i++) {
        //最多进行size次
        for (auto& son : mSpaceFileMap) {
            if (idBook.count(son.second.id)) {
                continue;
            }
            if (son.second.parentId == 0) {
                //root
                root = new TreeNode(son.second.id);
                root->parent = nullptr;
                idBook[root->fileId] = root;

                QTreeWidgetItem* item = new QTreeWidgetItem;

                item->setText(0, QString::fromStdString(son.second.serverFileName));
                item->setText(1, QString::fromStdString(son.second.serverPath));
                item->setText(2, QString::number(son.second.filesize));
                root->item = item;
                ui->treeWidget->addTopLevelItem(item);
                item->setWhatsThis(0, QString::number(root->fileId));
            }
            else if (idBook.count(son.second.parentId)) {
                node = new TreeNode(son.second.id);
                node->parent = idBook[son.second.parentId];
                idBook[son.second.parentId]->son.push_back(node);
                idBook[son.second.id] = node;

                QTreeWidgetItem* item = new QTreeWidgetItem;
                item->setText(0, QString::fromStdString(son.second.serverFileName));
                item->setText(1, QString::fromStdString(son.second.serverPath));
                item->setText(2, QString::number(son.second.filesize));
                node->item = item;
                if (node->parent->item) {
                    node->parent->item->addChild(item);
                    item->setWhatsThis(0, QString::number(son.second.id));
                }
                else {
                    qDebug() << "Error parent has no widget";
                }
            }
        }
    }
    return root;
}

TreeNode *MainPage::addSpaceFileToPage(FileInfo info)
{
    int parentId = info.parentId;
    TreeNode* node = nullptr;
    if (idBook.count(info.id) && idBook[info.id] != nullptr)
        return idBook[info.id];
    if (idBook.count(parentId)) {
        TreeNode* pNode = idBook[parentId];
        if (pNode == nullptr)
            return nullptr;
        node = new TreeNode(info.id);
        node->parent = idBook[parentId];
        idBook[parentId]->son.push_back(node);
        idBook[info.id] = node;
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, QString::fromStdString(info.serverFileName));
        item->setText(1, QString::fromStdString(info.serverPath));
        item->setText(2, QString::number(info.filesize));
        node->item = item;
        if (node->parent->item) {
            node->parent->item->addChild(item);
        }
        else {
            qDebug() << "Error parent has no widget";
        }
    }
    return node;
}
//findAllFile
void MainPage::findSpaceFileTreeSuccess(Response response) {
    std::string mdata = response.mData;
    MyProtocolStream stream(mdata);
    int size = 0;
    stream >> size;
    for (int i = 0; i < size; i++) {
        //TODO:ToolButton put in widget
        FileInfo info;
            stream
            >> info.id
            >> info.parentId
            >> info.serverFileName
            >> info.fileType
            >> info.serverPath
            >> info.timestamp
            //>> fileList[i].updated_at
            >> info.expiredTime;
        std::cout << info.serverPath << info.serverFileName << std::endl;
        mSpaceFileMap[info.id] = info;
    }

    mRoot = addAllSpaceFileToPage();
    if (!mRoot) {
        qDebug() << "init Space File Tree Error";
    }
}



//EDITSTOREFILE                             = 35,
//跟新，直接替换
bool MainPage::editFile(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    Processor::SendFile(info);
    QMessageBox::information(this, "Edit Action", QString("Editing: %1").arg(item->text(0)));
    return true;
}
bool MainPage::deleteFile(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    Processor::DeleteFile(info);
    QMessageBox::information(this, "Delete Action", QString("Deleting: %1").arg(item->text(0)));
    item->parent()->removeChild(item);
    return true;
}
bool MainPage::storeFile(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    QMessageBox::information(this, "Add Action", QString("Deleting: %1").arg(item->text(0)));
    return true;
}
bool MainPage::createDir(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    int fileId = item->whatsThis(0).toInt();
    FileInfo pInfo = mSpaceFileMap[fileId];
    QString dirName = QInputDialog::getText(this, "提示", "请输入目录名");
    FileInfo info;
    //info.Generate();
    info.ClientPath = pInfo.ClientPath + "/" + dirName.toStdString();
    qDebug() << "oooooooooooooooooooooo" << QString::fromStdString(pInfo.fileType);
    if (pInfo.fileType == std::string("rootdir")) {
        info.serverPath = pInfo.serverPath;
    }
    else {
        info.serverPath = pInfo.serverPath + pInfo.serverFileName + "/";
    }

    // 获取文件名（包括扩展名）
    info.serverFileName = dirName.toStdString();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.expiredTime = 3600 * 24 * 15;
    info.parentId = pInfo.id;//TODO:
    info.id = 0;
    info.serviceType = FileServerType::STOREFILE;
    info.fileType = "dir";
    //FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    addSpaceFileToPage(info);
    QMessageBox::information(this, "createDir Action", QString("createDir: %1").arg(item->text(0)));
    return true;
}
bool MainPage::renameFile(QTreeWidgetItem* item) {
    QString newName = QInputDialog::getText(this, "提示", "请输入要修改的名称");
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    if (!newName.isEmpty()) {
        info.serverFileName = newName.toStdString();
        Processor::RenameFile(info);
    }
    QMessageBox::information(this, "Rename Action", QString("Deleting: %1").arg(item->text(0)));
    return true;
}
bool MainPage::openFile(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    return true;
}
bool MainPage::openDir(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::update(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::openType(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::copy(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::cut(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::put(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
bool MainPage::close(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}
