/********************************************************************************
** Form generated from reading UI file 'mainpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINPAGE_H
#define UI_MAINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainPage
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton;
    QToolButton *toolButton_4;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;

    void setupUi(QWidget *MainPage)
    {
        if (MainPage->objectName().isEmpty())
            MainPage->setObjectName(QString::fromUtf8("MainPage"));
        MainPage->resize(605, 515);
        verticalLayout_2 = new QVBoxLayout(MainPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget_3 = new QWidget(MainPage);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(11, 11, 11, 0);
        label = new QLabel(widget_3);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(22);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout_2->addWidget(widget_3);

        widget_4 = new QWidget(MainPage);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        toolButton = new QToolButton(widget_4);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout_3->addWidget(toolButton);

        toolButton_4 = new QToolButton(widget_4);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_3->addWidget(toolButton_4);

        toolButton_2 = new QToolButton(widget_4);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_3->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(widget_4);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_3->addWidget(toolButton_3);


        verticalLayout_2->addWidget(widget_4);

        widget_2 = new QWidget(MainPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(widget_2);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);
        treeWidget->setMinimumSize(QSize(180, 0));
        treeWidget->setMaximumSize(QSize(1000, 16777215));

        horizontalLayout->addWidget(treeWidget);

        widget = new QWidget(widget_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setMinimumSize(QSize(360, 280));

        verticalLayout->addWidget(plainTextEdit);

        plainTextEdit_2 = new QPlainTextEdit(widget);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(plainTextEdit_2->sizePolicy().hasHeightForWidth());
        plainTextEdit_2->setSizePolicy(sizePolicy2);
        plainTextEdit_2->setMinimumSize(QSize(0, 100));
        plainTextEdit_2->setMaximumSize(QSize(16777215, 300));

        verticalLayout->addWidget(plainTextEdit_2);


        horizontalLayout->addWidget(widget);


        verticalLayout_2->addWidget(widget_2);


        retranslateUi(MainPage);

        QMetaObject::connectSlotsByName(MainPage);
    } // setupUi

    void retranslateUi(QWidget *MainPage)
    {
        MainPage->setWindowTitle(QCoreApplication::translate("MainPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("MainPage", "\344\270\252\344\272\272\347\251\272\351\227\264", nullptr));
        pushButton->setText(QCoreApplication::translate("MainPage", "\350\277\224\345\233\236", nullptr));
        toolButton->setText(QCoreApplication::translate("MainPage", "\344\270\212\344\274\240", nullptr));
        toolButton_4->setText(QCoreApplication::translate("MainPage", "\345\220\214\346\255\245", nullptr));
        toolButton_2->setText(QCoreApplication::translate("MainPage", "\346\216\250\351\200\201", nullptr));
        toolButton_3->setText(QCoreApplication::translate("MainPage", "Test", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("MainPage", "size", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainPage", "path", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainPage", "name", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(2, QCoreApplication::translate("MainPage", "0", nullptr));
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("MainPage", "/", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainPage", "root", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MainPage: public Ui_MainPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINPAGE_H
