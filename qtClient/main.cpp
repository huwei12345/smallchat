#include "mainwindow.h"
#include "framelessWidget.h"
#include "netthread.h"
#include "processqueue.h"
#include <QApplication>
#include <QThreadPool>
#include "network.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include "globalvaria.h"
#include "ftpsender.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlobalVaria::GetInstance();
    FtpSender* ftpSender = FtpSender::GetInstance();
    ClientNetWork *client = ClientNetWork::GetInstance();
    client->Client();
    qDebug() << "Show Window";
    MainWindow w1;
    w1.show();


    ftpSender->start();
    /*framelessWidget w;
    w.setWindowFlag(Qt::FramelessWindowHint);//设置无边框属性
    w.setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    w.show()*/;

    //NetThread *thread = new NetThread();
    //thread->start();

//    TaskQueue taskQueue;
//    Task task1, task2, task3; // Instantiate tasks

//    taskQueue.addTask(&task1);
//    taskQueue.addTask(&task2);
//    taskQueue.addTask(&task3);

//    // 创建任务队列对象和任务对象，并将任务添加到队列中。通过连接主程序的退出信号到 lambda 函数，确保在程序退出前等待所有任务完成。
//    QObject::connect(&a, &QCoreApplication::aboutToQuit, &taskQueue, [&taskQueue](){
//        taskQueue.threadPool.waitForDone(); // Wait for all tasks to finish
//    });

    // 连接信号槽
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        if (!client->isClose()) {
            printf("Connected Close!\n");
            fflush(stdout);
            client->close();
        }
    });


    // 创建系统托盘图标
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/main/icon.jpeg"), &a);
    trayIcon->setToolTip("QFei");
    // 创建菜单项
    QAction *showAction = new QAction("Show", &a);
    QAction *quitAction = new QAction("Quit", &a);

    // 创建菜单并添加菜单项
    QMenu *trayMenu = new QMenu(&w1);
    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    // 将菜单设置给系统托盘图标
    trayIcon->setContextMenu(trayMenu);

    // 显示系统托盘图标
    trayIcon->show();

    // 响应菜单项的槽函数
    QObject::connect(showAction, &QAction::triggered, [&](){
        // 执行显示主窗口的操作
        // 例如：mainwindow->show();
        w1.showNormal(); // 显示正常窗口
        w1.activateWindow(); // 激活窗口
    });

    QObject::connect(quitAction, &QAction::triggered, [&](){
        // 执行退出应用程序的操作
        QApplication::quit();
    });

    // 当用户双击系统托盘图标时，显示程序界面
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            w1.showNormal(); // 显示正常窗口
            w1.activateWindow(); // 激活窗口
        }
    });

    // 当窗口要被最小化时，隐藏窗口并显示托盘图标
    QObject::connect(&w1, &QWidget::windowIconChanged, [&](const QIcon &icon){
        if (icon.isNull()) {
            trayIcon->show();
            w1.hide();
        }
    });
    // 当点击托盘图标时，恢复窗口
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            // 鼠标单击托盘图标
            w1.showNormal(); // 或 window.show();
            w1.activateWindow();
        } else if (reason == QSystemTrayIcon::Context) {
            // 鼠标右键点击托盘图标
            qDebug() << "Tray icon context menu activated";
        } else if (reason == QSystemTrayIcon::MiddleClick) {
            // 鼠标中键点击托盘图标
            qDebug() << "Tray icon middle clicked";
        } else if (reason == QSystemTrayIcon::Unknown) {
            // 其他原因
            qDebug() << "Unknown reason for tray icon activation";
        }
    });

    return a.exec();
}


/*
2024.9.8
开发日志：

    目前通信协议比较完善、界面跳转比较完善，对于客户端应该也需要一个用户Instance().
    添加一个处理逻辑还是有些复杂。

    1.添加了MysqlPool，不用每次新建连接了
    2.目前在初始化friendPage界面的三个init都不太成熟。初始化朋友还未去除pending类型。获取未读取消息，目前没有
    闪烁提示以及弹出功能。   获取未添加好友请求还没有闪烁和弹出功能。
    队友所有的pending都没有进行update的操作
    3.添加好友还有没用id添加的方法
    4.可能还需要一个好友验证框
    5.发送消息还有一些问题。
    6.目前Server端需要用户层或者连接层了。如何组织还需要设计。
*/
