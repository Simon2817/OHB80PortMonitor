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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigPage
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widgetTop;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnIdelPurge;
    QPushButton *btnFirmwareUpdate;
    QSpacerItem *horizontalSpacer;
    QWidget *widgetSettings;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QWidget *ConfigPage)
    {
        if (ConfigPage->objectName().isEmpty())
            ConfigPage->setObjectName(QString::fromUtf8("ConfigPage"));
        ConfigPage->resize(400, 300);
        verticalLayout_3 = new QVBoxLayout(ConfigPage);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        widgetTop = new QWidget(ConfigPage);
        widgetTop->setObjectName(QString::fromUtf8("widgetTop"));
        widgetTop->setMinimumSize(QSize(80, 0));
        horizontalLayout = new QHBoxLayout(widgetTop);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 12, 0);
        btnIdelPurge = new QToolButton(widgetTop);
        btnIdelPurge->setObjectName(QString::fromUtf8("btnIdelPurge"));

        horizontalLayout->addWidget(btnIdelPurge);

        btnFirmwareUpdate = new QPushButton(widgetTop);
        btnFirmwareUpdate->setObjectName(QString::fromUtf8("btnFirmwareUpdate"));

        horizontalLayout->addWidget(btnFirmwareUpdate);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(widgetTop);

        widgetSettings = new QWidget(ConfigPage);
        widgetSettings->setObjectName(QString::fromUtf8("widgetSettings"));
        verticalLayout = new QVBoxLayout(widgetSettings);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        scrollArea = new QScrollArea(widgetSettings);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 386, 259));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        verticalLayout_3->addWidget(widgetSettings);


        retranslateUi(ConfigPage);

        QMetaObject::connectSlotsByName(ConfigPage);
    } // setupUi

    void retranslateUi(QWidget *ConfigPage)
    {
        ConfigPage->setWindowTitle(QCoreApplication::translate("ConfigPage", "Form", nullptr));
        btnIdelPurge->setText(QCoreApplication::translate("ConfigPage", "Idle Purge", nullptr));
        btnFirmwareUpdate->setText(QCoreApplication::translate("ConfigPage", "Firmware Up", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigPage: public Ui_ConfigPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGPAGE_H
