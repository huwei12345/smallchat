#include "mainpage.h"
#include "ui_mainpage.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMessageBox>
#include "processor.h"
#include "ftpsender.h"
#include "network.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));
    returnWindow = NULL;
    ui->pushButton->setShortcut(tr("Esc"));
    connect(ClientNetWork::GetInstance(), &ClientNetWork::storeFileResponse, this, &MainPage::StoreFileAllow);
    //ui->treeWidget->setHeaderLabel("Tree Example");
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 设置上下文菜单策略

    connect(ClientNetWork::GetInstance(), ClientNetWork::findSpaceFileTreeSuccess, this, MainPage::findSpaceFileTreeSuccess);
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
    //根据info，显示在界面上
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, QString::fromStdString(info.serverFileName));
    item->setText(1, QString::fromStdString(info.serverPath));
    item->setText(2, QString::number(info.filesize));
    ui->treeWidget->addTopLevelItem(item);
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
    //QMessageBox::information(this, "提示", "Single");
}


void MainPage::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
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
        QAction *editAction = contextMenu.addAction("Edit");
        QAction *deleteAction = contextMenu.addAction("Delete");
        // 执行菜单
        QAction *selectedAction = contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
        if (selectedAction == editAction) {
            QMessageBox::information(this, "Edit Action", QString("Editing: %1").arg(item->text(0)));
        } else if (selectedAction == deleteAction) {
            QMessageBox::information(this, "Delete Action", QString("Deleting: %1").arg(item->text(0)));
        }
    }
}

void MainPage::addSpaceFileToPage(int i, FileInfo info) {

}

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
        addSpaceFileToPage(i, info);//
        info.print();
        mSpaceFileMap[info.id] = info;
    }
}
