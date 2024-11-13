/********************************************************************************
** Form generated from reading UI file 'editimage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITIMAGE_H
#define UI_EDITIMAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editImage
{
public:
    QHBoxLayout *horizontalLayout;
    QToolButton *ImageBtn;

    void setupUi(QWidget *editImage)
    {
        if (editImage->objectName().isEmpty())
            editImage->setObjectName(QString::fromUtf8("editImage"));
        editImage->resize(400, 300);
        horizontalLayout = new QHBoxLayout(editImage);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        ImageBtn = new QToolButton(editImage);
        ImageBtn->setObjectName(QString::fromUtf8("ImageBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImageBtn->sizePolicy().hasHeightForWidth());
        ImageBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(ImageBtn);


        retranslateUi(editImage);

        QMetaObject::connectSlotsByName(editImage);
    } // setupUi

    void retranslateUi(QWidget *editImage)
    {
        editImage->setWindowTitle(QCoreApplication::translate("editImage", "Form", nullptr));
        ImageBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class editImage: public Ui_editImage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITIMAGE_H
