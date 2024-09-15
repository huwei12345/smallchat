#ifndef PROCESSQUEUE_H
#define PROCESSQUEUE_H


#include <QObject>
#include <QRunnable>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QThreadPool>
#include "request.h"

class TaskQueue : public QObject
{
    Q_OBJECT

public:
    explicit TaskQueue(QObject *parent = nullptr);
    void addTask(Task *task);
    QThreadPool threadPool;

public slots:
    //void startProcessing();

private:
    QQueue<Task *> tasks;
    QMutex mutex;
};

#endif // PROCESSQUEUE_H
