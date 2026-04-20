/********************************************************************************
** Form generated from reading UI file 'firmwareupdatewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRMWAREUPDATEWIDGET_H
#define UI_FIRMWAREUPDATEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "LoggerWidget.h"

QT_BEGIN_NAMESPACE

class Ui_FirmwareUpdateWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabUpdate;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *labQRCode;
    QLineEdit *lineEditQRCode;
    QPushButton *btnAddDevice;
    QPushButton *btnAddAllDevices;
    QPushButton *btnUdateSelected;
    QPushButton *btnClear;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QLabel *labDToBeUpdate;
    QTableWidget *tableWidgetSelectedDevices;
    QSpacerItem *verticalSpacer;
    LoggerWidget *tabLogger;

    void setupUi(QWidget *FirmwareUpdateWidget)
    {
        if (FirmwareUpdateWidget->objectName().isEmpty())
            FirmwareUpdateWidget->setObjectName(QString::fromUtf8("FirmwareUpdateWidget"));
        FirmwareUpdateWidget->resize(608, 351);
        verticalLayout = new QVBoxLayout(FirmwareUpdateWidget);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        tabWidget = new QTabWidget(FirmwareUpdateWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabUpdate = new QWidget();
        tabUpdate->setObjectName(QString::fromUtf8("tabUpdate"));
        verticalLayout_2 = new QVBoxLayout(tabUpdate);
        verticalLayout_2->setSpacing(3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(tabUpdate);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(3, 3, 3, 3);
        labQRCode = new QLabel(widget);
        labQRCode->setObjectName(QString::fromUtf8("labQRCode"));

        horizontalLayout->addWidget(labQRCode);

        lineEditQRCode = new QLineEdit(widget);
        lineEditQRCode->setObjectName(QString::fromUtf8("lineEditQRCode"));

        horizontalLayout->addWidget(lineEditQRCode);

        btnAddDevice = new QPushButton(widget);
        btnAddDevice->setObjectName(QString::fromUtf8("btnAddDevice"));

        horizontalLayout->addWidget(btnAddDevice);

        btnAddAllDevices = new QPushButton(widget);
        btnAddAllDevices->setObjectName(QString::fromUtf8("btnAddAllDevices"));

        horizontalLayout->addWidget(btnAddAllDevices);

        btnUdateSelected = new QPushButton(widget);
        btnUdateSelected->setObjectName(QString::fromUtf8("btnUdateSelected"));

        horizontalLayout->addWidget(btnUdateSelected);

        btnClear = new QPushButton(widget);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));

        horizontalLayout->addWidget(btnClear);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(widget);

        progressBar = new QProgressBar(tabUpdate);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);

        labDToBeUpdate = new QLabel(tabUpdate);
        labDToBeUpdate->setObjectName(QString::fromUtf8("labDToBeUpdate"));

        verticalLayout_2->addWidget(labDToBeUpdate);

        tableWidgetSelectedDevices = new QTableWidget(tabUpdate);
        tableWidgetSelectedDevices->setObjectName(QString::fromUtf8("tableWidgetSelectedDevices"));
        tableWidgetSelectedDevices->setMinimumSize(QSize(600, 100));

        verticalLayout_2->addWidget(tableWidgetSelectedDevices);

        verticalSpacer = new QSpacerItem(20, 45, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(tabUpdate, QString());
        tabLogger = new LoggerWidget();
        tabLogger->setObjectName(QString::fromUtf8("tabLogger"));
        tabWidget->addTab(tabLogger, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(FirmwareUpdateWidget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FirmwareUpdateWidget);
    } // setupUi

    void retranslateUi(QWidget *FirmwareUpdateWidget)
    {
        FirmwareUpdateWidget->setWindowTitle(QCoreApplication::translate("FirmwareUpdateWidget", "Form", nullptr));
        labQRCode->setText(QCoreApplication::translate("FirmwareUpdateWidget", "QRCode :   ", nullptr));
        btnAddDevice->setText(QCoreApplication::translate("FirmwareUpdateWidget", "Add Device", nullptr));
        btnAddAllDevices->setText(QCoreApplication::translate("FirmwareUpdateWidget", "Add All Devices", nullptr));
        btnUdateSelected->setText(QCoreApplication::translate("FirmwareUpdateWidget", "Update", nullptr));
        btnClear->setText(QCoreApplication::translate("FirmwareUpdateWidget", "Clear", nullptr));
        labDToBeUpdate->setText(QCoreApplication::translate("FirmwareUpdateWidget", "Devices to be upgraded", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabUpdate), QCoreApplication::translate("FirmwareUpdateWidget", "Update", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLogger), QCoreApplication::translate("FirmwareUpdateWidget", "Logger", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FirmwareUpdateWidget: public Ui_FirmwareUpdateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRMWAREUPDATEWIDGET_H
