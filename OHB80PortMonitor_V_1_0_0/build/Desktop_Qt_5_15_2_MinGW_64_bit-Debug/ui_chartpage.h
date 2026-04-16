/********************************************************************************
** Form generated from reading UI file 'chartpage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTPAGE_H
#define UI_CHARTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;

    void setupUi(QWidget *ChartPage)
    {
        if (ChartPage->objectName().isEmpty())
            ChartPage->setObjectName(QString::fromUtf8("ChartPage"));
        ChartPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(ChartPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ChartPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        retranslateUi(ChartPage);

        QMetaObject::connectSlotsByName(ChartPage);
    } // setupUi

    void retranslateUi(QWidget *ChartPage)
    {
        ChartPage->setWindowTitle(QCoreApplication::translate("ChartPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("ChartPage", "Chart Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartPage: public Ui_ChartPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTPAGE_H
