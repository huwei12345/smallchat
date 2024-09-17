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
    for (int i = 0; i <= (int)str.size(); i++) {
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

//添加
void FindFriendPage::on_pushButton_2_clicked()
{
    int index = 0;
    QListWidgetItem* item = ui->listWidget->item(index);
    int friendId = atoi(item->text().toStdString().c_str());
    bool ret = Processor::AddFriend(friendId);
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
