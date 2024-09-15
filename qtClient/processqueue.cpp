#include "processqueue.h"


TaskQueue::TaskQueue(QObject *parent) : QObject(parent)
{
    threadPool.setMaxThreadCount(3); // Set maximum threads to 3
}

void TaskQueue::addTask(Task *task)
{
    QMutexLocker locker(&mutex);
    tasks.enqueue(task);
    //当前有空闲线程 ？？？那没有空闲的任务不处理了？
    threadPool.start(task);

    //那如果添加的任务多余线程数，多出来的任务不是就没办法得到处理？
    /*
    如果任务队列中的任务数量超过了设置的线程池最大线程数，多余的任务会等待直到有空闲的线程可以处理它们为止。这种情况下，QThreadPool 会自动管理线程的执行顺序，并确保尽可能高效地利用已有的线程资源。

    具体来说，当任务队列中的任务数量超过了线程池的最大线程数时，以下情况可能发生：

    等待任务执行： 多余的任务会等待，直到有线程空闲下来可以处理它们为止。这意味着并不会丢失任务，只是会延迟执行。

    线程重用： QThreadPool 会尽可能重用已有的线程来执行等待中的任务，而不是每次都创建新线程。这样可以避免频繁地创建和销毁线程，提高性能。

    任务调度： 线程池会根据任务的到来顺序和线程的可用性来调度任务执行顺序。如果任务很多或者某些任务执行时间较长，线程池会动态调整任务的执行顺序和分配，以尽可能平衡处理负载。

    因此，使用 QThreadPool 可以在不需要手动管理线程的情况下，实现高效的多线程任务处理。确保在设计任务处理逻辑时，考虑到可能的任务延迟和资源限制，以便于系统能够在各种情况下都能正常运行和处理任务。
    */
}

/* 使用系统自带的ThreadPool，换种写法
void TaskQueue::startProcessing()
{
    while (!tasks.isEmpty()) {
        Task *task = nullptr;
        {
            QMutexLocker locker(&mutex);
            if (!tasks.isEmpty())
                task = tasks.dequeue();
        }
        if (task) {
            QThread *thread = new QThread;
            task->moveToThread(thread);
            connect(thread, &QThread::started, task, &Task::process);
            connect(task, &Task::finished, thread, &QThread::quit);
            connect(task, &Task::finished, task, &Task::deleteLater);
            connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            thread->start();
        }
    }
}
*/
