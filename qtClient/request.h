#ifndef REQUEST_H
#define REQUEST_H


#include <QObject>
#include <QRunnable>

class Task : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit Task(QObject *parent = nullptr);
    void setData(/* data needed for task */);

signals:
    void finished(); // signal emitted when task is finished

protected:
    void run() override; // override the run method to execute the task
};



#endif // REQUEST_H
