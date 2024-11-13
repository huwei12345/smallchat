#ifndef CHATVIEWDELEGATE_H
#define CHATVIEWDELEGATE_H

#include <QStyledItemDelegate>

class ChatViewItemProperty {
public:
    enum ItemType {Text = 0, Image, File};
    //文字
    QString messageTip;
    QString textData;
    //按钮
    QWidget *widget;
    ItemType type;
} ;
Q_DECLARE_METATYPE(ChatViewItemProperty)

class ChatViewDelegate : public QStyledItemDelegate
{
public:
    explicit ChatViewDelegate(QObject *parent = nullptr);


    bool drawText(QPainter *painter, const QStyleOptionViewItem &option, ChatViewItemProperty &itemData) const;
    bool drawImage(QPainter *painter, const QStyleOptionViewItem &option, ChatViewItemProperty &itemData) const;
    bool drawFile(QPainter *painter, const QStyleOptionViewItem &option, ChatViewItemProperty &itemData) const;
protected:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // CHATVIEWDELEGATE_H
