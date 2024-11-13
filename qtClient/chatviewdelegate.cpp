#include "chatviewdelegate.h"

#include <QPainter>
#include <QDebug>

//按钮的高、宽以及坐标点
#define WIDGET_LEFT_MARGIN 600
#define WIDGET_TOP_MARGIN 20
#define WIDGET_WIDTH 150
#define WIDGET_HEIGHT 100
//每个item的高度
#define LISTVIEW_ITEM_HEIGHT 150
//分割item的线的颜色
#define LINE_COLOR "#CECECE"
//文字的颜色
#define TEXT_COLOR "#130c0e"
//文字的大小
#define TEXT_SIZE 10

ChatViewDelegate::ChatViewDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

bool ChatViewDelegate::drawText(QPainter* painter, const QStyleOptionViewItem &option, ChatViewItemProperty& itemData) const {
    painter->save();
    //每个item的区域
    QRectF rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth(option.rect.width() - 1);
    rect.setHeight(option.rect.height() - 1);
    qDebug() << "drawText " << option.rect.height();
    //text的区域
    QRect textRect = QRect(
    0 + 10, rect.top() + WIDGET_TOP_MARGIN / 10, WIDGET_LEFT_MARGIN, WIDGET_HEIGHT / 2);
    painter->setPen(QPen(TEXT_COLOR));
    painter->setFont(QFont("SourceHanSansCN-Normal", TEXT_SIZE));
    //头像 UserName                2024-08-20 12:00:00 234
    painter->drawText(textRect, itemData.messageTip);

    //widget的区域  图片 or 文件
    QRect widgetRect = QRect(
    0, rect.top() + WIDGET_TOP_MARGIN + 10, WIDGET_LEFT_MARGIN, WIDGET_HEIGHT / 2);
    if (itemData.widget) {
        itemData.widget->setGeometry(widgetRect);
        itemData.widget->show();
    }

//    //设置线的颜色
//    painter->setPen(QPen(QColor(LINE_COLOR)));
//    //两点确定一条直线
//    //线的起点：x一定是0，并且每一条线的x是一样的，y是每个item的顶部坐标 + 1
//    //线的终点：y一定是和起点的y是一样的，那么只需要给出x的长度，线的长度就是每个item的宽度
//    painter->drawLine(QPointF(0, rect.bottom() - 1), QPointF(rect.width(), rect.bottom() - 1));

    painter->restore();
    return true;
}

bool ChatViewDelegate::drawImage(QPainter* painter, const QStyleOptionViewItem &option, ChatViewItemProperty& itemData) const {
    painter->save();
    //每个item的区域
    QRectF rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth(option.rect.width() - 1);
    rect.setHeight(option.rect.height() - 1);
    //text的区域
    QRect textRect = QRect(
    0 + 10, rect.top() + WIDGET_TOP_MARGIN / 10, WIDGET_LEFT_MARGIN, WIDGET_HEIGHT / 2);
    painter->setPen(QPen(TEXT_COLOR));
    painter->setFont(QFont("SourceHanSansCN-Normal", TEXT_SIZE));

    //头像 UserName                2024-08-20 12:00:00 234
    painter->drawText(textRect, itemData.messageTip);

    //widget的区域  图片 or 文件
    QRect widgetRect = QRect(
    0, rect.top() + WIDGET_TOP_MARGIN + 10, WIDGET_WIDTH, WIDGET_HEIGHT);
    if (itemData.widget) {
    itemData.widget->setGeometry(widgetRect);
    itemData.widget->show();
    }

    //设置线的颜色
    painter->setPen(QPen(QColor(LINE_COLOR)));
    //两点确定一条直线
    //线的起点：x一定是0，并且每一条线的x是一样的，y是每个item的顶部坐标 + 1
    //线的终点：y一定是和起点的y是一样的，那么只需要给出x的长度，线的长度就是每个item的宽度
    painter->drawLine(QPointF(0, rect.bottom() - 1), QPointF(rect.width(), rect.bottom() - 1));

    painter->restore();
    return true;
}

bool ChatViewDelegate::drawFile(QPainter* painter, const QStyleOptionViewItem &option, ChatViewItemProperty& itemData) const {
    painter->save();
    //每个item的区域
    QRectF rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth(option.rect.width() - 1);
    rect.setHeight(option.rect.height() - 1);
    //text的区域
    QRect textRect = QRect(
    0 + 10, rect.top() + WIDGET_TOP_MARGIN / 10, WIDGET_LEFT_MARGIN, WIDGET_HEIGHT / 2);
    painter->setPen(QPen(TEXT_COLOR));
    painter->setFont(QFont("SourceHanSansCN-Normal", TEXT_SIZE));

    //头像 UserName                2024-08-20 12:00:00 234
    painter->drawText(textRect, itemData.messageTip);

    //widget的区域  图片 or 文件
    QRect widgetRect = QRect(
    0, rect.top() + WIDGET_TOP_MARGIN + 10, WIDGET_WIDTH, WIDGET_HEIGHT);
    if (itemData.widget) {
    itemData.widget->setGeometry(widgetRect);
    itemData.widget->show();
    }

    //设置线的颜色
    painter->setPen(QPen(QColor(LINE_COLOR)));
    //两点确定一条直线
    //线的起点：x一定是0，并且每一条线的x是一样的，y是每个item的顶部坐标 + 1
    //线的终点：y一定是和起点的y是一样的，那么只需要给出x的长度，线的长度就是每个item的宽度
    painter->drawLine(QPointF(0, rect.bottom() - 1), QPointF(rect.width(), rect.bottom() - 1));

    painter->restore();
    return true;
}

void ChatViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return;
    }
    QVariant dataVar = index.data(Qt::UserRole + 1);
    ChatViewItemProperty itemData = dataVar.value<ChatViewItemProperty>();
    if (itemData.type == ChatViewItemProperty::Text) {
        drawText(painter, option, itemData);
    }
    else if (itemData.type == ChatViewItemProperty::Image) {
        drawImage(painter, option, itemData);
    }
    else if (itemData.type == ChatViewItemProperty::File) {
        drawFile(painter, option, itemData);
    }
    else {
        qDebug() << "paint draw what type " << (int)itemData.type;
    }

}

bool ChatViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget* ChatViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QStyledItemDelegate::createEditor(parent, option, index);
}


// 重写 sizeHint 方法设置每一项的大小
//QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    if (index.data(Qt::UserRole).toString() == "button") {
//        // 设置按钮项的固定大小
//        return QSize(80, 80);  // 设置按钮的宽度和高度
//    } else {
//        // 设置其他项的大小
//        return QSize(option.rect.width(), 40);  // 设置每一项的高度为 50，宽度自动适应
//    }
//}

QSize ChatViewDelegate::sizeHint(
    const QStyleOptionViewItem &option,
    const QModelIndex &index)
    const {
    QVariant dataVar = index.data(Qt::UserRole + 1);
    ChatViewItemProperty itemData = dataVar.value<ChatViewItemProperty>();
    if (itemData.type == ChatViewItemProperty::Text) {
        return QSize(option.rect.width(), LISTVIEW_ITEM_HEIGHT / 2);
    }
    else if (itemData.type == ChatViewItemProperty::Image) {
        return QSize(option.rect.width(), LISTVIEW_ITEM_HEIGHT);
    }
    else if (itemData.type == ChatViewItemProperty::File) {
        return QSize(option.rect.width(), LISTVIEW_ITEM_HEIGHT);
    }
    else {
        qDebug() << "sizeHint what type " << (int)itemData.type;
    }
    return QSize(option.rect.width(), LISTVIEW_ITEM_HEIGHT);
}
