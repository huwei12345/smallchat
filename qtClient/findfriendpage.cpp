#include "findfriendpage.h"
#include "processor.h"
#include "ui_findfriendpage.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "network.h"
#include "Protocol.h"
FindFriendPage::FindFriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindFriendPage)
{
    ui->setupUi(this);
    returnWindow = NULL;
    QIcon windowIcon(QPixmap(":/main/title.jpeg")); // 假设你的图标文件位于资源文件中或者项目目录下
    setWindowIcon(windowIcon);
    setWindowTitle(tr("Qfei"));

    connect(ClientNetWork::GetInstance(), &ClientNetWork::FindFriendSuccess, this, &FindFriendPage::findFriendSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::AddFriendSuccess, this, &FindFriendPage::addFriendSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::applyJoinGroupSuccess, this, &FindFriendPage::applyJoinGroupSuccess);
    connect(ClientNetWork::GetInstance(), &ClientNetWork::FindGroupSuccess, this, &FindFriendPage::FindGroupSuccess);
}

FindFriendPage::~FindFriendPage()
{
    delete ui;
}

void FindFriendPage::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) // 当按下Return/Enter键时
    {
        hide();
        returnWindow->show();
    }
    else
    {
        // 继续处理其他按键事件
        QWidget::keyPressEvent(event);
    }
}


int stoiAll(const std::string &str) {
    int number = 0;
    if (str.size() > 15) {
        return -1;
    }
    for (int i = 0; i < (int)str.size(); i++) {
        if (str[i] > '9' || str[i] < '0') {
            return -1;
        }
        number = number * 10 + (str[i] - '0');
    }
    return number;
}

//查找
void FindFriendPage::on_pushButton_clicked()
{
    if (ui->friendButton->isChecked()) {
        FindFriend();
    }
    else {
        FindGroup();
    }
}

void FindFriendPage::findFriendSuccess(Response response) {
    if (response.mCode) {
        std::string mdata = response.mData;
        MyProtocolStream stream2(mdata);
        int size = 0;
        stream2 >> size;
        if (size > 0) {
            for (int i = 0; i < size; i++) {
                UserInfo info;
                stream2 >> info.user_id >> info.username >> info.email;
                char text[100] = {0};
                sprintf(text, "%10d    %15s     %15s ", info.user_id, info.username.c_str(), info.email.c_str());
                info.print();
//                QLabel *label = new QLabel(this);
//                label->setText(text);
                QListWidgetItem *item = new QListWidgetItem();
                item->setText(text);
                ui->listWidget->addItem(item);
            }
        }
    }
    else {

    }
}

void FindFriendPage::FindGroupSuccess(Response response)
{
    //未实现
    if (response.mCode) {
        std::string mdata = response.mData;
        MyProtocolStream stream2(mdata);
        int size = 0;
        stream2 >> size;
        if (size > 0) {
            for (int i = 0; i < size; i++) {
                GroupInfo info;
                stream2 >> info.id >> info.group_name >> info.role >> info.admin_id >> info.gtype >> info.description >> info.tips >> info.timestamp;
                char text[100] = {0};
                sprintf(text, "%10d    %15s     %15s ", info.id, info.group_name.c_str(), info.gtype.c_str());
                info.print();
//              QLabel *label = new QLabel(this);
//              label->setText(text);
                QListWidgetItem *item = new QListWidgetItem();
                item->setText(text);
                ui->listWidget->addItem(item);
            }
        }
    }
    else {

    }
}


//添加
void FindFriendPage::on_pushButton_2_clicked()
{
    if (mSelectItem == NULL) {
        return;
    }
    QListWidgetItem* item = mSelectItem;
    int id = atoi(item->text().toStdString().c_str());
    bool ret = false;
    if (ui->friendButton->isChecked()) {
        //添加好友
        ret = Processor::AddFriend(id);
    }
    else {
        //添加群组
        ret = Processor::JoinGroup(id);
    }
    if (!ret) {
        QMessageBox::information(this,"提示","网络不可达！");
    }
}

void FindFriendPage::addFriendSuccess(int mCode) {
    if (mCode == true) {
        QMessageBox::information(this,"提示","已发送添加好友申请！");
    }
    else {

    }
}

void FindFriendPage::on_listWidget_itemSelectionChanged()
{
    int selectedCount = ui->listWidget->selectedItems().count();

    // Enable button if at least one item is selected, disable otherwise
    ui->pushButton_2->setEnabled(selectedCount > 0);
}

void FindFriendPage::applyJoinGroupSuccess(Response rsp)
{
    if (rsp.mCode) {
        QMessageBox::information(this, "群组通知", "申请加群成功，等待管理员操作");
    }
    else {
        QMessageBox::information(this, "群组通知", "申请加群失败！");
    }
}

void FindFriendPage::on_listWidget_itemClicked(QListWidgetItem *item)
{
    mSelectItem = item;
}

void FindFriendPage::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    mSelectItem = item;
}

void FindFriendPage::FindFriend()
{
    std::string str = ui->lineEdit->text().toStdString();
    if (str.size() <= 0) {
        return;
    }
    int friendId = stoiAll(str);
    if (str[0] >= '0' && str[0] <= '9' && friendId > 0) {
        printf("friendId = %d\n", friendId);
        bool ret = Processor::FindFriend(friendId);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
        }
    }
    else if (isalpha(str[0])) {
        printf("friendName = %s\n", str.c_str());
        bool ret = Processor::FindFriendByName(str);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
        }
    }
    else {
        printf("friendId = %d\n", friendId);
        QMessageBox::information(this,"提示","输入错误！");
    }
}

void FindFriendPage::FindGroup()
{
    std::string str = ui->lineEdit->text().toStdString();
    if (str.size() <= 0) {
        return;
    }
    int groupId = stoiAll(str);
    if (str[0] >= '0' && str[0] <= '9' && groupId > 0) {
        printf("groupId = %d\n", groupId);
        bool ret = Processor::FindGroup(groupId);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
        }
    }
    else if (isalpha(str[0])) {
        printf("groupName = %s\n", str.c_str());
        bool ret = Processor::FindGroupByName(str);
        if (!ret) {
            QMessageBox::information(this,"提示","网络不可达！");
        }
    }
    else {
        printf("friendId = %d\n", groupId);
        QMessageBox::information(this,"提示","输入错误！");
    }
}

void FindFriendPage::on_friendButton_clicked()
{
    LOG << "Find Friend";
    ui->tipLabel->setText("用户");
}


void FindFriendPage::on_groupButton_clicked()
{
    LOG << "Find Group";
    ui->tipLabel->setText("群组");
}

