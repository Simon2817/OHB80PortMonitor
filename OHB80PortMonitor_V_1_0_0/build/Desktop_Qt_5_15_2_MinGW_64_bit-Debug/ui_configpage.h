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
    QToolButton *btnPneumaticValvePressure;
    QToolButton *btnSH85PeriodicSelfCheck;
    QToolButton *btnSH85SelfCheck;
    QToolButton *btnHumidityOffset;
    QToolButton *btnPurgeFlow;
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
        ConfigPage->resize(506, 425);
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

        btnPneumaticValvePressure = new QToolButton(widgetTop);
        btnPneumaticValvePressure->setObjectName(QString::fromUtf8("btnPneumaticValvePressure"));

        horizontalLayout->addWidget(btnPneumaticValvePressure);

        btnSH85PeriodicSelfCheck = new QToolButton(widgetTop);
        btnSH85PeriodicSelfCheck->setObjectName(QString::fromUtf8("btnSH85PeriodicSelfCheck"));

        horizontalLayout->addWidget(btnSH85PeriodicSelfCheck);

        btnSH85SelfCheck = new QToolButton(widgetTop);
        btnSH85SelfCheck->setObjectName(QString::fromUtf8("btnSH85SelfCheck"));

        horizontalLayout->addWidget(btnSH85SelfCheck);

        btnHumidityOffset = new QToolButton(widgetTop);
        btnHumidityOffset->setObjectName(QString::fromUtf8("btnHumidityOffset"));

        horizontalLayout->addWidget(btnHumidityOffset);

        btnPurgeFlow = new QToolButton(widgetTop);
        btnPurgeFlow->setObjectName(QString::fromUtf8("btnPurgeFlow"));

        horizontalLayout->addWidget(btnPurgeFlow);

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
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 492, 386));
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
        btnPneumaticValvePressure->setText(QCoreApplication::translate("ConfigPage", "Pneumatic Valve", nullptr));
        btnSH85PeriodicSelfCheck->setText(QCoreApplication::translate("ConfigPage", "SH85 Periodic", nullptr));
        btnSH85SelfCheck->setText(QCoreApplication::translate("ConfigPage", "SH85 Self-check", nullptr));
        btnHumidityOffset->setText(QCoreApplication::translate("ConfigPage", "Humidity Offset", nullptr));
        btnPurgeFlow->setText(QCoreApplication::translate("ConfigPage", "Purge Flow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigPage: public Ui_ConfigPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGPAGE_H
