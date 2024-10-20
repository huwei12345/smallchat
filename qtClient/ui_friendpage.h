/********************************************************************************
** Form generated from reading UI file 'friendpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDPAGE_H
#define UI_FRIENDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendPage
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *toolButton_5;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QWidget *tab1;
    QHBoxLayout *horizontalLayout_4;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_5;
    QListWidget *listWidget_3;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_6;
    QListWidget *listWidget_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton;
    QToolButton *toolButton_3;
    QToolButton *toolButton_2;

    void setupUi(QWidget *FriendPage)
    {
        if (FriendPage->objectName().isEmpty())
            FriendPage->setObjectName(QString::fromUtf8("FriendPage"));
        FriendPage->resize(271, 627);
        verticalLayout_3 = new QVBoxLayout(FriendPage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 4, 0, 4);
        widget = new QWidget(FriendPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMaximumSize(QSize(16777215, 80));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        toolButton_5 = new QToolButton(widget_2);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));
        toolButton_5->setMinimumSize(QSize(60, 60));
        toolButton_5->setMaximumSize(QSize(60, 60));

        horizontalLayout_2->addWidget(toolButton_5);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        comboBox = new QComboBox(widget_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(false);

        horizontalLayout_2->addWidget(comboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(widget_2);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        verticalLayout_2 = new QVBoxLayout(widget_4);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(widget_4);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        tabWidget->addTab(tab, QString());
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        horizontalLayout_4 = new QHBoxLayout(tab1);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(3, 0, 3, 0);
        tabWidget_2 = new QTabWidget(tab1);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setLayoutDirection(Qt::LeftToRight);
        tabWidget_2->setAutoFillBackground(false);
        tabWidget_2->setTabPosition(QTabWidget::North);
        tabWidget_2->setTabShape(QTabWidget::Triangular);
        tabWidget_2->setElideMode(Qt::ElideRight);
        tabWidget_2->setUsesScrollButtons(true);
        tabWidget_2->setDocumentMode(false);
        tabWidget_2->setTabsClosable(false);
        tabWidget_2->setMovable(false);
        tabWidget_2->setTabBarAutoHide(false);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_5 = new QHBoxLayout(tab_3);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        listWidget_3 = new QListWidget(tab_3);
        listWidget_3->setObjectName(QString::fromUtf8("listWidget_3"));

        horizontalLayout_5->addWidget(listWidget_3);

        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_6 = new QHBoxLayout(tab_4);
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        listWidget_2 = new QListWidget(tab_4);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

        horizontalLayout_6->addWidget(listWidget_2);

        tabWidget_2->addTab(tab_4, QString());

        horizontalLayout_4->addWidget(tabWidget_2);

        tabWidget->addTab(tab1, QString());

        verticalLayout_2->addWidget(tabWidget);


        verticalLayout->addWidget(widget_4);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy);
        widget_3->setMaximumSize(QSize(16777215, 40));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        toolButton_4 = new QToolButton(widget_3);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_3->addWidget(toolButton_4);

        toolButton = new QToolButton(widget_3);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout_3->addWidget(toolButton);

        toolButton_3 = new QToolButton(widget_3);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_3->addWidget(toolButton_3);

        toolButton_2 = new QToolButton(widget_3);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_3->addWidget(toolButton_2);


        verticalLayout->addWidget(widget_3);


        verticalLayout_3->addWidget(widget);


        retranslateUi(FriendPage);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FriendPage);
    } // setupUi

    void retranslateUi(QWidget *FriendPage)
    {
        FriendPage->setWindowTitle(QCoreApplication::translate("FriendPage", "Form", nullptr));
        toolButton_5->setText(QCoreApplication::translate("FriendPage", "\345\244\264\345\203\217", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("FriendPage", "\345\234\250\347\272\277", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("FriendPage", "\351\232\220\350\272\253", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("FriendPage", "\347\246\273\347\272\277", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("FriendPage", "\345\277\231\347\242\214", nullptr));

        comboBox->setCurrentText(QCoreApplication::translate("FriendPage", "\345\234\250\347\272\277", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("FriendPage", "\346\266\210\346\201\257", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QCoreApplication::translate("FriendPage", "\345\245\275\345\217\213", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QCoreApplication::translate("FriendPage", "\347\276\244", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QCoreApplication::translate("FriendPage", "\345\205\263\347\263\273", nullptr));
        toolButton_4->setText(QCoreApplication::translate("FriendPage", "...", nullptr));
        toolButton->setText(QCoreApplication::translate("FriendPage", "\346\237\245\346\211\276", nullptr));
        toolButton_3->setText(QCoreApplication::translate("FriendPage", "\345\210\233\345\273\272\347\276\244", nullptr));
        toolButton_2->setText(QCoreApplication::translate("FriendPage", "\345\245\275\345\217\213\350\257\267\346\261\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendPage: public Ui_FriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDPAGE_H
