/********************************************************************************
** Form generated from reading UI file 'groupchatwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPCHATWINDOW_H
#define UI_GROUPCHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupChatWindow
{
public:

    void setupUi(QWidget *GroupChatWindow)
    {
        if (GroupChatWindow->objectName().isEmpty())
            GroupChatWindow->setObjectName(QString::fromUtf8("GroupChatWindow"));
        GroupChatWindow->resize(400, 300);

        retranslateUi(GroupChatWindow);

        QMetaObject::connectSlotsByName(GroupChatWindow);
    } // setupUi

    void retranslateUi(QWidget *GroupChatWindow)
    {
        GroupChatWindow->setWindowTitle(QCoreApplication::translate("GroupChatWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GroupChatWindow: public Ui_GroupChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHATWINDOW_H
