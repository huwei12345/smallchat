#ifndef CHATEDITIMAGE_H
#define CHATEDITIMAGE_H

#include <QWidget>
#include <QToolButton>
#include "Protocol.h"
class ChatEditImage : public QToolButton
{
    Q_OBJECT

public:
    explicit ChatEditImage(QWidget *parent = nullptr);
    ~ChatEditImage();

    void showContextMenu(const QPoint &pos);
    bool build(MessageInfo* info);
protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setIconSizeBasedOnButtonSize();

private slots:
    void on_ImageBtn_clicked();

private:
    QString fileName;
    QString fileType;
};

#endif // CHATEDITIMAGE_H
