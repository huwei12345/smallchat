#include "mainpage.h"
#include "ui_mainpage.h"
#include <QDebug>
#include <QFileDialog>
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
}

MainPage::MainPage(UserInfo info, QWidget *parent) :
    MainPage(parent)
{
    mInfo = info;
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
    char text[200];
    sprintf(text, "%15s %15s parentId: %d id: %d\n", info.serverFileName.c_str(), info.serverPath.c_str(), info.parentId, info.id);
    QString *str = new QString(text);
    item->setText(0, *str);
    ui->treeWidget->addTopLevelItem(item);
}
