/********************************************************************************
** Form generated from reading UI file 'debugpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGPAGE_H
#define UI_DEBUGPAGE_H

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

class Ui_DebugPage
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widgetTop;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnSetPressure;
    QPushButton *btnFirmwareConfig;
    QSpacerItem *horizontalSpacer;
    QWidget *widgetSettings;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QWidget *DebugPage)
    {
        if (DebugPage->objectName().isEmpty())
            DebugPage->setObjectName(QString::fromUtf8("DebugPage"));
        DebugPage->resize(400, 300);
        verticalLayout_3 = new QVBoxLayout(DebugPage);
        verticalLayout_3->setSpacing(3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        widgetTop = new QWidget(DebugPage);
        widgetTop->setObjectName(QString::fromUtf8("widgetTop"));
        widgetTop->setMinimumSize(QSize(80, 0));
        horizontalLayout = new QHBoxLayout(widgetTop);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 12, 0);
        btnSetPressure = new QToolButton(widgetTop);
        btnSetPressure->setObjectName(QString::fromUtf8("btnSetPressure"));

        horizontalLayout->addWidget(btnSetPressure);

        btnFirmwareConfig = new QPushButton(widgetTop);
        btnFirmwareConfig->setObjectName(QString::fromUtf8("btnFirmwareConfig"));

        horizontalLayout->addWidget(btnFirmwareConfig);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addWidget(widgetTop);

        widgetSettings = new QWidget(DebugPage);
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


        retranslateUi(DebugPage);

        QMetaObject::connectSlotsByName(DebugPage);
    } // setupUi

    void retranslateUi(QWidget *DebugPage)
    {
        DebugPage->setWindowTitle(QCoreApplication::translate("DebugPage", "Form", nullptr));
        btnSetPressure->setText(QCoreApplication::translate("DebugPage", "Set Pressure", nullptr));
        btnFirmwareConfig->setText(QCoreApplication::translate("DebugPage", "Firmware", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DebugPage: public Ui_DebugPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGPAGE_H
