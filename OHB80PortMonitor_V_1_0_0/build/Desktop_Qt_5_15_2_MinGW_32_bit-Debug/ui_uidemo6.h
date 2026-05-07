/********************************************************************************
** Form generated from reading UI file 'uidemo6.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UIDEMO6_H
#define UI_UIDEMO6_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "alarmpage.h"
#include "chartpage.h"
#include "communicatepage.h"
#include "configpage.h"
#include "debugpage.h"
#include "homepage.h"
#include "scrollingtiplabel.h"
#include "useraccountlabel.h"

QT_BEGIN_NAMESPACE

class Ui_UIDemo6
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widgetTitle;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labIco;
    QLabel *labTitle;
    ScrollingTipLabel *scrollingTipLabel;
    QSpacerItem *horizontalSpacer;
    UserAccountLabel *useraccountLab;
    QWidget *widgetMenu;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnMenu;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Max;
    QPushButton *btnMenu_Close;
    QWidget *widgetMain;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widgetLeft;
    QVBoxLayout *verticalLayout_2;
    QToolButton *btnHome;
    QToolButton *btnSetting;
    QToolButton *btnChart;
    QToolButton *btnCommunicate;
    QToolButton *btnAlarm;
    QToolButton *btnDebug;
    QSpacerItem *verticalSpacer;
    QWidget *widgetRight;
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    HomePage *homepage;
    ConfigPage *configpage;
    ChartPage *chartpage;
    CommunicatePage *communicatepage;
    AlarmPage *alarmpage;
    DebugPage *debugpage;

    void setupUi(QDialog *UIDemo6)
    {
        if (UIDemo6->objectName().isEmpty())
            UIDemo6->setObjectName(QString::fromUtf8("UIDemo6"));
        UIDemo6->resize(1200, 900);
        UIDemo6->setMinimumSize(QSize(0, 0));
        UIDemo6->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(UIDemo6);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widgetTitle = new QWidget(UIDemo6);
        widgetTitle->setObjectName(QString::fromUtf8("widgetTitle"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetTitle->sizePolicy().hasHeightForWidth());
        widgetTitle->setSizePolicy(sizePolicy);
        widgetTitle->setMinimumSize(QSize(0, 40));
        widgetTitle->setStyleSheet(QString::fromUtf8("QWidget#widgetTitle{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:1 #0973AB,stop:0 #FFFFFF);\n"
"}\n"
"QWidget#widgetTitle>QLabel{\n"
"color:#ABD6FF;\n"
"}\n"
"QLabel#labTitle{\n"
"font:14px;\n"
"}\n"
"QLabel#labInfo{\n"
"font:12px;\n"
"}\n"
"QFrame#line{\n"
"color:#ABD6FF;\n"
"}\n"
"QToolButton::menu-indicator{\n"
"image:None;\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(widgetTitle);
        horizontalLayout_2->setSpacing(9);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(6, 0, 0, 0);
        labIco = new QLabel(widgetTitle);
        labIco->setObjectName(QString::fromUtf8("labIco"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labIco->sizePolicy().hasHeightForWidth());
        labIco->setSizePolicy(sizePolicy1);
        labIco->setMinimumSize(QSize(0, 0));
        labIco->setStyleSheet(QString::fromUtf8(""));
        labIco->setPixmap(QPixmap(QString::fromUtf8(":/image/logo.jpg")));
        labIco->setScaledContents(false);
        labIco->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(labIco);

        labTitle = new QLabel(widgetTitle);
        labTitle->setObjectName(QString::fromUtf8("labTitle"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labTitle->sizePolicy().hasHeightForWidth());
        labTitle->setSizePolicy(sizePolicy2);
        labTitle->setStyleSheet(QString::fromUtf8("color:black;"));
        labTitle->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(labTitle);

        scrollingTipLabel = new ScrollingTipLabel(widgetTitle);
        scrollingTipLabel->setObjectName(QString::fromUtf8("scrollingTipLabel"));
        scrollingTipLabel->setMinimumSize(QSize(500, 0));
        scrollingTipLabel->setMaximumSize(QSize(500, 35));

        horizontalLayout_2->addWidget(scrollingTipLabel);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        useraccountLab = new UserAccountLabel(widgetTitle);
        useraccountLab->setObjectName(QString::fromUtf8("useraccountLab"));

        horizontalLayout_2->addWidget(useraccountLab);

        widgetMenu = new QWidget(widgetTitle);
        widgetMenu->setObjectName(QString::fromUtf8("widgetMenu"));
        sizePolicy1.setHeightForWidth(widgetMenu->sizePolicy().hasHeightForWidth());
        widgetMenu->setSizePolicy(sizePolicy1);
        widgetMenu->setStyleSheet(QString::fromUtf8("QToolButton,QPushButton{\n"
"border:0px solid #FF0000;\n"
"}\n"
"QToolButton:hover,QPushButton:hover,QToolButton:pressed,QPushButton:pressed{\n"
"background:#067DB8;\n"
"}"));
        horizontalLayout = new QHBoxLayout(widgetMenu);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnMenu = new QToolButton(widgetMenu);
        btnMenu->setObjectName(QString::fromUtf8("btnMenu"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
        btnMenu->setSizePolicy(sizePolicy3);
        btnMenu->setMinimumSize(QSize(35, 0));
        btnMenu->setMaximumSize(QSize(35, 16777215));
        btnMenu->setFocusPolicy(Qt::NoFocus);
        btnMenu->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/titlemenu.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMenu->setIcon(icon);
        btnMenu->setAutoRepeat(false);
        btnMenu->setPopupMode(QToolButton::InstantPopup);

        horizontalLayout->addWidget(btnMenu);

        btnMenu_Min = new QPushButton(widgetMenu);
        btnMenu_Min->setObjectName(QString::fromUtf8("btnMenu_Min"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy4);
        btnMenu_Min->setMinimumSize(QSize(35, 0));
        btnMenu_Min->setMaximumSize(QSize(35, 16777215));
        btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Min->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/titlemini.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMenu_Min->setIcon(icon1);
        btnMenu_Min->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Min);

        btnMenu_Max = new QPushButton(widgetMenu);
        btnMenu_Max->setObjectName(QString::fromUtf8("btnMenu_Max"));
        sizePolicy3.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy3);
        btnMenu_Max->setMinimumSize(QSize(35, 0));
        btnMenu_Max->setMaximumSize(QSize(35, 16777215));
        btnMenu_Max->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/titlemax.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMenu_Max->setIcon(icon2);
        btnMenu_Max->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Max);

        btnMenu_Close = new QPushButton(widgetMenu);
        btnMenu_Close->setObjectName(QString::fromUtf8("btnMenu_Close"));
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy3);
        btnMenu_Close->setMinimumSize(QSize(35, 0));
        btnMenu_Close->setMaximumSize(QSize(35, 16777215));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/image/titleclose.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMenu_Close->setIcon(icon3);
        btnMenu_Close->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Close);


        horizontalLayout_2->addWidget(widgetMenu);


        verticalLayout->addWidget(widgetTitle);

        widgetMain = new QWidget(UIDemo6);
        widgetMain->setObjectName(QString::fromUtf8("widgetMain"));
        widgetMain->setStyleSheet(QString::fromUtf8("QWidget#widgetMain{\n"
"border-image: url(:/image/bj.png);\n"
"}"));
        horizontalLayout_3 = new QHBoxLayout(widgetMain);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        widgetLeft = new QWidget(widgetMain);
        widgetLeft->setObjectName(QString::fromUtf8("widgetLeft"));
        widgetLeft->setMinimumSize(QSize(100, 0));
        widgetLeft->setMaximumSize(QSize(100, 16777215));
        widgetLeft->setStyleSheet(QString::fromUtf8("QWidget#widgetLeft{\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 rgba(1,86,155,100),stop:1 rgba(1,104,183,100));\n"
"}\n"
"QToolButton{\n"
"font:13px;\n"
"color:#ABD6FF;\n"
"border-style:solid;\n"
"border-width:0px 0px 1px 0px;\n"
"border-color:rgba(171,214,255,30);\n"
"padding:8px;\n"
"}\n"
"QToolButton:hover,QToolButton:presse{\n"
"background:#067DB8;\n"
"}\n"
"QToolButton:checked{\n"
"color:#FFFFFF;\n"
"border-style:solid;\n"
"border-width:0px 4px 0px 0px;\n"
"border-color:#00B7DE;\n"
"padding:8px 4px 8px 8px;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #00326B,stop:1 #00346D);\n"
"}\n"
""));
        verticalLayout_2 = new QVBoxLayout(widgetLeft);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        btnHome = new QToolButton(widgetLeft);
        btnHome->setObjectName(QString::fromUtf8("btnHome"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(btnHome->sizePolicy().hasHeightForWidth());
        btnHome->setSizePolicy(sizePolicy5);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/image/homepage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnHome->setIcon(icon4);
        btnHome->setIconSize(QSize(50, 50));
        btnHome->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnHome);

        btnSetting = new QToolButton(widgetLeft);
        btnSetting->setObjectName(QString::fromUtf8("btnSetting"));
        sizePolicy5.setHeightForWidth(btnSetting->sizePolicy().hasHeightForWidth());
        btnSetting->setSizePolicy(sizePolicy5);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/image/configpage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSetting->setIcon(icon5);
        btnSetting->setIconSize(QSize(50, 50));
        btnSetting->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnSetting);

        btnChart = new QToolButton(widgetLeft);
        btnChart->setObjectName(QString::fromUtf8("btnChart"));
        sizePolicy5.setHeightForWidth(btnChart->sizePolicy().hasHeightForWidth());
        btnChart->setSizePolicy(sizePolicy5);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/image/chartpage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnChart->setIcon(icon6);
        btnChart->setIconSize(QSize(50, 50));
        btnChart->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnChart);

        btnCommunicate = new QToolButton(widgetLeft);
        btnCommunicate->setObjectName(QString::fromUtf8("btnCommunicate"));
        sizePolicy5.setHeightForWidth(btnCommunicate->sizePolicy().hasHeightForWidth());
        btnCommunicate->setSizePolicy(sizePolicy5);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/image/communicatepage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCommunicate->setIcon(icon7);
        btnCommunicate->setIconSize(QSize(50, 50));
        btnCommunicate->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnCommunicate);

        btnAlarm = new QToolButton(widgetLeft);
        btnAlarm->setObjectName(QString::fromUtf8("btnAlarm"));
        sizePolicy5.setHeightForWidth(btnAlarm->sizePolicy().hasHeightForWidth());
        btnAlarm->setSizePolicy(sizePolicy5);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/image/alarmpage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAlarm->setIcon(icon8);
        btnAlarm->setIconSize(QSize(50, 50));
        btnAlarm->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnAlarm);

        btnDebug = new QToolButton(widgetLeft);
        btnDebug->setObjectName(QString::fromUtf8("btnDebug"));
        sizePolicy5.setHeightForWidth(btnDebug->sizePolicy().hasHeightForWidth());
        btnDebug->setSizePolicy(sizePolicy5);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/image/debugpage.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDebug->setIcon(icon9);
        btnDebug->setIconSize(QSize(50, 50));
        btnDebug->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_2->addWidget(btnDebug);

        verticalSpacer = new QSpacerItem(20, 53, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_3->addWidget(widgetLeft);

        widgetRight = new QWidget(widgetMain);
        widgetRight->setObjectName(QString::fromUtf8("widgetRight"));
        verticalLayout_3 = new QVBoxLayout(widgetRight);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(widgetRight);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        homepage = new HomePage();
        homepage->setObjectName(QString::fromUtf8("homepage"));
        homepage->setStyleSheet(QString::fromUtf8(""));
        stackedWidget->addWidget(homepage);
        configpage = new ConfigPage();
        configpage->setObjectName(QString::fromUtf8("configpage"));
        configpage->setStyleSheet(QString::fromUtf8(""));
        stackedWidget->addWidget(configpage);
        chartpage = new ChartPage();
        chartpage->setObjectName(QString::fromUtf8("chartpage"));
        stackedWidget->addWidget(chartpage);
        communicatepage = new CommunicatePage();
        communicatepage->setObjectName(QString::fromUtf8("communicatepage"));
        stackedWidget->addWidget(communicatepage);
        alarmpage = new AlarmPage();
        alarmpage->setObjectName(QString::fromUtf8("alarmpage"));
        stackedWidget->addWidget(alarmpage);
        debugpage = new DebugPage();
        debugpage->setObjectName(QString::fromUtf8("debugpage"));
        stackedWidget->addWidget(debugpage);

        verticalLayout_3->addWidget(stackedWidget);


        horizontalLayout_3->addWidget(widgetRight);


        verticalLayout->addWidget(widgetMain);


        retranslateUi(UIDemo6);

        stackedWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(UIDemo6);
    } // setupUi

    void retranslateUi(QDialog *UIDemo6)
    {
        UIDemo6->setWindowTitle(QCoreApplication::translate("UIDemo6", "Dialog", nullptr));
        labIco->setText(QString());
        labTitle->setText(QString());
        scrollingTipLabel->setText(QString());
        btnMenu->setText(QString());
#if QT_CONFIG(tooltip)
        btnMenu_Min->setToolTip(QCoreApplication::translate("UIDemo6", "\346\234\200\345\260\217\345\214\226", nullptr));
#endif // QT_CONFIG(tooltip)
        btnMenu_Min->setText(QString());
#if QT_CONFIG(tooltip)
        btnMenu_Max->setToolTip(QCoreApplication::translate("UIDemo6", "\346\234\200\345\244\247\345\214\226", nullptr));
#endif // QT_CONFIG(tooltip)
        btnMenu_Max->setText(QString());
#if QT_CONFIG(tooltip)
        btnMenu_Close->setToolTip(QCoreApplication::translate("UIDemo6", "\345\205\263\351\227\255", nullptr));
#endif // QT_CONFIG(tooltip)
        btnMenu_Close->setText(QString());
        btnHome->setText(QCoreApplication::translate("UIDemo6", "Home", nullptr));
        btnSetting->setText(QCoreApplication::translate("UIDemo6", "Config", nullptr));
        btnChart->setText(QCoreApplication::translate("UIDemo6", "Chart", nullptr));
        btnCommunicate->setText(QCoreApplication::translate("UIDemo6", " Comm. ", nullptr));
        btnAlarm->setText(QCoreApplication::translate("UIDemo6", "Alarm", nullptr));
        btnDebug->setText(QCoreApplication::translate("UIDemo6", "Debug", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UIDemo6: public Ui_UIDemo6 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UIDEMO6_H
