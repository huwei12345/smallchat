#ifndef CHATEDITTEXT_H
#define CHATEDITTEXT_H

#include <QTextEdit>
#include <QObject>
#include <QWidget>
#include "Protocol.h"
#include "emojiselector.h"
class ChatEditText : public QTextEdit
{
    Q_OBJECT
public:
    ChatEditText(QWidget *parent = nullptr);
    ChatEditText(MessageInfo* info, QWidget *parent = nullptr);
    bool build(MessageInfo* info);
    bool openFileDirectory(const QString &filePath);
    void sharkWindow();
private slots:
    void handleCursorPositionChange();

private:
    EmojiSelector *mEmojiSelector;
};

#endif // CHATEDITTEXT_H
