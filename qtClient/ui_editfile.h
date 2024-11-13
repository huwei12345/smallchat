/********************************************************************************
** Form generated from reading UI file 'editfile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITFILE_H
#define UI_EDITFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditFile
{
public:
    QHBoxLayout *horizontalLayout;
    QToolButton *FileBtn;

    void setupUi(QWidget *EditFile)
    {
        if (EditFile->objectName().isEmpty())
            EditFile->setObjectName(QString::fromUtf8("EditFile"));
        EditFile->resize(333, 286);
        horizontalLayout = new QHBoxLayout(EditFile);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        FileBtn = new QToolButton(EditFile);
        FileBtn->setObjectName(QString::fromUtf8("FileBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileBtn->sizePolicy().hasHeightForWidth());
        FileBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(FileBtn);


        retranslateUi(EditFile);

        QMetaObject::connectSlotsByName(EditFile);
    } // setupUi

    void retranslateUi(QWidget *EditFile)
    {
        EditFile->setWindowTitle(QCoreApplication::translate("EditFile", "Form", nullptr));
        FileBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class EditFile: public Ui_EditFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITFILE_H
