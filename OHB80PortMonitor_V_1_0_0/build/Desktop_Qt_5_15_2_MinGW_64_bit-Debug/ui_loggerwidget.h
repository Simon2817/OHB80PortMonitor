/********************************************************************************
** Form generated from reading UI file 'loggerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGGERWIDGET_H
#define UI_LOGGERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoggerWidget
{
public:
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;
    QWidget *liveTab;
    QVBoxLayout *liveLayout;
    QListView *liveListView;
    QWidget *historyTab;
    QVBoxLayout *histLayout;
    QHBoxLayout *dateLayout;
    QLabel *labelDate;
    QLineEdit *dateEdit;
    QPushButton *selectDateBtn;
    QSpacerItem *dateSpacer;
    QHBoxLayout *timeLayout;
    QCheckBox *timeCheck;
    QSpinBox *timeFromHour;
    QLabel *labelTimeFrom;
    QSpinBox *timeFromMinute;
    QLabel *labelTimeFrom2;
    QSpinBox *timeFromSecond;
    QLabel *labelTimeTo;
    QSpinBox *timeToHour;
    QLabel *labelTimeTo2;
    QSpinBox *timeToMinute;
    QLabel *labelTimeTo3;
    QSpinBox *timeToSecond;
    QSpacerItem *timeSpacer;
    QHBoxLayout *searchLayout;
    QLabel *labelLike;
    QLineEdit *likeEdit;
    QPushButton *searchBtn;
    QPushButton *findPrevBtn;
    QPushButton *findNextBtn;
    QListView *histListView;
    QWidget *pageBarWidget;
    QHBoxLayout *pageBarLayout;

    void setupUi(QWidget *LoggerWidget)
    {
        if (LoggerWidget->objectName().isEmpty())
            LoggerWidget->setObjectName(QString::fromUtf8("LoggerWidget"));
        LoggerWidget->resize(800, 600);
        mainLayout = new QVBoxLayout(LoggerWidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(LoggerWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        liveTab = new QWidget();
        liveTab->setObjectName(QString::fromUtf8("liveTab"));
        liveLayout = new QVBoxLayout(liveTab);
        liveLayout->setSpacing(0);
        liveLayout->setObjectName(QString::fromUtf8("liveLayout"));
        liveLayout->setContentsMargins(0, 0, 0, 0);
        liveListView = new QListView(liveTab);
        liveListView->setObjectName(QString::fromUtf8("liveListView"));
        liveListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        liveListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        liveListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        liveLayout->addWidget(liveListView);

        tabWidget->addTab(liveTab, QString());
        historyTab = new QWidget();
        historyTab->setObjectName(QString::fromUtf8("historyTab"));
        histLayout = new QVBoxLayout(historyTab);
        histLayout->setSpacing(4);
        histLayout->setObjectName(QString::fromUtf8("histLayout"));
        histLayout->setContentsMargins(4, 4, 4, 4);
        dateLayout = new QHBoxLayout();
        dateLayout->setObjectName(QString::fromUtf8("dateLayout"));
        labelDate = new QLabel(historyTab);
        labelDate->setObjectName(QString::fromUtf8("labelDate"));

        dateLayout->addWidget(labelDate);

        dateEdit = new QLineEdit(historyTab);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setReadOnly(true);

        dateLayout->addWidget(dateEdit);

        selectDateBtn = new QPushButton(historyTab);
        selectDateBtn->setObjectName(QString::fromUtf8("selectDateBtn"));
        selectDateBtn->setCursor(QCursor(Qt::PointingHandCursor));

        dateLayout->addWidget(selectDateBtn);

        dateSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        dateLayout->addItem(dateSpacer);


        histLayout->addLayout(dateLayout);

        timeLayout = new QHBoxLayout();
        timeLayout->setObjectName(QString::fromUtf8("timeLayout"));
        timeCheck = new QCheckBox(historyTab);
        timeCheck->setObjectName(QString::fromUtf8("timeCheck"));

        timeLayout->addWidget(timeCheck);

        timeFromHour = new QSpinBox(historyTab);
        timeFromHour->setObjectName(QString::fromUtf8("timeFromHour"));
        timeFromHour->setEnabled(false);
        timeFromHour->setMinimum(0);
        timeFromHour->setMaximum(23);

        timeLayout->addWidget(timeFromHour);

        labelTimeFrom = new QLabel(historyTab);
        labelTimeFrom->setObjectName(QString::fromUtf8("labelTimeFrom"));

        timeLayout->addWidget(labelTimeFrom);

        timeFromMinute = new QSpinBox(historyTab);
        timeFromMinute->setObjectName(QString::fromUtf8("timeFromMinute"));
        timeFromMinute->setEnabled(false);
        timeFromMinute->setMinimum(0);
        timeFromMinute->setMaximum(59);

        timeLayout->addWidget(timeFromMinute);

        labelTimeFrom2 = new QLabel(historyTab);
        labelTimeFrom2->setObjectName(QString::fromUtf8("labelTimeFrom2"));

        timeLayout->addWidget(labelTimeFrom2);

        timeFromSecond = new QSpinBox(historyTab);
        timeFromSecond->setObjectName(QString::fromUtf8("timeFromSecond"));
        timeFromSecond->setEnabled(false);
        timeFromSecond->setMinimum(0);
        timeFromSecond->setMaximum(59);

        timeLayout->addWidget(timeFromSecond);

        labelTimeTo = new QLabel(historyTab);
        labelTimeTo->setObjectName(QString::fromUtf8("labelTimeTo"));

        timeLayout->addWidget(labelTimeTo);

        timeToHour = new QSpinBox(historyTab);
        timeToHour->setObjectName(QString::fromUtf8("timeToHour"));
        timeToHour->setEnabled(false);
        timeToHour->setMinimum(0);
        timeToHour->setMaximum(23);

        timeLayout->addWidget(timeToHour);

        labelTimeTo2 = new QLabel(historyTab);
        labelTimeTo2->setObjectName(QString::fromUtf8("labelTimeTo2"));

        timeLayout->addWidget(labelTimeTo2);

        timeToMinute = new QSpinBox(historyTab);
        timeToMinute->setObjectName(QString::fromUtf8("timeToMinute"));
        timeToMinute->setEnabled(false);
        timeToMinute->setMinimum(0);
        timeToMinute->setMaximum(59);

        timeLayout->addWidget(timeToMinute);

        labelTimeTo3 = new QLabel(historyTab);
        labelTimeTo3->setObjectName(QString::fromUtf8("labelTimeTo3"));

        timeLayout->addWidget(labelTimeTo3);

        timeToSecond = new QSpinBox(historyTab);
        timeToSecond->setObjectName(QString::fromUtf8("timeToSecond"));
        timeToSecond->setEnabled(false);
        timeToSecond->setMinimum(0);
        timeToSecond->setMaximum(59);

        timeLayout->addWidget(timeToSecond);

        timeSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        timeLayout->addItem(timeSpacer);


        histLayout->addLayout(timeLayout);

        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName(QString::fromUtf8("searchLayout"));
        labelLike = new QLabel(historyTab);
        labelLike->setObjectName(QString::fromUtf8("labelLike"));

        searchLayout->addWidget(labelLike);

        likeEdit = new QLineEdit(historyTab);
        likeEdit->setObjectName(QString::fromUtf8("likeEdit"));

        searchLayout->addWidget(likeEdit);

        searchBtn = new QPushButton(historyTab);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));

        searchLayout->addWidget(searchBtn);

        findPrevBtn = new QPushButton(historyTab);
        findPrevBtn->setObjectName(QString::fromUtf8("findPrevBtn"));
        findPrevBtn->setEnabled(false);

        searchLayout->addWidget(findPrevBtn);

        findNextBtn = new QPushButton(historyTab);
        findNextBtn->setObjectName(QString::fromUtf8("findNextBtn"));
        findNextBtn->setEnabled(false);

        searchLayout->addWidget(findNextBtn);


        histLayout->addLayout(searchLayout);

        histListView = new QListView(historyTab);
        histListView->setObjectName(QString::fromUtf8("histListView"));
        histListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        histListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        histListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        histLayout->addWidget(histListView);

        pageBarWidget = new QWidget(historyTab);
        pageBarWidget->setObjectName(QString::fromUtf8("pageBarWidget"));
        pageBarLayout = new QHBoxLayout(pageBarWidget);
        pageBarLayout->setSpacing(2);
        pageBarLayout->setObjectName(QString::fromUtf8("pageBarLayout"));
        pageBarLayout->setContentsMargins(0, 0, 0, 0);

        histLayout->addWidget(pageBarWidget);

        tabWidget->addTab(historyTab, QString());

        mainLayout->addWidget(tabWidget);


        retranslateUi(LoggerWidget);
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeFromHour, SLOT(setEnabled(bool)));
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeFromMinute, SLOT(setEnabled(bool)));
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeFromSecond, SLOT(setEnabled(bool)));
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeToHour, SLOT(setEnabled(bool)));
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeToMinute, SLOT(setEnabled(bool)));
        QObject::connect(timeCheck, SIGNAL(toggled(bool)), timeToSecond, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoggerWidget);
    } // setupUi

    void retranslateUi(QWidget *LoggerWidget)
    {
        tabWidget->setTabText(tabWidget->indexOf(liveTab), QCoreApplication::translate("LoggerWidget", "\345\256\236\346\227\266\346\227\245\345\277\227", nullptr));
        labelDate->setText(QCoreApplication::translate("LoggerWidget", "\346\227\245\346\234\237\357\274\232", nullptr));
        selectDateBtn->setText(QCoreApplication::translate("LoggerWidget", "\351\200\211\346\213\251\346\227\245\346\234\237", nullptr));
        timeCheck->setText(QCoreApplication::translate("LoggerWidget", "\346\227\266\351\227\264\345\214\272\351\227\264\357\274\232", nullptr));
        labelTimeFrom->setText(QCoreApplication::translate("LoggerWidget", ":", nullptr));
        labelTimeFrom2->setText(QCoreApplication::translate("LoggerWidget", ":", nullptr));
        labelTimeTo->setText(QCoreApplication::translate("LoggerWidget", "\350\207\263", nullptr));
        labelTimeTo2->setText(QCoreApplication::translate("LoggerWidget", ":", nullptr));
        labelTimeTo3->setText(QCoreApplication::translate("LoggerWidget", ":", nullptr));
        labelLike->setText(QCoreApplication::translate("LoggerWidget", "\346\250\241\347\263\212\345\214\271\351\205\215\357\274\232", nullptr));
        likeEdit->setPlaceholderText(QCoreApplication::translate("LoggerWidget", "\347\225\231\347\251\272\345\210\231\344\270\215\345\201\232\346\250\241\347\263\212\345\214\271\351\205\215", nullptr));
        searchBtn->setText(QCoreApplication::translate("LoggerWidget", "\346\237\245\350\257\242", nullptr));
        findPrevBtn->setText(QCoreApplication::translate("LoggerWidget", "\346\237\245\346\211\276\345\211\215\344\270\200\344\270\252", nullptr));
        findNextBtn->setText(QCoreApplication::translate("LoggerWidget", "\346\237\245\346\211\276\345\220\216\344\270\200\344\270\252", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(historyTab), QCoreApplication::translate("LoggerWidget", "\345\216\206\345\217\262\346\237\245\350\257\242", nullptr));
        (void)LoggerWidget;
    } // retranslateUi

};

namespace Ui {
    class LoggerWidget: public Ui_LoggerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGGERWIDGET_H
