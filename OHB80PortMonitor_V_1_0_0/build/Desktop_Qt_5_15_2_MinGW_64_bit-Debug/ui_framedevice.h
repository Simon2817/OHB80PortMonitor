/********************************************************************************
** Form generated from reading UI file 'framedevice.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMEDEVICE_H
#define UI_FRAMEDEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FrameDevice
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *gboxDeviceTitle;
    QGridLayout *gridLayout_2;
    QLabel *labType;
    QFrame *gboxDevicePanel;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout;
    QLabel *labInletPressureValue;
    QLabel *labRH;
    QLabel *labRHValue;
    QLabel *labInletFlow;
    QLabel *labInletFlowValue;
    QLabel *labInletPressure;
    QLabel *labID;
    QLabel *labIDValue;

    void setupUi(QWidget *FrameDevice)
    {
        if (FrameDevice->objectName().isEmpty())
            FrameDevice->setObjectName(QString::fromUtf8("FrameDevice"));
        FrameDevice->resize(106, 85);
        verticalLayout = new QVBoxLayout(FrameDevice);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        gboxDeviceTitle = new QFrame(FrameDevice);
        gboxDeviceTitle->setObjectName(QString::fromUtf8("gboxDeviceTitle"));
        gboxDeviceTitle->setFrameShape(QFrame::StyledPanel);
        gboxDeviceTitle->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(gboxDeviceTitle);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(1, 0, 0, 0);
        labType = new QLabel(gboxDeviceTitle);
        labType->setObjectName(QString::fromUtf8("labType"));
        labType->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(labType, 0, 0, 1, 1);


        verticalLayout->addWidget(gboxDeviceTitle);

        gboxDevicePanel = new QFrame(FrameDevice);
        gboxDevicePanel->setObjectName(QString::fromUtf8("gboxDevicePanel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gboxDevicePanel->sizePolicy().hasHeightForWidth());
        gboxDevicePanel->setSizePolicy(sizePolicy);
        gboxDevicePanel->setMinimumSize(QSize(0, 0));
        gridLayout_6 = new QGridLayout(gboxDevicePanel);
        gridLayout_6->setSpacing(0);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labInletPressureValue = new QLabel(gboxDevicePanel);
        labInletPressureValue->setObjectName(QString::fromUtf8("labInletPressureValue"));
        sizePolicy.setHeightForWidth(labInletPressureValue->sizePolicy().hasHeightForWidth());
        labInletPressureValue->setSizePolicy(sizePolicy);

        gridLayout->addWidget(labInletPressureValue, 1, 1, 1, 1);

        labRH = new QLabel(gboxDevicePanel);
        labRH->setObjectName(QString::fromUtf8("labRH"));

        gridLayout->addWidget(labRH, 3, 0, 1, 1);

        labRHValue = new QLabel(gboxDevicePanel);
        labRHValue->setObjectName(QString::fromUtf8("labRHValue"));

        gridLayout->addWidget(labRHValue, 3, 1, 1, 1);

        labInletFlow = new QLabel(gboxDevicePanel);
        labInletFlow->setObjectName(QString::fromUtf8("labInletFlow"));
        labInletFlow->setMaximumSize(QSize(20, 16777215));
        labInletFlow->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labInletFlow, 2, 0, 1, 1);

        labInletFlowValue = new QLabel(gboxDevicePanel);
        labInletFlowValue->setObjectName(QString::fromUtf8("labInletFlowValue"));
        sizePolicy.setHeightForWidth(labInletFlowValue->sizePolicy().hasHeightForWidth());
        labInletFlowValue->setSizePolicy(sizePolicy);

        gridLayout->addWidget(labInletFlowValue, 2, 1, 1, 1);

        labInletPressure = new QLabel(gboxDevicePanel);
        labInletPressure->setObjectName(QString::fromUtf8("labInletPressure"));
        labInletPressure->setMaximumSize(QSize(20, 16777215));
        labInletPressure->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labInletPressure, 1, 0, 1, 1);

        labID = new QLabel(gboxDevicePanel);
        labID->setObjectName(QString::fromUtf8("labID"));

        gridLayout->addWidget(labID, 0, 0, 1, 1);

        labIDValue = new QLabel(gboxDevicePanel);
        labIDValue->setObjectName(QString::fromUtf8("labIDValue"));

        gridLayout->addWidget(labIDValue, 0, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout, 0, 0, 1, 2);


        verticalLayout->addWidget(gboxDevicePanel);


        retranslateUi(FrameDevice);

        QMetaObject::connectSlotsByName(FrameDevice);
    } // setupUi

    void retranslateUi(QWidget *FrameDevice)
    {
        FrameDevice->setWindowTitle(QCoreApplication::translate("FrameDevice", "Form", nullptr));
        labType->setText(QCoreApplication::translate("FrameDevice", "Set", nullptr));
        labInletPressureValue->setText(QCoreApplication::translate("FrameDevice", "4.23~4.12bar", nullptr));
        labRH->setText(QCoreApplication::translate("FrameDevice", "RH: ", nullptr));
        labRHValue->setText(QCoreApplication::translate("FrameDevice", "8.12~7.56%", nullptr));
        labInletFlow->setText(QCoreApplication::translate("FrameDevice", "F  : ", nullptr));
        labInletFlowValue->setText(QCoreApplication::translate("FrameDevice", "10.23L/Min", nullptr));
        labInletPressure->setText(QCoreApplication::translate("FrameDevice", "P  : ", nullptr));
        labID->setText(QCoreApplication::translate("FrameDevice", "ID: ", nullptr));
        labIDValue->setText(QCoreApplication::translate("FrameDevice", "12034~12038", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FrameDevice: public Ui_FrameDevice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMEDEVICE_H
