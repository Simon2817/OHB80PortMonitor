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
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AlarmPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWidget *AlarmPage)
    {
        if (AlarmPage->objectName().isEmpty())
            AlarmPage->setObjectName(QString::fromUtf8("AlarmPage"));
        AlarmPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(AlarmPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(AlarmPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        retranslateUi(AlarmPage);

        QMetaObject::connectSlotsByName(AlarmPage);
    } // setupUi

    void retranslateUi(QWidget *AlarmPage)
    {
        AlarmPage->setWindowTitle(QCoreApplication::translate("AlarmPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("AlarmPage", "Alarm Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlarmPage: public Ui_AlarmPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALARMPAGE_H
