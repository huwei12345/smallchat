/********************************************************************************
** Form generated from reading UI file 'findfriendpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDFRIENDPAGE_H
#define UI_FINDFRIENDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FindFriendPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_2;

    void setupUi(QWidget *FindFriendPage)
    {
        if (FindFriendPage->objectName().isEmpty())
            FindFriendPage->setObjectName(QString::fromUtf8("FindFriendPage"));
        FindFriendPage->resize(566, 429);
        verticalLayout = new QVBoxLayout(FindFriendPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(FindFriendPage);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(18);
        font.setBold(false);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        groupBox = new QGroupBox(FindFriendPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_3->addWidget(radioButton_2);

        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_3->addWidget(radioButton);


        verticalLayout->addWidget(groupBox);

        widget = new QWidget(FindFriendPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addWidget(widget);

        listWidget = new QListWidget(FindFriendPage);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);

        widget_2 = new QWidget(FindFriendPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(widget_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addWidget(widget_2);


        retranslateUi(FindFriendPage);

        QMetaObject::connectSlotsByName(FindFriendPage);
    } // setupUi

    void retranslateUi(QWidget *FindFriendPage)
    {
        FindFriendPage->setWindowTitle(QCoreApplication::translate("FindFriendPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("FindFriendPage", "\346\237\245\346\211\276\346\234\213\345\217\213", nullptr));
        groupBox->setTitle(QString());
        radioButton_2->setText(QCoreApplication::translate("FindFriendPage", "\346\237\245\346\211\276\344\272\272", nullptr));
        radioButton->setText(QCoreApplication::translate("FindFriendPage", "\346\237\245\346\211\276\347\276\244", nullptr));
        label_2->setText(QCoreApplication::translate("FindFriendPage", "\347\224\250\346\210\267", nullptr));
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("FindFriendPage", "\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215/\350\264\246\345\217\267", nullptr));
        pushButton->setText(QCoreApplication::translate("FindFriendPage", "\346\237\245\346\211\276", nullptr));
        pushButton_3->setText(QCoreApplication::translate("FindFriendPage", "\350\257\246\347\273\206\344\277\241\346\201\257", nullptr));
        pushButton_2->setText(QCoreApplication::translate("FindFriendPage", "\346\267\273\345\212\240", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindFriendPage: public Ui_FindFriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDFRIENDPAGE_H
