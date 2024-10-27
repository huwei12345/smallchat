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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupChatWindow
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton;
    QLabel *label;
    QLabel *label_2;
    QToolButton *toolButton_12;
    QToolButton *toolButton_10;
    QToolButton *toolButton_13;
    QToolButton *toolButton_11;
    QLabel *label_5;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *plainTextEdit;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QToolButton *fontBtn;
    QToolButton *cutImageBtn;
    QToolButton *imageBtn;
    QToolButton *emojiBtn;
    QToolButton *fileBtn;
    QToolButton *gifBtn;
    QToolButton *sharkBtn;
    QToolButton *testBtn;
    QSpacerItem *horizontalSpacer;
    QTextEdit *plainTextEdit_2;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_4;
    QListWidget *listWidget_2;
    QLabel *label_4;
    QListWidget *listWidget;

    void setupUi(QWidget *GroupChatWindow)
    {
        if (GroupChatWindow->objectName().isEmpty())
            GroupChatWindow->setObjectName(QString::fromUtf8("GroupChatWindow"));
        GroupChatWindow->resize(855, 622);
        verticalLayout_3 = new QVBoxLayout(GroupChatWindow);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        widget = new QWidget(GroupChatWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
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


        verticalLayout_3->addWidget(widget);

        label_5 = new QLabel(GroupChatWindow);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_3->addWidget(label_5);

        widget_6 = new QWidget(GroupChatWindow);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        horizontalLayout_4 = new QHBoxLayout(widget_6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, -1);
        widget_2 = new QWidget(widget_6);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
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

        fontBtn = new QToolButton(widget_3);
        fontBtn->setObjectName(QString::fromUtf8("fontBtn"));

        horizontalLayout_2->addWidget(fontBtn);

        cutImageBtn = new QToolButton(widget_3);
        cutImageBtn->setObjectName(QString::fromUtf8("cutImageBtn"));

        horizontalLayout_2->addWidget(cutImageBtn);

        imageBtn = new QToolButton(widget_3);
        imageBtn->setObjectName(QString::fromUtf8("imageBtn"));

        horizontalLayout_2->addWidget(imageBtn);

        emojiBtn = new QToolButton(widget_3);
        emojiBtn->setObjectName(QString::fromUtf8("emojiBtn"));

        horizontalLayout_2->addWidget(emojiBtn);

        fileBtn = new QToolButton(widget_3);
        fileBtn->setObjectName(QString::fromUtf8("fileBtn"));

        horizontalLayout_2->addWidget(fileBtn);

        gifBtn = new QToolButton(widget_3);
        gifBtn->setObjectName(QString::fromUtf8("gifBtn"));

        horizontalLayout_2->addWidget(gifBtn);

        sharkBtn = new QToolButton(widget_3);
        sharkBtn->setObjectName(QString::fromUtf8("sharkBtn"));

        horizontalLayout_2->addWidget(sharkBtn);

        testBtn = new QToolButton(widget_3);
        testBtn->setObjectName(QString::fromUtf8("testBtn"));

        horizontalLayout_2->addWidget(testBtn);

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

        verticalLayout->setStretch(0, 20);
        verticalLayout->setStretch(1, 10);

        horizontalLayout_4->addWidget(widget_2);

        widget_7 = new QWidget(widget_6);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_7->sizePolicy().hasHeightForWidth());
        widget_7->setSizePolicy(sizePolicy2);
        verticalLayout_4 = new QVBoxLayout(widget_7);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 0, -1, -1);
        listWidget_2 = new QListWidget(widget_7);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(listWidget_2->sizePolicy().hasHeightForWidth());
        listWidget_2->setSizePolicy(sizePolicy3);

        verticalLayout_4->addWidget(listWidget_2);

        label_4 = new QLabel(widget_7);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        listWidget = new QListWidget(widget_7);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        sizePolicy3.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy3);

        verticalLayout_4->addWidget(listWidget);


        horizontalLayout_4->addWidget(widget_7);


        verticalLayout_3->addWidget(widget_6);


        retranslateUi(GroupChatWindow);

        QMetaObject::connectSlotsByName(GroupChatWindow);
    } // setupUi

    void retranslateUi(QWidget *GroupChatWindow)
    {
        GroupChatWindow->setWindowTitle(QCoreApplication::translate("GroupChatWindow", "Form", nullptr));
        toolButton->setText(QCoreApplication::translate("GroupChatWindow", "\347\276\244\345\244\264\345\203\217", nullptr));
        label->setText(QCoreApplication::translate("GroupChatWindow", "\347\276\244\345\220\215\347\247\260\357\274\210\347\276\244\345\217\267\357\274\211\357\274\232", nullptr));
        label_2->setText(QString());
        toolButton_12->setText(QCoreApplication::translate("GroupChatWindow", "\345\216\206\345\217\262\346\266\210\346\201\257", nullptr));
        toolButton_10->setText(QCoreApplication::translate("GroupChatWindow", "\350\247\206\351\242\221", nullptr));
        toolButton_13->setText(QCoreApplication::translate("GroupChatWindow", "\350\257\255\351\237\263", nullptr));
        toolButton_11->setText(QCoreApplication::translate("GroupChatWindow", "...", nullptr));
        label_5->setText(QCoreApplication::translate("GroupChatWindow", "\351\200\232\345\221\212                                                           ", nullptr));
        label_3->setText(QCoreApplication::translate("GroupChatWindow", "\345\267\245\345\205\267  ", nullptr));
        fontBtn->setText(QCoreApplication::translate("GroupChatWindow", "\345\255\227\344\275\223", nullptr));
        cutImageBtn->setText(QCoreApplication::translate("GroupChatWindow", "\346\210\252\345\233\276", nullptr));
        imageBtn->setText(QCoreApplication::translate("GroupChatWindow", "\345\233\276\347\211\207", nullptr));
        emojiBtn->setText(QCoreApplication::translate("GroupChatWindow", "\350\241\250\346\203\205", nullptr));
        fileBtn->setText(QCoreApplication::translate("GroupChatWindow", "\346\226\207\344\273\266", nullptr));
        gifBtn->setText(QCoreApplication::translate("GroupChatWindow", "GIF", nullptr));
        sharkBtn->setText(QCoreApplication::translate("GroupChatWindow", "\346\212\226\345\212\250", nullptr));
        testBtn->setText(QCoreApplication::translate("GroupChatWindow", "\346\226\207\344\273\2662", nullptr));
        pushButton->setText(QCoreApplication::translate("GroupChatWindow", "\345\217\221\351\200\201", nullptr));
        label_4->setText(QCoreApplication::translate("GroupChatWindow", "\347\276\244\346\210\220\345\221\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GroupChatWindow: public Ui_GroupChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHATWINDOW_H
