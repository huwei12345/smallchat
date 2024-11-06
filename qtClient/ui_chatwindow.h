/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton;
    QLabel *label;
    QLabel *label_2;
    QToolButton *toolButton_12;
    QToolButton *toolButton_10;
    QToolButton *toolButton_13;
    QToolButton *toolButton_11;
    QTextEdit *plainTextEdit;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_8;
    QToolButton *toolButton_4;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_9;
    QSpacerItem *horizontalSpacer;
    QTextEdit *plainTextEdit_2;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;

    void setupUi(QWidget *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName(QString::fromUtf8("ChatWindow"));
        ChatWindow->resize(522, 461);
        horizontalLayout_4 = new QHBoxLayout(ChatWindow);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_2 = new QWidget(ChatWindow);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(widget_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        toolButton = new QToolButton(widget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout->addWidget(toolButton);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        toolButton_12 = new QToolButton(widget);
        toolButton_12->setObjectName(QString::fromUtf8("toolButton_12"));

        horizontalLayout->addWidget(toolButton_12);

        toolButton_10 = new QToolButton(widget);
        toolButton_10->setObjectName(QString::fromUtf8("toolButton_10"));

        horizontalLayout->addWidget(toolButton_10);

        toolButton_13 = new QToolButton(widget);
        toolButton_13->setObjectName(QString::fromUtf8("toolButton_13"));

        horizontalLayout->addWidget(toolButton_13);

        toolButton_11 = new QToolButton(widget);
        toolButton_11->setObjectName(QString::fromUtf8("toolButton_11"));

        horizontalLayout->addWidget(toolButton_11);


        verticalLayout->addWidget(widget);

        plainTextEdit = new QTextEdit(widget_2);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setMinimumSize(QSize(0, 150));

        verticalLayout->addWidget(plainTextEdit);

        widget_5 = new QWidget(widget_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy1);
        widget_5->setMinimumSize(QSize(0, 80));
        verticalLayout_2 = new QVBoxLayout(widget_5);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(widget_5);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        toolButton_2 = new QToolButton(widget_3);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_2->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(widget_3);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_2->addWidget(toolButton_3);

        toolButton_8 = new QToolButton(widget_3);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));

        horizontalLayout_2->addWidget(toolButton_8);

        toolButton_4 = new QToolButton(widget_3);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_2->addWidget(toolButton_4);

        toolButton_5 = new QToolButton(widget_3);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));

        horizontalLayout_2->addWidget(toolButton_5);

        toolButton_6 = new QToolButton(widget_3);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));

        horizontalLayout_2->addWidget(toolButton_6);

        toolButton_7 = new QToolButton(widget_3);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));

        horizontalLayout_2->addWidget(toolButton_7);

        toolButton_9 = new QToolButton(widget_3);
        toolButton_9->setObjectName(QString::fromUtf8("toolButton_9"));

        horizontalLayout_2->addWidget(toolButton_9);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget_3);

        plainTextEdit_2 = new QTextEdit(widget_5);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setMinimumSize(QSize(0, 40));

        verticalLayout_2->addWidget(plainTextEdit_2);

        widget_4 = new QWidget(widget_5);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(widget_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_3->addWidget(pushButton);


        verticalLayout_2->addWidget(widget_4);


        verticalLayout->addWidget(widget_5);

        verticalLayout->setStretch(1, 20);
        verticalLayout->setStretch(2, 10);

        horizontalLayout_4->addWidget(widget_2);


        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QWidget *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "Form", nullptr));
        toolButton->setText(QCoreApplication::translate("ChatWindow", "\345\244\264\345\203\217", nullptr));
        label->setText(QCoreApplication::translate("ChatWindow", "\347\224\250\346\210\267\357\274\232", nullptr));
        label_2->setText(QString());
        toolButton_12->setText(QCoreApplication::translate("ChatWindow", "\345\216\206\345\217\262\346\266\210\346\201\257", nullptr));
        toolButton_10->setText(QCoreApplication::translate("ChatWindow", "\350\247\206\351\242\221", nullptr));
        toolButton_13->setText(QCoreApplication::translate("ChatWindow", "\350\257\255\351\237\263", nullptr));
        toolButton_11->setText(QCoreApplication::translate("ChatWindow", "...", nullptr));
        label_3->setText(QCoreApplication::translate("ChatWindow", "\345\267\245\345\205\267  ", nullptr));
        toolButton_2->setText(QCoreApplication::translate("ChatWindow", "\345\255\227\344\275\223", nullptr));
        toolButton_3->setText(QCoreApplication::translate("ChatWindow", "\346\210\252\345\233\276", nullptr));
        toolButton_8->setText(QCoreApplication::translate("ChatWindow", "\345\233\276\347\211\207", nullptr));
        toolButton_4->setText(QCoreApplication::translate("ChatWindow", "\350\241\250\346\203\205", nullptr));
        toolButton_5->setText(QCoreApplication::translate("ChatWindow", "\346\226\207\344\273\266", nullptr));
        toolButton_6->setText(QCoreApplication::translate("ChatWindow", "GIF", nullptr));
        toolButton_7->setText(QCoreApplication::translate("ChatWindow", "\346\212\226\345\212\250", nullptr));
        toolButton_9->setText(QCoreApplication::translate("ChatWindow", "\346\226\207\344\273\2662", nullptr));
        pushButton->setText(QCoreApplication::translate("ChatWindow", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
