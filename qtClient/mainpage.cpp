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
#include <QDesktopServices>
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
    mCurrentCopyItem = nullptr;
    mCurrentCutItem = nullptr;
    mCurrentFile = nullptr;
    mCurrentItem = nullptr;
    //D:/BaiduNetdiskDownload/untitled2/debug/userInfo/1/space/;
    mSpaceRootDir = QCoreApplication::applicationDirPath() + "/" + "userInfo/" + QString::number(::user_id) + "/space/";
    ui->returnBtn->setShortcut(tr("Esc"));
    connect(ClientNetWork::GetInstance(), &ClientNetWork::storeFileResponse, this, &MainPage::StoreFileAllow);
    //ui->treeWidget->setHeaderLabel("Tree Example");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 设置上下文菜单策略
    //ui->treeWidget->
    connect(ClientNetWork::GetInstance(), &ClientNetWork::findSpaceFileTreeSuccess, this, &MainPage::findSpaceFileTreeSuccess);

    connect(ClientNetWork::GetInstance(), &ClientNetWork::SpaceFileGetSuccess, this, &MainPage::SpaceFileGetSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ProcessDeleteFileSuccess, this, &MainPage::ProcessDeleteFileSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::ProcessRenameFileSuccess, this, &MainPage::ProcessRenameFileSuccess);

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
    int ret = createDirectoryIfNotExist(mSpaceRootDir);
    if (!ret) return -1;
    ret = initSpaceFileTree();
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

void MainPage::on_returnBtn_clicked()
{
    this->hide();
    returnWindow->show();
}

//上传
void MainPage::on_updateBtn_clicked()
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
    info.parentId = mInfo.storage_id;
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    if (item != nullptr) {
        int parentId = item->whatsThis(0).toInt();
        if (parentId > 0) {
            if (mSpaceFileMap[parentId].fileType == std::string("rootdir") || mSpaceFileMap[parentId].fileType == std::string("dir")) {
                info.parentId = parentId;
                info.serverPath = mSpaceFileMap[parentId].serverPath + mSpaceFileMap[parentId].serverFileName + "/";
            }
            else {
                info.parentId = mSpaceFileMap[parentId].parentId;
                info.serverPath = mSpaceFileMap[info.parentId].serverPath + mSpaceFileMap[info.parentId].serverFileName + "/";
            }
        }
    }
    info.serviceType = FileServerType::STOREFILE;
    //TODO:此处的fileType应该通过判断文件，进行更改
    info.fileType = "text";
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
}


//Response: 告知允许上传，及部分参数，开始上传
void MainPage::StoreFileAllow(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream2(mdata);
    int ret = response.mCode;
    int ftpTaskId = -1;
    stream2 >> ftpTaskId;
    FileInfo info = FtpSender::GetInstance()->file(ftpTaskId);
    stream2 >> info.id >> info.serverPath >> info.serverFileName;
    if (info.fileType == std::string("rootdir") || info.fileType == std::string("dir")) {
        addSpaceFileToPage(info);
        mSpaceFileMap[info.id] = info;
        FtpSender::GetInstance()->removeFile(info);
        return;
    }
    if (ret) {
        FtpSender::GetInstance()->SendFile(info);//ftp发送队列异步发送
    }
    else {
            QMessageBox::information(this, "提示", "Server is not allow SendFile");
    }
}

//传输完毕
void MainPage::StoreFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream(mdata);
    bool ret = response.mCode;
    FileInfo info;
    if (ret) {
        int ftptaskId = 0;
        stream >> ftptaskId;
        info = FtpSender::GetInstance()->file(ftptaskId);
        stream >> info.id;
    }
    else {
        return;
    }
    QMessageBox::information(this, "提示", "SendFile %s Over");
    addSpaceFileToPage(info);
}

void MainPage::on_testBtn_clicked()
{

}

void MainPage::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "Single";
    mCurrentItem = item;
    int fileId = item->whatsThis(0).toInt();
    FileInfo &info = mSpaceFileMap[fileId];
    qDebug() << info.id << "kkk" << info.parentId << "kkk" << QS(info.ClientPath);
    Q_UNUSED(item)
    Q_UNUSED(column)
}

//双击显示
void MainPage::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(item)
    Q_UNUSED(column)
    int fileId = item->whatsThis(0).toInt();
    FileInfo &info = mSpaceFileMap[fileId];
    if (info.fileType == std::string("rootdir") || info.fileType == std::string("dir")) {
        return;
    }
    QMessageBox::information(this, "提示", "Double");
    QString idx = QString("userInfo/") + QN(::user_id) + "/";
    qDebug() << "idx" << idx << QS(info.serverPath);
    if (QS(info.serverPath).startsWith(idx)) {
        QString clientPath = mSpaceRootDir + QS(info.serverPath).mid(idx.size()) + QS(info.serverFileName);
        qDebug() << clientPath;
        info.ClientPath = clientPath.toStdString();
    }
    else {
        return;
    }
    if (spaceHasFile(&info)) {
        showInEdit(&info);
    }
    else {
        qDebug() << "Local has Not File, Now DownLoad start..." << QS(info.serverPath) << QS(info.serverFileName)
                 << "in " << QS(info.ClientPath);
        //服务类型其实不需要知道
        info.serviceType = FileServerType::GETSTOREFILE;
        info.Generate();
        FtpSender::GetInstance()->addFile(info);
        Processor::GetFile(info);
    }
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
        QAction *addAction = contextMenu.addAction("添加");
        QAction *deleteAction = contextMenu.addAction("删除");
        QAction *createDirAction = contextMenu.addAction("创建目录");
        QAction *renameAction = contextMenu.addAction("重命名");
        QAction *openDirAction = contextMenu.addAction("打开所在目录");
        QAction *pushAction = contextMenu.addAction("推送");
        QAction *pullAction = contextMenu.addAction("拉取");
        QAction *openTypeAction = contextMenu.addAction("用...方式打开");
        QAction *copyAction = contextMenu.addAction("复制");
        QAction *cutAction = contextMenu.addAction("剪切");
        QAction *putAction = contextMenu.addAction("粘贴");
        QAction *closeAction = contextMenu.addAction("关闭");
        // 执行菜单
        QAction *selectedAction = contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));

        if (selectedAction == openAction) {
            openFile(item);
        } else if (selectedAction == addAction) {
            addFile(item);
        } else if (selectedAction == deleteAction) {
            deleteFile(item);
        } else if (selectedAction == createDirAction) {
            createDir(item);
        } else if (selectedAction == renameAction) {
            renameFile(item);
        } else if (selectedAction == openDirAction) {
            openDir(item);
        } else if (selectedAction == pushAction) {
            push(item);
        } else if (selectedAction == pullAction) {
            pull(item);
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

void MainPage::SpaceFileGetSuccess(FileInfo info)
{
    LOG << "remote file " << QS(info.ClientPath) << " sync success";
}

void MainPage::ProcessDeleteFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream(mdata);
    bool ret = response.mCode;
    FileInfo info;
    if (!ret) {
        return;
    }
    int id = 0;
    stream >> id;
    if (!mSpaceFileMap.count(id)) {
        return;
    }
    info = mSpaceFileMap[id];
    qDebug() << "delete file" << id;
    deleteSpaceFileFromPage(info);
}

void MainPage::ProcessRenameFileSuccess(Response response)
{
    std::string mdata = response.mData;
    MyProtocolStream stream(mdata);
    bool ret = response.mCode;
    if (!ret) {
        return;
    }
    int id = 0;
    stream >> id;
    if (!mSpaceFileMap.count(id)) {
        return;
    }
    FileInfo &info = mSpaceFileMap[id];
    stream >> info.serverPath >> info.serverFileName;
    setClientDir(info);
    moveClientLocalDir(info);
    info.serverFileName =
    qDebug() << "rename file" << id;
    deleteSpaceFileFromPage(info);
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
//                item->setText(1, QString::fromStdString(son.second.serverPath));
//                item->setText(2, QString::number(son.second.filesize));
                item->setToolTip(0, QS(son.second.ClientPath));
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
//                item->setText(1, QString::fromStdString(son.second.serverPath));
//                item->setText(2, QString::number(son.second.filesize));
                item->setToolTip(0, QS(son.second.ClientPath));
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

//id = -1，是在本地创建，还没有同步的
TreeNode *MainPage::addSpaceFileToPage(FileInfo info)
{
    int parentId = info.parentId;
    TreeNode* node = nullptr;
    if (info.id > 0 && idBook.count(info.id) && idBook[info.id] != nullptr)
        return idBook[info.id];
    if (idBook.count(parentId)) {
        TreeNode* pNode = idBook[parentId];
        if (pNode == nullptr)
            return nullptr;
        node = new TreeNode(info.id);
        node->parent = idBook[parentId];
        idBook[parentId]->son.push_back(node);
        if (info.id > 0)
            idBook[info.id] = node;
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, QString::fromStdString(info.serverFileName));
        qDebug() << "tttttttttttttttttt "<< info.id << QS(info.serverPath);
//        item->setText(1, QString::fromStdString(info.serverPath));
//        item->setText(2, QString::number(info.filesize));
        item->setToolTip(0, QS(info.ClientPath));
        node->item = item;
        if (node->parent->item) {
            node->parent->item->addChild(item);
            //创建本地目录
            if (info.fileType == "dir") {
                QString idx = QString("userInfo/") + QN(::user_id) + "/";
                if (QS(info.serverPath).startsWith(idx)) {
                    QString clientPath = mSpaceRootDir + QS(info.serverPath).mid(idx.size()) + QS(info.serverFileName);
                    qDebug() << clientPath;
                    info.ClientPath = clientPath.toStdString();
                    createDirectoryIfNotExist(clientPath);
                }
            }
            if (info.id > 0) {
                mSpaceFileMap[info.id] = info;
                item->setWhatsThis(0, QString::number(info.id));
            }

        }
        else {
            qDebug() << "Error parent has no widget";
        }

    }
    return node;
}

bool MainPage::deleteSpaceFileFromPage(FileInfo info) {
    TreeNode* node = nullptr;
    if (info.id <= 0 || !idBook.count(info.id) || idBook[info.id] == nullptr)
        return false;
    node = idBook[info.id];
    for (auto &x : node->son) {
        deleteSpaceFileFromPage(mSpaceFileMap[x->fileId]);
    }
    node->son.clear();
    mSpaceFileMap.erase(info.id);
    //删除本地文件
    deleteLocalSpaceFile(info);

    QTreeWidgetItem* item = node->item;
    if (node->parent != nullptr && node->parent->item) {
        node->parent->item->removeChild(item);
        delete item;
    }
    else {
        qDebug() << "Error erase item";
    }
    idBook.erase(info.id);
    delete node;
    return true;
}

bool MainPage::deleteLocalSpaceFile(FileInfo info) {
    if (info.fileType == "rootdir" || info.fileType == "dir") {
        QDir dir(QS(info.ClientPath));
        if (dir.exists()) {
            bool result = dir.rmpath(QS(info.ClientPath));
            if (result) {
                qDebug() << "删除目录成功: " << QS(info.ClientPath);
            } else {
                qDebug() << "删除目录失败: " << QS(info.ClientPath);
            }
        } else {
            qDebug() << "目录不存在: " << QS(info.ClientPath);
        }
    }
    else {
        QFile file(QS(info.ClientPath));
        if (file.remove()) {
            qDebug() << "删除文件成功:" << QS(info.ClientPath);
        }
        else {
            qDebug() << "删除文件失败:" << QS(info.ClientPath);
        }
    }
    return true;
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
        setClientDir(info);
        mSpaceFileMap[info.id] = info;
    }

    mRoot = addAllSpaceFileToPage();
    if (!mRoot) {
        qDebug() << "init Space File Tree Error";
    }
}

//删除文件
//如果文件是刚上传的，还没有双击同步到客户端，那么删除很危险，会把原文件删掉
//后续可以判断，如果是有引用，可以用复制的方式复制。删除的时候进行提示。
bool MainPage::deleteFile(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    Processor::DeleteFile(info);
    QMessageBox::information(this, "Delete Action", QString("Deleting: %1").arg(item->text(0)));
    //在返回响应后更细致的删除处理，和删除本地文件
    return true;
}

//添加文件
bool MainPage::addFile(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    FileInfo info;
    int fileId = item->whatsThis(0).toInt();
    FileInfo parentInfo = mSpaceFileMap[fileId];
    if (parentInfo.fileType != std::string("dir") && parentInfo.fileType != std::string("rootdir")) {
        return false;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"),
        "",
        tr("All Files (*);;Text Files (*.txt)"));
    //D:/easybcd.zip
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected.";
        return false;
    }
    info.Generate();
    info.ClientPath = fileName.toStdString();
    info.serverPath = parentInfo.serverPath + parentInfo.serverFileName + "/";
    QFileInfo fileInfo(fileName);
    // 获取文件名（包括扩展名）
    info.serverFileName = fileInfo.fileName().toStdString();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.expiredTime = 3600 * 24 * 15;
    info.parentId = parentInfo.id;//TODO:
    info.serviceType = FileServerType::STOREFILE;
    info.fileType = "text";
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return true;

//添加离线文件
//QMessageBox::information(this, "Add Action", QString("Deleting: %1").arg(item->text(0)));
//    QString newName = QInputDialog::getText(this, "提示", "请输入新文件名");
//    int fileId = item->whatsThis(0).toInt();
//    FileInfo parentInfo = mSpaceFileMap[fileId];
//    FileInfo newInfo;
//    newInfo.ClientPath = parentInfo.ClientPath + "/" + newName.toStdString();
//    newInfo.parentId = parentInfo.id;
//    newInfo.serverPath = "new";
//    newInfo.fileType = "text";
//    newInfo.serverFileName = newName.toStdString();
//    addSpaceFileToPage(newInfo);
//    addLoaclFile(QS(newInfo.ClientPath));
}

bool MainPage::createDir(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    int fileId = item->whatsThis(0).toInt();
    FileInfo pInfo = mSpaceFileMap[fileId];
    QString dirName = QInputDialog::getText(this, "提示", "请输入目录名");
    if (dirName == "" || dirName.isEmpty()) {
        return false;
    }
    FileInfo info;
    info.Generate();
    info.ClientPath = pInfo.ClientPath + "/" + dirName.toStdString();
    info.serverPath = pInfo.serverPath + pInfo.serverFileName + "/";

    // 获取文件名（包括扩展名）
    info.serverFileName = dirName.toStdString();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.expiredTime = 3600 * 24 * 15;
    info.parentId = pInfo.id;//TODO:
    info.id = 0;
    info.serviceType = FileServerType::STOREFILE;
    info.fileType = "dir";
    //为了之后获取结构体，并不是需要发送文件
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    QMessageBox::information(this, "createDir Action", QString("createDir: %1").arg(dirName));
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
    if (spaceHasFile(&info)) {
        if(!showInEdit(&info)) {
            LOG << "Error Open " << QS(info.ClientPath);
        }
    }
    else {
        LOG << "Open File Failure, No File" << QS(info.ClientPath);
    }
    return true;
}

bool MainPage::openDir(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    if (info.fileType == std::string("dir") && info.fileType == std::string("rootdir")) {
        QString directoryPath = QS(info.ClientPath);
        QUrl url = QUrl::fromLocalFile(directoryPath);
        // 打开文件所在目录
        return QDesktopServices::openUrl(url);
    }
    else {
        QString filePath = QS(info.ClientPath);
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && fileInfo.isFile()) {
            QString directoryPath = fileInfo.absolutePath();
            QUrl url = QUrl::fromLocalFile(directoryPath);
            // 打开文件所在目录
            return QDesktopServices::openUrl(url);
        } else {
            LOG << "File does not exist or is not a valid file.";
            return false;
        }
    }
    return true;
}

//推送到远程 单推 全推 推目录 推选中（选做）
bool MainPage::push(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
    if (info.fileType == std::string("dir") && info.fileType == std::string("rootdir")) {
        return false;
    }
    info.Generate();
    info.send_id = user_id;
    info.recv_id = 0;//发送给服务器存储
    info.serviceType = FileServerType::STOREFILE;
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::StoreFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return true;
}

//从远程拉取，并显示在edit中， 单拉 全拉 拉目录 拉选中（选做）
bool MainPage::pull(QTreeWidgetItem* item) {
    int fileId = item->whatsThis(0).toInt();
    FileInfo info = mSpaceFileMap[fileId];
//    if (info.fileType == std::string("dir") && info.fileType == std::string("rootdir")) {
//        return false;
//    }

    info.Generate();
    info.serviceType = FileServerType::GETSTOREFILE;
    FtpSender::GetInstance()->addFile(info);
    bool ret = Processor::GetFile(info);
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
    return true;
}


//某种外部打开方式打开
bool MainPage::openType(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    return true;
}

bool MainPage::copy(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    mCurrentCopyItem = item;
    return true;
}

bool MainPage::cut(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    mCurrentCutItem = item;
    return true;
}

bool MainPage::put(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    if (mCurrentCopyItem != nullptr) {

        //
        mCurrentCopyItem = nullptr;
        return true;
    }
    else if (mCurrentCutItem != nullptr) {

        mCurrentCutItem = nullptr;
        return true;
    }
    else if (mCurrentCopyItemList.size() != 0) {

        mCurrentCopyItemList.clear();
        return true;
    }
    else if (mCurrentCutItemList.size() != 0) {
        mCurrentCutItemList.clear();
        return true;
    }
    return true;
}

bool MainPage::close(QTreeWidgetItem* item) {
    Q_UNUSED(item)
    //询问是否保存修改

    ui->plainTextEdit->clear();
    if (mCurrentFile != nullptr && mCurrentFile->isOpen()) {
        mCurrentFile->close();
        mCurrentFile = nullptr;
    }
    return true;
}


bool MainPage::spaceHasFile(FileInfo *info)
{
    if (info->fileType == std::string("dir") && info->fileType == std::string("rootdir")) {
        return false;
    }
    QFile file(QS(info->ClientPath));
    return file.exists();
}

bool MainPage::spaceHasFile(QString ClientPath)
{
    QFile file(ClientPath);
    return file.exists();
}

bool MainPage::setClientDir(FileInfo &info) {
    if (info.fileType == "rootdir") {
        info.ClientPath = mSpaceRootDir.toStdString();
    }
    else if (info.fileType == "dir") {
        QString idx = QString("userInfo/") + QN(::user_id) + "/";
        if (QS(info.serverPath).startsWith(idx)) {
            QString clientPath = mSpaceRootDir + QS(info.serverPath).mid(idx.size()) + QS(info.serverFileName);
            qDebug() << clientPath;
            info.ClientPath = clientPath.toStdString();
            createDirectoryIfNotExist(clientPath);
        }
    }
    else {
        QString idx = QString("userInfo/") + QN(::user_id) + "/";
        qDebug() << "idx" << idx << QS(info.serverPath);
        if (QS(info.serverPath).startsWith(idx)) {
            QString clientPath = mSpaceRootDir + QS(info.serverPath).mid(idx.size()) + QS(info.serverFileName);
            qDebug() << clientPath;
            if (spaceHasFile(clientPath)) {
                info.ClientPath = clientPath.toStdString();
            }
            else {
                info.ClientPath = "not in local";
            }
        }
    }
}

bool MainPage::moveClientLocalDir(FileInfo &info) {
    //将本地文件重命名，或者移动
}


bool MainPage::showInEdit(FileInfo *info)
{
    QFile file(QS(info->ClientPath));
    // 打开文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading!" << QS(info->ClientPath);
        return false;
    }
    // 获取文件大小
    qint64 fileSize = file.size();
    QString fileContent;
    // 判断文件是否大于10MB
    if (fileSize > 10 * 1024 * 1024) {  // 10MB = 10 * 1024 * 1024 字节
        // 文件大于10MB，逐行读取
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();  // 逐行读取
            qDebug() << line;
            fileContent += line;
        }
    } else {
        // 文件小于等于10MB，一次性读取
        QTextStream in(&file);
        fileContent = in.readAll();  // 读取全部内容
        qDebug() << fileContent;
    }
    file.close();  // 关闭文件
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(fileContent);
    return true;
}

bool MainPage::createDirectoryIfNotExist(const QString &dirPath) {
    QDir dir;
    // 如果目录已经存在，返回
    if (dir.exists(dirPath)) {
        qDebug() << "目录已经存在:" << dirPath;
        return true;
    }
    // 否则，创建目录
    if (dir.mkpath(dirPath)) {
        qDebug() << "目录创建成功:" << dirPath;
        return true;
    } else {
        qDebug() << "创建目录失败:" << dirPath;
    }
    return false;
}

bool MainPage::addLoaclFile(const QString &filePath) {
    QFile file(filePath);
    if (file.exists()) {
        return false;
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 关闭文件
        file.close();
        qDebug() << "文件创建成功:" << filePath;
        return true;
    } else {
        qDebug() << "无法创建文件:" << filePath;
    }
    return false;
}

void MainPage::on_diffBtn_clicked()
{
    //显示时间差异 md5差异，每个文件的差异可以点击查看

    //强制全部更新，强制全部推送

    //省流型部分推送，差异推送，部分拉取，差异拉取。

}

