/********************************************************************************
** Form generated from reading UI file 'homepage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "cranemapwidget.h"

QT_BEGIN_NAMESPACE

class Ui_HomePage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labDetailed;
    QWidget *deviceMonitorPlaceholder;
    QWidget *overheadCranesWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_9;
    QLabel *labFoupInColor;
    QLabel *labFoupInText;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labPurgeTime5MinColor;
    QLabel *labPurgeTime5MinText;
    QSpacerItem *horizontalSpacer_4;
    QLabel *labPurgeTime10MinColor;
    QLabel *labPurgeTime10MinText;
    QSpacerItem *horizontalSpacer_5;
    QLabel *labPurgeTime20MinColor;
    QLabel *labPurgeTime20MinText;
    QSpacerItem *horizontalSpacer_6;
    QLabel *labPurgeTime30MinColor;
    QLabel *labPurgeTime30MinText;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labFoupOutColor;
    QLabel *labFoupOutText;
    QSpacerItem *horizontalSpacer_8;
    QLabel *labAlarmColor;
    QLabel *labAlarmText;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnFoupView;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *btnSetView;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnZoomIn;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *btnZoomOut;
    CraneMapWidget *widget;

    void setupUi(QWidget *HomePage)
    {
        if (HomePage->objectName().isEmpty())
            HomePage->setObjectName(QString::fromUtf8("HomePage"));
        HomePage->resize(831, 602);
        verticalLayout = new QVBoxLayout(HomePage);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        labDetailed = new QLabel(HomePage);
        labDetailed->setObjectName(QString::fromUtf8("labDetailed"));
        labDetailed->setMinimumSize(QSize(0, 30));
        labDetailed->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout->addWidget(labDetailed);

        deviceMonitorPlaceholder = new QWidget(HomePage);
        deviceMonitorPlaceholder->setObjectName(QString::fromUtf8("deviceMonitorPlaceholder"));

        verticalLayout->addWidget(deviceMonitorPlaceholder);

        overheadCranesWidget = new QWidget(HomePage);
        overheadCranesWidget->setObjectName(QString::fromUtf8("overheadCranesWidget"));
        overheadCranesWidget->setMinimumSize(QSize(0, 20));
        horizontalLayout = new QHBoxLayout(overheadCranesWidget);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        horizontalSpacer_9 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        labFoupInColor = new QLabel(overheadCranesWidget);
        labFoupInColor->setObjectName(QString::fromUtf8("labFoupInColor"));
        labFoupInColor->setMinimumSize(QSize(15, 15));
        labFoupInColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labFoupInColor);

        labFoupInText = new QLabel(overheadCranesWidget);
        labFoupInText->setObjectName(QString::fromUtf8("labFoupInText"));

        horizontalLayout->addWidget(labFoupInText);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        labPurgeTime5MinColor = new QLabel(overheadCranesWidget);
        labPurgeTime5MinColor->setObjectName(QString::fromUtf8("labPurgeTime5MinColor"));
        labPurgeTime5MinColor->setMinimumSize(QSize(15, 15));
        labPurgeTime5MinColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labPurgeTime5MinColor);

        labPurgeTime5MinText = new QLabel(overheadCranesWidget);
        labPurgeTime5MinText->setObjectName(QString::fromUtf8("labPurgeTime5MinText"));

        horizontalLayout->addWidget(labPurgeTime5MinText);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        labPurgeTime10MinColor = new QLabel(overheadCranesWidget);
        labPurgeTime10MinColor->setObjectName(QString::fromUtf8("labPurgeTime10MinColor"));
        labPurgeTime10MinColor->setMinimumSize(QSize(15, 15));
        labPurgeTime10MinColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labPurgeTime10MinColor);

        labPurgeTime10MinText = new QLabel(overheadCranesWidget);
        labPurgeTime10MinText->setObjectName(QString::fromUtf8("labPurgeTime10MinText"));

        horizontalLayout->addWidget(labPurgeTime10MinText);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        labPurgeTime20MinColor = new QLabel(overheadCranesWidget);
        labPurgeTime20MinColor->setObjectName(QString::fromUtf8("labPurgeTime20MinColor"));
        labPurgeTime20MinColor->setMinimumSize(QSize(15, 15));
        labPurgeTime20MinColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labPurgeTime20MinColor);

        labPurgeTime20MinText = new QLabel(overheadCranesWidget);
        labPurgeTime20MinText->setObjectName(QString::fromUtf8("labPurgeTime20MinText"));

        horizontalLayout->addWidget(labPurgeTime20MinText);

        horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        labPurgeTime30MinColor = new QLabel(overheadCranesWidget);
        labPurgeTime30MinColor->setObjectName(QString::fromUtf8("labPurgeTime30MinColor"));
        labPurgeTime30MinColor->setMinimumSize(QSize(15, 15));
        labPurgeTime30MinColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labPurgeTime30MinColor);

        labPurgeTime30MinText = new QLabel(overheadCranesWidget);
        labPurgeTime30MinText->setObjectName(QString::fromUtf8("labPurgeTime30MinText"));

        horizontalLayout->addWidget(labPurgeTime30MinText);

        horizontalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);

        labFoupOutColor = new QLabel(overheadCranesWidget);
        labFoupOutColor->setObjectName(QString::fromUtf8("labFoupOutColor"));
        labFoupOutColor->setMinimumSize(QSize(15, 15));
        labFoupOutColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labFoupOutColor);

        labFoupOutText = new QLabel(overheadCranesWidget);
        labFoupOutText->setObjectName(QString::fromUtf8("labFoupOutText"));

        horizontalLayout->addWidget(labFoupOutText);

        horizontalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        labAlarmColor = new QLabel(overheadCranesWidget);
        labAlarmColor->setObjectName(QString::fromUtf8("labAlarmColor"));
        labAlarmColor->setMinimumSize(QSize(15, 15));
        labAlarmColor->setMaximumSize(QSize(15, 15));

        horizontalLayout->addWidget(labAlarmColor);

        labAlarmText = new QLabel(overheadCranesWidget);
        labAlarmText->setObjectName(QString::fromUtf8("labAlarmText"));

        horizontalLayout->addWidget(labAlarmText);

        horizontalSpacer = new QSpacerItem(452, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnFoupView = new QPushButton(overheadCranesWidget);
        btnFoupView->setObjectName(QString::fromUtf8("btnFoupView"));
        btnFoupView->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(btnFoupView);

        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_10);

        btnSetView = new QPushButton(overheadCranesWidget);
        btnSetView->setObjectName(QString::fromUtf8("btnSetView"));
        btnSetView->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(btnSetView);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnZoomIn = new QPushButton(overheadCranesWidget);
        btnZoomIn->setObjectName(QString::fromUtf8("btnZoomIn"));
        btnZoomIn->setMinimumSize(QSize(30, 30));
        btnZoomIn->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(btnZoomIn);

        horizontalSpacer_11 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        btnZoomOut = new QPushButton(overheadCranesWidget);
        btnZoomOut->setObjectName(QString::fromUtf8("btnZoomOut"));
        btnZoomOut->setMinimumSize(QSize(30, 30));
        btnZoomOut->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(btnZoomOut);


        verticalLayout->addWidget(overheadCranesWidget);

        widget = new CraneMapWidget(HomePage);
        widget->setObjectName(QString::fromUtf8("widget"));

        verticalLayout->addWidget(widget);

        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 9);

        retranslateUi(HomePage);

        QMetaObject::connectSlotsByName(HomePage);
    } // setupUi

    void retranslateUi(QWidget *HomePage)
    {
        HomePage->setWindowTitle(QCoreApplication::translate("HomePage", "Form", nullptr));
        labDetailed->setText(QCoreApplication::translate("HomePage", "detailed information", nullptr));
        labFoupInText->setText(QCoreApplication::translate("HomePage", "< 5 Min", nullptr));
        labPurgeTime5MinText->setText(QCoreApplication::translate("HomePage", ">= 5 Min", nullptr));
        labPurgeTime10MinText->setText(QCoreApplication::translate("HomePage", ">= 10 Min", nullptr));
        labPurgeTime20MinText->setText(QCoreApplication::translate("HomePage", ">= 20 Min", nullptr));
        labPurgeTime30MinText->setText(QCoreApplication::translate("HomePage", ">= 30 Min", nullptr));
        labFoupOutText->setText(QCoreApplication::translate("HomePage", "FoupOut", nullptr));
        labAlarmText->setText(QCoreApplication::translate("HomePage", "Alarm", nullptr));
        btnFoupView->setText(QCoreApplication::translate("HomePage", "Foup View", nullptr));
        btnSetView->setText(QCoreApplication::translate("HomePage", "Set View", nullptr));
        btnZoomIn->setText(QCoreApplication::translate("HomePage", "+", nullptr));
        btnZoomOut->setText(QCoreApplication::translate("HomePage", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomePage: public Ui_HomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
