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
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DebugPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWidget *DebugPage)
    {
        if (DebugPage->objectName().isEmpty())
            DebugPage->setObjectName(QString::fromUtf8("DebugPage"));
        DebugPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(DebugPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DebugPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        retranslateUi(DebugPage);

        QMetaObject::connectSlotsByName(DebugPage);
    } // setupUi

    void retranslateUi(QWidget *DebugPage)
    {
        DebugPage->setWindowTitle(QCoreApplication::translate("DebugPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("DebugPage", "Debug Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DebugPage: public Ui_DebugPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGPAGE_H
