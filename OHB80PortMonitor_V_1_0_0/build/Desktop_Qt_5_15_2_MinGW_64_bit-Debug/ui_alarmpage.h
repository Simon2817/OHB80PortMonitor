/********************************************************************************
** Form generated from reading UI file 'alarmpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALARMPAGE_H
#define UI_ALARMPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "alarmloggerwidget.h"

QT_BEGIN_NAMESPACE

class Ui_AlarmPage
{
public:
    QVBoxLayout *verticalLayout;
    AlarmLoggerWidget *alarmLoggerWidget;

    void setupUi(QWidget *AlarmPage)
    {
        if (AlarmPage->objectName().isEmpty())
            AlarmPage->setObjectName(QString::fromUtf8("AlarmPage"));
        AlarmPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(AlarmPage);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        alarmLoggerWidget = new AlarmLoggerWidget(AlarmPage);
        alarmLoggerWidget->setObjectName(QString::fromUtf8("alarmLoggerWidget"));

        verticalLayout->addWidget(alarmLoggerWidget);


        retranslateUi(AlarmPage);

        QMetaObject::connectSlotsByName(AlarmPage);
    } // setupUi

    void retranslateUi(QWidget *AlarmPage)
    {
        AlarmPage->setWindowTitle(QCoreApplication::translate("AlarmPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlarmPage: public Ui_AlarmPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALARMPAGE_H
