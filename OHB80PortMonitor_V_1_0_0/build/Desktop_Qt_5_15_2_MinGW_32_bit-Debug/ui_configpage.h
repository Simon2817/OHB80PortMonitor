/********************************************************************************
** Form generated from reading UI file 'configpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGPAGE_H
#define UI_CONFIGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWidget *ConfigPage)
    {
        if (ConfigPage->objectName().isEmpty())
            ConfigPage->setObjectName(QString::fromUtf8("ConfigPage"));
        ConfigPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(ConfigPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ConfigPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        retranslateUi(ConfigPage);

        QMetaObject::connectSlotsByName(ConfigPage);
    } // setupUi

    void retranslateUi(QWidget *ConfigPage)
    {
        ConfigPage->setWindowTitle(QCoreApplication::translate("ConfigPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("ConfigPage", "Config Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigPage: public Ui_ConfigPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGPAGE_H
