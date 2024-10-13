#ifndef EMOJICCODER_H
#define EMOJICCODER_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class EmojiCoder : QWidget
{
    Q_OBJECT
public:
    EmojiCoder(QWidget* parent = nullptr);
    int explain(QTextEdit* , QString context);
    int encode();
};

#endif // EMOJICCODER_H
