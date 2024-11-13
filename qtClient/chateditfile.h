#ifndef CHATEDITFILE_H
#define CHATEDITFILE_H

#include <QToolButton>
#include <QWidget>
#include "Protocol.h"
class ChatEditFile : public QToolButton
{
    Q_OBJECT

public:
    explicit ChatEditFile(QWidget *parent = nullptr);
    ~ChatEditFile();

    bool openFileDirectory(const QString &filePath);
    void setIconSizeBasedOnButtonSize();
    bool build(MessageInfo* info);

protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_FileBtn_clicked();

    void showContextMenu(const QPoint &pos);
private:
    QString fileName;
    QString fileType;
};

#endif // CHATEDITFILE_H
