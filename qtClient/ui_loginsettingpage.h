/********************************************************************************
** Form generated from reading UI file 'loginsettingpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINSETTINGPAGE_H
#define UI_LOGINSETTINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginSettingPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *label_9;
    QComboBox *comboBox;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_7;
    QLineEdit *lineEdit_2;
    QLabel *label_5;
    QLineEdit *lineEdit_7;
    QLabel *label_4;
    QLineEdit *lineEdit_5;
    QLabel *label_8;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *label_3;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *returnBtn;
    QSpacerItem *horizontalSpacer;
    QToolButton *restoreBtn;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *applyBtn;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *LoginSettingPage)
    {
        if (LoginSettingPage->objectName().isEmpty())
            LoginSettingPage->setObjectName(QString::fromUtf8("LoginSettingPage"));
        LoginSettingPage->resize(495, 456);
        verticalLayout = new QVBoxLayout(LoginSettingPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(LoginSettingPage);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(16777215, 30));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_6 = new QLabel(LoginSettingPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        label_9 = new QLabel(LoginSettingPage);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        comboBox = new QComboBox(LoginSettingPage);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 7, 1, 1, 1);

        label_2 = new QLabel(LoginSettingPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        lineEdit = new QLineEdit(LoginSettingPage);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        lineEdit_3 = new QLineEdit(LoginSettingPage);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 2, 1, 1, 1);

        lineEdit_4 = new QLineEdit(LoginSettingPage);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout->addWidget(lineEdit_4, 3, 1, 1, 1);

        label_7 = new QLabel(LoginSettingPage);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        lineEdit_2 = new QLineEdit(LoginSettingPage);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        label_5 = new QLabel(LoginSettingPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        lineEdit_7 = new QLineEdit(LoginSettingPage);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        gridLayout->addWidget(lineEdit_7, 8, 1, 1, 1);

        label_4 = new QLabel(LoginSettingPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lineEdit_5 = new QLineEdit(LoginSettingPage);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_5, 4, 1, 1, 1);

        label_8 = new QLabel(LoginSettingPage);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 8, 0, 1, 1);

        widget = new QWidget(LoginSettingPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_2->addWidget(radioButton);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_2->addWidget(radioButton_2);


        horizontalLayout->addWidget(groupBox);


        gridLayout->addWidget(widget, 5, 1, 1, 1);

        label_3 = new QLabel(LoginSettingPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        widget_2 = new QWidget(LoginSettingPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        returnBtn = new QToolButton(widget_2);
        returnBtn->setObjectName(QString::fromUtf8("returnBtn"));
        sizePolicy1.setHeightForWidth(returnBtn->sizePolicy().hasHeightForWidth());
        returnBtn->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(returnBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        restoreBtn = new QToolButton(widget_2);
        restoreBtn->setObjectName(QString::fromUtf8("restoreBtn"));
        sizePolicy1.setHeightForWidth(restoreBtn->sizePolicy().hasHeightForWidth());
        restoreBtn->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(restoreBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        applyBtn = new QToolButton(widget_2);
        applyBtn->setObjectName(QString::fromUtf8("applyBtn"));
        sizePolicy1.setHeightForWidth(applyBtn->sizePolicy().hasHeightForWidth());
        applyBtn->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(applyBtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addWidget(widget_2);


        retranslateUi(LoginSettingPage);

        QMetaObject::connectSlotsByName(LoginSettingPage);
    } // setupUi

    void retranslateUi(QWidget *LoginSettingPage)
    {
        LoginSettingPage->setWindowTitle(QCoreApplication::translate("LoginSettingPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("LoginSettingPage", "Setting", nullptr));
        label_6->setText(QCoreApplication::translate("LoginSettingPage", "FTPPASSWORD", nullptr));
        label_9->setText(QCoreApplication::translate("LoginSettingPage", "proxyType", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("LoginSettingPage", "sock5", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("LoginSettingPage", "sock", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("LoginSettingPage", "http", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("LoginSettingPage", "other", nullptr));

        label_2->setText(QCoreApplication::translate("LoginSettingPage", "SERVERIP", nullptr));
        lineEdit->setText(QCoreApplication::translate("LoginSettingPage", "192.168.58.132", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("LoginSettingPage", "21", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("LoginSettingPage", "huwei", nullptr));
        label_7->setText(QCoreApplication::translate("LoginSettingPage", "proxy", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("LoginSettingPage", "192.168.58.132", nullptr));
        label_5->setText(QCoreApplication::translate("LoginSettingPage", "FTPUSER", nullptr));
        label_4->setText(QCoreApplication::translate("LoginSettingPage", "FTPPORT", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("LoginSettingPage", "123456", nullptr));
        label_8->setText(QCoreApplication::translate("LoginSettingPage", "proxyIP", nullptr));
        groupBox->setTitle(QString());
        radioButton->setText(QCoreApplication::translate("LoginSettingPage", "open", nullptr));
        radioButton_2->setText(QCoreApplication::translate("LoginSettingPage", "close", nullptr));
        label_3->setText(QCoreApplication::translate("LoginSettingPage", "FTPIP", nullptr));
        returnBtn->setText(QCoreApplication::translate("LoginSettingPage", "\350\277\224\345\233\236", nullptr));
        restoreBtn->setText(QCoreApplication::translate("LoginSettingPage", "\345\272\224\347\224\250", nullptr));
        applyBtn->setText(QCoreApplication::translate("LoginSettingPage", "\350\277\230\345\216\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginSettingPage: public Ui_LoginSettingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSETTINGPAGE_H
