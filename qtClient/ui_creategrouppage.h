/********************************************************************************
** Form generated from reading UI file 'creategrouppage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEGROUPPAGE_H
#define UI_CREATEGROUPPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateGroupPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *widget;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QLabel *label_5;
    QLineEdit *lineEdit_4;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *CreateGroupPage)
    {
        if (CreateGroupPage->objectName().isEmpty())
            CreateGroupPage->setObjectName(QString::fromUtf8("CreateGroupPage"));
        CreateGroupPage->resize(469, 333);
        verticalLayout = new QVBoxLayout(CreateGroupPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CreateGroupPage);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(18);
        font.setBold(false);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        widget = new QWidget(CreateGroupPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        formLayout = new QFormLayout(widget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_2);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);

        lineEdit_3 = new QLineEdit(widget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_3);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_5);

        lineEdit_4 = new QLineEdit(widget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_4);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(CreateGroupPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget_2);


        retranslateUi(CreateGroupPage);

        QMetaObject::connectSlotsByName(CreateGroupPage);
    } // setupUi

    void retranslateUi(QWidget *CreateGroupPage)
    {
        CreateGroupPage->setWindowTitle(QCoreApplication::translate("CreateGroupPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("CreateGroupPage", "\345\210\233\345\273\272\347\276\244", nullptr));
        label_2->setText(QCoreApplication::translate("CreateGroupPage", "\347\276\244\345\220\215\347\247\260", nullptr));
        label_3->setText(QCoreApplication::translate("CreateGroupPage", "\347\276\244\345\210\206\347\261\273", nullptr));
        label_4->setText(QCoreApplication::translate("CreateGroupPage", "\345\212\240\347\276\244\346\230\257\345\220\246\351\234\200\350\246\201\345\257\206\347\240\201", nullptr));
        label_5->setText(QCoreApplication::translate("CreateGroupPage", "\347\276\244\347\256\200\344\273\213", nullptr));
        pushButton->setText(QCoreApplication::translate("CreateGroupPage", "\347\224\263\350\257\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateGroupPage: public Ui_CreateGroupPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEGROUPPAGE_H
