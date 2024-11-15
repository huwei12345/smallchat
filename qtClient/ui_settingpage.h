/********************************************************************************
** Form generated from reading UI file 'settingpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGPAGE_H
#define UI_SETTINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_settingPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *comboBox;
    QLabel *label_3;
    QSpinBox *spinBox;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QLabel *label_5;
    QCheckBox *checkBox;
    QLabel *label_6;
    QCheckBox *checkBox_3;
    QLabel *label_7;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QLabel *label_9;
    QCheckBox *checkBox_2;
    QLabel *label_8;
    QComboBox *comboBox_3;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;

    void setupUi(QWidget *settingPage)
    {
        if (settingPage->objectName().isEmpty())
            settingPage->setObjectName(QString::fromUtf8("settingPage"));
        settingPage->resize(665, 590);
        verticalLayout = new QVBoxLayout(settingPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(settingPage);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        widget = new QWidget(settingPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        comboBox = new QComboBox(widget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        spinBox = new QSpinBox(widget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout->addWidget(spinBox, 1, 1, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        comboBox_2 = new QComboBox(widget);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 2, 1, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        checkBox = new QCheckBox(widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout->addWidget(checkBox, 3, 1, 1, 1);

        label_6 = new QLabel(widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        checkBox_3 = new QCheckBox(widget);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        gridLayout->addWidget(checkBox_3, 4, 1, 1, 1);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(widget_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);


        horizontalLayout->addWidget(groupBox);


        gridLayout->addWidget(widget_3, 5, 1, 1, 1);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 6, 0, 1, 1);

        checkBox_2 = new QCheckBox(widget);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout->addWidget(checkBox_2, 6, 1, 1, 1);

        label_8 = new QLabel(widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        comboBox_3 = new QComboBox(widget);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        gridLayout->addWidget(comboBox_3, 7, 1, 1, 1);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(settingPage);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        toolButton = new QToolButton(widget_2);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        horizontalLayout_3->addWidget(toolButton);

        toolButton_2 = new QToolButton(widget_2);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        horizontalLayout_3->addWidget(toolButton_2);

        toolButton_3 = new QToolButton(widget_2);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));

        horizontalLayout_3->addWidget(toolButton_3);

        toolButton_4 = new QToolButton(widget_2);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));

        horizontalLayout_3->addWidget(toolButton_4);


        verticalLayout->addWidget(widget_2);


        retranslateUi(settingPage);

        QMetaObject::connectSlotsByName(settingPage);
    } // setupUi

    void retranslateUi(QWidget *settingPage)
    {
        settingPage->setWindowTitle(QCoreApplication::translate("settingPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("settingPage", "Setting", nullptr));
        label_2->setText(QCoreApplication::translate("settingPage", "\347\225\214\351\235\242\351\243\216\346\240\274", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("settingPage", "\347\273\217\345\205\270", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("settingPage", "\346\232\227\351\273\221", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("settingPage", "\345\215\241\351\200\232", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("settingPage", "\345\233\275\351\243\216", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("settingPage", "\345\205\266\344\273\226", nullptr));

        label_3->setText(QCoreApplication::translate("settingPage", "\345\255\227\344\275\223", nullptr));
        label_4->setText(QCoreApplication::translate("settingPage", "\351\273\230\350\256\244\347\212\266\346\200\201", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("settingPage", "\345\234\250\347\272\277", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("settingPage", "\345\205\215\346\211\223\346\211\260", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("settingPage", "\351\232\220\350\272\253", nullptr));

        label_5->setText(QCoreApplication::translate("settingPage", "\350\207\252\345\220\257\345\212\250", nullptr));
        checkBox->setText(QCoreApplication::translate("settingPage", "\345\274\200\345\220\257", nullptr));
        label_6->setText(QCoreApplication::translate("settingPage", "\350\207\252\346\233\264\346\226\260", nullptr));
        checkBox_3->setText(QCoreApplication::translate("settingPage", "\350\207\252\346\233\264\346\226\260", nullptr));
        label_7->setText(QCoreApplication::translate("settingPage", "\346\226\207\344\273\266\344\275\215\347\275\256", nullptr));
        groupBox->setTitle(QString());
        label_9->setText(QCoreApplication::translate("settingPage", "\344\277\235\347\225\231\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
        checkBox_2->setText(QCoreApplication::translate("settingPage", "\344\277\235\347\225\231", nullptr));
        label_8->setText(QCoreApplication::translate("settingPage", "\351\200\232\347\237\245\346\226\271\345\274\217", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("settingPage", "\346\234\211\345\243\260\346\234\211\351\227\252\347\203\201", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("settingPage", "\346\227\240\345\243\260\346\234\211\351\227\252\347\203\201", nullptr));
        comboBox_3->setItemText(2, QCoreApplication::translate("settingPage", "\346\227\240\345\243\260\346\234\211\351\227\252\347\203\201", nullptr));
        comboBox_3->setItemText(3, QCoreApplication::translate("settingPage", "\346\234\211\345\243\260\346\227\240\351\227\252\347\203\201", nullptr));

        toolButton->setText(QCoreApplication::translate("settingPage", "\351\200\200\345\207\272", nullptr));
        toolButton_2->setText(QCoreApplication::translate("settingPage", "\345\272\224\347\224\250", nullptr));
        toolButton_3->setText(QCoreApplication::translate("settingPage", "\351\207\215\347\275\256", nullptr));
        toolButton_4->setText(QCoreApplication::translate("settingPage", "\346\263\250\351\224\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class settingPage: public Ui_settingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGPAGE_H
