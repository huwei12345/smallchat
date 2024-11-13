#include "chatedittext.h"
#include <QTextBlock>
#include <QDebug>
#include <QFileInfo>
#include <QDesktopServices>
#include <QPropertyAnimation>
#include <cmath>

ChatEditText::ChatEditText(QWidget *parent) :
    QTextEdit(parent)
{
    //实现点击文件功能
    QObject::connect(this, &QTextEdit::cursorPositionChanged, this, &ChatEditText::handleCursorPositionChange);
    mEmojiSelector = new EmojiSelector;
    setReadOnly(true);
    // 设置样式表，去掉边框
    setStyleSheet("QTextEdit { border: none; }");
}

ChatEditText::ChatEditText(MessageInfo *info, QWidget *parent) :
    ChatEditText(parent)
{

}

bool ChatEditText::build(MessageInfo *info)
{
    if (QString::fromStdString(info->message_text) == "0xa500") {
        insertPlainText("抖动窗口！");
        sharkWindow();
        return true;
    }
    mEmojiSelector->showContentWithEmoji(this, QString::fromStdString(info->message_text));
    return true;
}

//抖动窗口
void ChatEditText::sharkWindow() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(200);
    animation->setLoopCount(3);
    animation->setKeyValueAt(0, this->geometry());
    int x = this->x();
    int y = this->y();
    for (int i = 1; i <= 4; ++i) {
        animation->setKeyValueAt(0.25 * i, QRect(x + 2 * pow(-1, i), y + 10 * i, this->width(), this->height()));
        //animation->setKeyValueAt(0.5 * i + 0.25, QRect(button->x() + 5 * i, button->y(), button->width(), button->height()));
    }
    animation->start();
}


//TODO:实现超链接功能
void ChatEditText::handleCursorPositionChange() {
    // 处理文本框中的点击事件
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        return; // 如果有选中内容，不处理
    }
    QTextCharFormat format = cursor.charFormat();
    if (format.foreground().color() == Qt::blue && format.fontUnderline()) {
        QString str = cursor.block().text();
        QString completeTip = "Complete Trans Coming in ";
        int index = str.indexOf(completeTip);
        QString filePath;
        if (index != -1) {
            filePath = str.mid(index + completeTip.size());
        }
        qDebug() << filePath;
        if (!filePath.isEmpty()) {
            // 在这里可以处理打开文件的逻辑
            // 例如，打开文件或执行其他操作
            // QFile file(filePath);
            // 进行读取或其他操作
            bool ret = openFileDirectory(filePath);
            if (ret) {
                qDebug() << "Open " << filePath << " Success";
            }
        }
    }
}

bool ChatEditText::openFileDirectory(const QString &filePath) {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile()) {
        QString directoryPath = fileInfo.absolutePath();
        QUrl url = QUrl::fromLocalFile(directoryPath);
        // 打开文件所在目录
        QDesktopServices::openUrl(url);
        return true;
    } else {
        qWarning() << "File does not exist or is not a valid file.";
        return false;
    }
    return false;
}
