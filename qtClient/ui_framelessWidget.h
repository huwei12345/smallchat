/********************************************************************************
** Form generated from reading UI file 'framelessWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMELESSWIDGET_H
#define UI_FRAMELESSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_framelessWidget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QWidget *mainWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *mainLayout;
    QWidget *titleBar;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Button_close;
    QWidget *mainDisplayWidget;

    void setupUi(QWidget *framelessWidget)
    {
        if (framelessWidget->objectName().isEmpty())
            framelessWidget->setObjectName(QString::fromUtf8("framelessWidget"));
        framelessWidget->resize(579, 462);
        framelessWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(framelessWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(30, 30, 30, 30);
        mainWidget = new QWidget(framelessWidget);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        verticalLayout_3 = new QVBoxLayout(mainWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(0);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        titleBar = new QWidget(mainWidget);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 30));
        titleBar->setMaximumSize(QSize(16777215, 30));
        titleBar->setStyleSheet(QString::fromUtf8("#titleBar{\n"
"\n"
"background-color: rgb(251, 251, 251);}"));
        verticalLayout_2 = new QVBoxLayout(titleBar);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 10, 10, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Button_close = new QPushButton(titleBar);
        Button_close->setObjectName(QString::fromUtf8("Button_close"));
        Button_close->setMinimumSize(QSize(18, 18));
        Button_close->setMaximumSize(QSize(18, 18));
        Button_close->setStyleSheet(QString::fromUtf8("QPushButton{background-color:#c2c2c2;border-radius:9px}\n"
"QPushButton:hover {background-color:#cb4042;border-radius:9px;}\n"
"QPushButton:pressed {background-color:#ab3b3a;border-radius:9px;}"));

        horizontalLayout->addWidget(Button_close);


        verticalLayout_2->addLayout(horizontalLayout);


        mainLayout->addWidget(titleBar);

        mainDisplayWidget = new QWidget(mainWidget);
        mainDisplayWidget->setObjectName(QString::fromUtf8("mainDisplayWidget"));
        mainDisplayWidget->setStyleSheet(QString::fromUtf8("#mainDisplayWidget{\n"
"background-color: rgb(251, 251, 251);}"));

        mainLayout->addWidget(mainDisplayWidget);


        verticalLayout_3->addLayout(mainLayout);


        verticalLayout->addWidget(mainWidget);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(framelessWidget);

        QMetaObject::connectSlotsByName(framelessWidget);
    } // setupUi

    void retranslateUi(QWidget *framelessWidget)
    {
        framelessWidget->setWindowTitle(QCoreApplication::translate("framelessWidget", "MainWindow", nullptr));
        Button_close->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class framelessWidget: public Ui_framelessWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMELESSWIDGET_H
