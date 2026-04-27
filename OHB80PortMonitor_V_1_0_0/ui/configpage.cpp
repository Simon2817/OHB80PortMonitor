#include "configpage.h"
#include "ui_configpage.h"
#include "customwidget/configsettingwidget/idlepurgesettingwidget.h"

ConfigPage::ConfigPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigPage)
    , m_idlePurgeWidget(nullptr)
{
    ui->setupUi(this);
    initUI();
}

ConfigPage::~ConfigPage()
{
    delete ui;
}

void ConfigPage::initUI()
{
    initNav();

    m_idlePurgeWidget = new IdlePurgeSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_idlePurgeWidget);

    ui->scrollAreaWidgetContents->layout()->addItem(
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)
        );
}

void ConfigPage::initNav()
{
    ui->widgetTop->setProperty("nav", "top");
    QList<QToolButton *> btns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this, &ConfigPage::navBtnClicked);
    }
}

void ConfigPage::navBtnClicked()
{
    QToolButton *btn = (QToolButton *)sender();
    QString objName = btn->objectName();

    QList<QToolButton *> btns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *b, btns) {
        b->setChecked(b == btn);
    }

    QString key;
    if (objName == "btnHSMS") {
        key = "idle_purge";
    }

    // if (m_settingWidgetList.contains(key)) {
    //     SettingWidget *target = m_settingWidgetList[key];
    //     QPoint pos = target->mapTo(ui->scrollAreaWidgetContents, QPoint(0, 0));
    //     ui->scrollArea->verticalScrollBar()->setValue(pos.y());
    // }
}
