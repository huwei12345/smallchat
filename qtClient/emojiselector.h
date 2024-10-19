#ifndef EMOJISELECTOR_H
#define EMOJISELECTOR_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class EmojiSelector;
}
class QTextEdit;
class Emoji;

class EmojiSelector : public QDialog
{
    Q_OBJECT

public:
    explicit EmojiSelector(QWidget *parent = nullptr);
    ~EmojiSelector();
    int explain(QTextEdit* , QString context);
    void jsonGet();
    QString extractContentWithImages(QTextEdit *textEdit);
    bool showContentWithImages(QTextEdit *textEdit, QString context);
    int cxplain(QTextEdit *plaintest, QTextEdit *plain, QString context);
signals:
    void emojiSelected(const QString &emoji);


private:
    void setEmojis(QStandardItemModel *model);
    Ui::EmojiSelector *ui;
    std::map<QString, std::map<QString, Emoji*>> mEmojiStructureMap;
    std::map<QString, Emoji*> mEmojiPalinMap;
};

#endif // EMOJISELECTOR_H
