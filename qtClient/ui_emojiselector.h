/********************************************************************************
** Form generated from reading UI file 'emojiselector.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMOJISELECTOR_H
#define UI_EMOJISELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_EmojiSelector
{
public:

    void setupUi(QDialog *EmojiSelector)
    {
        if (EmojiSelector->objectName().isEmpty())
            EmojiSelector->setObjectName(QString::fromUtf8("EmojiSelector"));
        EmojiSelector->resize(400, 300);

        retranslateUi(EmojiSelector);

        QMetaObject::connectSlotsByName(EmojiSelector);
    } // setupUi

    void retranslateUi(QDialog *EmojiSelector)
    {
        EmojiSelector->setWindowTitle(QCoreApplication::translate("EmojiSelector", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmojiSelector: public Ui_EmojiSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMOJISELECTOR_H
