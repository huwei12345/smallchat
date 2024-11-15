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
    QLineEdit *ftpIpEdit;
    QLabel *label_5;
    QLineEdit *ftpUserEdit;
    QLabel *label_10;
    QLabel *label_6;
    QLineEdit *ftpPortEdit;
    QLabel *label_3;
    QLabel *label_2;
    QComboBox *proxyTypeEdit;
    QLabel *label_7;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *serverIpEdit;
    QLineEdit *proxyIPEdit;
    QLineEdit *ftpPasswdEdit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *openProxyBtn;
    QRadioButton *closeProxyBtn;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
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
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(16777215, 30));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ftpIpEdit = new QLineEdit(LoginSettingPage);
        ftpIpEdit->setObjectName(QString::fromUtf8("ftpIpEdit"));

        gridLayout->addWidget(ftpIpEdit, 1, 1, 1, 1);

        label_5 = new QLabel(LoginSettingPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        ftpUserEdit = new QLineEdit(LoginSettingPage);
        ftpUserEdit->setObjectName(QString::fromUtf8("ftpUserEdit"));

        gridLayout->addWidget(ftpUserEdit, 3, 1, 1, 1);

        label_10 = new QLabel(LoginSettingPage);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 9, 0, 1, 1);

        label_6 = new QLabel(LoginSettingPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        ftpPortEdit = new QLineEdit(LoginSettingPage);
        ftpPortEdit->setObjectName(QString::fromUtf8("ftpPortEdit"));

        gridLayout->addWidget(ftpPortEdit, 2, 1, 1, 1);

        label_3 = new QLabel(LoginSettingPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(LoginSettingPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        proxyTypeEdit = new QComboBox(LoginSettingPage);
        proxyTypeEdit->addItem(QString());
        proxyTypeEdit->addItem(QString());
        proxyTypeEdit->addItem(QString());
        proxyTypeEdit->addItem(QString());
        proxyTypeEdit->setObjectName(QString::fromUtf8("proxyTypeEdit"));

        gridLayout->addWidget(proxyTypeEdit, 7, 1, 1, 1);

        label_7 = new QLabel(LoginSettingPage);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        label_4 = new QLabel(LoginSettingPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        label_8 = new QLabel(LoginSettingPage);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 8, 0, 1, 1);

        label_9 = new QLabel(LoginSettingPage);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        serverIpEdit = new QLineEdit(LoginSettingPage);
        serverIpEdit->setObjectName(QString::fromUtf8("serverIpEdit"));

        gridLayout->addWidget(serverIpEdit, 0, 1, 1, 1);

        proxyIPEdit = new QLineEdit(LoginSettingPage);
        proxyIPEdit->setObjectName(QString::fromUtf8("proxyIPEdit"));

        gridLayout->addWidget(proxyIPEdit, 8, 1, 1, 1);

        ftpPasswdEdit = new QLineEdit(LoginSettingPage);
        ftpPasswdEdit->setObjectName(QString::fromUtf8("ftpPasswdEdit"));
        ftpPasswdEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(ftpPasswdEdit, 4, 1, 1, 1);

        widget = new QWidget(LoginSettingPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        openProxyBtn = new QRadioButton(groupBox);
        openProxyBtn->setObjectName(QString::fromUtf8("openProxyBtn"));

        horizontalLayout_2->addWidget(openProxyBtn);

        closeProxyBtn = new QRadioButton(groupBox);
        closeProxyBtn->setObjectName(QString::fromUtf8("closeProxyBtn"));

        horizontalLayout_2->addWidget(closeProxyBtn);


        horizontalLayout->addWidget(groupBox);


        gridLayout->addWidget(widget, 5, 1, 1, 1);

        groupBox_3 = new QGroupBox(LoginSettingPage);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        radioButton = new QRadioButton(groupBox_3);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_4->addWidget(radioButton);

        radioButton_2 = new QRadioButton(groupBox_3);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_4->addWidget(radioButton_2);


        gridLayout->addWidget(groupBox_3, 9, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        widget_2 = new QWidget(LoginSettingPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        returnBtn = new QToolButton(widget_2);
        returnBtn->setObjectName(QString::fromUtf8("returnBtn"));
        sizePolicy.setHeightForWidth(returnBtn->sizePolicy().hasHeightForWidth());
        returnBtn->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(returnBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        restoreBtn = new QToolButton(widget_2);
        restoreBtn->setObjectName(QString::fromUtf8("restoreBtn"));
        sizePolicy.setHeightForWidth(restoreBtn->sizePolicy().hasHeightForWidth());
        restoreBtn->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(restoreBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        applyBtn = new QToolButton(widget_2);
        applyBtn->setObjectName(QString::fromUtf8("applyBtn"));
        sizePolicy.setHeightForWidth(applyBtn->sizePolicy().hasHeightForWidth());
        applyBtn->setSizePolicy(sizePolicy);

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
        ftpIpEdit->setText(QCoreApplication::translate("LoginSettingPage", "192.168.58.132", nullptr));
        label_5->setText(QCoreApplication::translate("LoginSettingPage", "FTPUSER", nullptr));
        ftpUserEdit->setText(QCoreApplication::translate("LoginSettingPage", "huwei", nullptr));
        label_10->setText(QCoreApplication::translate("LoginSettingPage", "\350\277\236\346\216\245\346\226\271\345\274\217", nullptr));
        label_6->setText(QCoreApplication::translate("LoginSettingPage", "FTPPASSWORD", nullptr));
        ftpPortEdit->setText(QCoreApplication::translate("LoginSettingPage", "21", nullptr));
        label_3->setText(QCoreApplication::translate("LoginSettingPage", "FTPIP", nullptr));
        label_2->setText(QCoreApplication::translate("LoginSettingPage", "SERVERIP", nullptr));
        proxyTypeEdit->setItemText(0, QCoreApplication::translate("LoginSettingPage", "sock5", nullptr));
        proxyTypeEdit->setItemText(1, QCoreApplication::translate("LoginSettingPage", "sock", nullptr));
        proxyTypeEdit->setItemText(2, QCoreApplication::translate("LoginSettingPage", "http", nullptr));
        proxyTypeEdit->setItemText(3, QCoreApplication::translate("LoginSettingPage", "other", nullptr));

        label_7->setText(QCoreApplication::translate("LoginSettingPage", "proxy", nullptr));
        label_4->setText(QCoreApplication::translate("LoginSettingPage", "FTPPORT", nullptr));
        label_8->setText(QCoreApplication::translate("LoginSettingPage", "proxyIP", nullptr));
        label_9->setText(QCoreApplication::translate("LoginSettingPage", "proxyType", nullptr));
        serverIpEdit->setText(QCoreApplication::translate("LoginSettingPage", "192.168.58.132", nullptr));
        ftpPasswdEdit->setText(QCoreApplication::translate("LoginSettingPage", "123456", nullptr));
        groupBox->setTitle(QString());
        openProxyBtn->setText(QCoreApplication::translate("LoginSettingPage", "open", nullptr));
        closeProxyBtn->setText(QCoreApplication::translate("LoginSettingPage", "close", nullptr));
        radioButton->setText(QCoreApplication::translate("LoginSettingPage", "\346\234\215\345\212\241\345\231\250", nullptr));
        radioButton_2->setText(QCoreApplication::translate("LoginSettingPage", "\347\233\264\350\277\236", nullptr));
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
