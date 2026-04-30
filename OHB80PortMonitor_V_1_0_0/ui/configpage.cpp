#include "configpage.h"
#include "ui_configpage.h"
#include "customwidget/configsettingwidget/idlepurgesettingwidget.h"
#include "customwidget/configsettingwidget/pneumaticvalvepressuresettingwidget.h"
#include "customwidget/configsettingwidget/sh85selfchecksettingwidget.h"
#include "customwidget/configsettingwidget/humidityoffsetsettingwidget.h"
#include "customwidget/configsettingwidget/purgeflowsettingwidget.h"
#include <QScrollBar>

ConfigPage::ConfigPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigPage)
    , m_idlePurgeWidget(nullptr)
    , m_pneumaticValvePressureWidget(nullptr)
    , m_sh85SelfCheckWidget(nullptr)
    , m_humidityOffsetWidget(nullptr)
    , m_purgeFlowWidget(nullptr)
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

    m_pneumaticValvePressureWidget = new PneumaticValvePressureSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_pneumaticValvePressureWidget);

    m_sh85SelfCheckWidget = new SH85SelfCheckSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_sh85SelfCheckWidget);

    m_humidityOffsetWidget = new HumidityOffsetSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_humidityOffsetWidget);

    m_purgeFlowWidget = new PurgeFlowSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_purgeFlowWidget);

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

    QWidget *targetWidget = nullptr;
    if (objName == "btnIdelPurge") {
        targetWidget = m_idlePurgeWidget;
    } else if (objName == "btnPneumaticValvePressure") {
        targetWidget = m_pneumaticValvePressureWidget;
    } else if (objName == "btnSH85SelfCheck") {
        targetWidget = m_sh85SelfCheckWidget;
    } else if (objName == "btnHumidityOffset") {
        targetWidget = m_humidityOffsetWidget;
    } else if (objName == "btnPurgeFlow") {
        targetWidget = m_purgeFlowWidget;
    }

    if (targetWidget) {
        QPoint pos = targetWidget->mapTo(ui->scrollAreaWidgetContents, QPoint(0, 0));
        ui->scrollArea->verticalScrollBar()->setValue(pos.y());
    }
}
