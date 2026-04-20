#include "debugpage.h"
#include "ui_debugpage.h"
#include "customwidget/debugsettingwidget/firmwareupdateconfigsettingwidget.h"
#include "customwidget/debugsettingwidget/firmwareupdatesettingwidget.h"

DebugPage::DebugPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPage)
    , m_firmwareConfigWidget(nullptr)
    , m_firmwareUpdateWidget(nullptr)
{
    ui->setupUi(this);

    initUI();
}

DebugPage::~DebugPage()
{
    delete ui;
}

void DebugPage::initUI()
{
    initNav();
    
    // 固件更新配置 SettingWidget
    m_firmwareConfigWidget = new FirmwareUpdateConfigSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_firmwareConfigWidget);

    // 固件升级 SettingWidget（独立的 SettingWidget，与配置界面平级）
    m_firmwareUpdateWidget = new FirmwareUpdateSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_firmwareUpdateWidget);

    // 配置界面选择 bin 文件后 → 同步到升级界面
    connect(m_firmwareConfigWidget, &FirmwareUpdateConfigSettingWidget::binFilePathChanged,
            m_firmwareUpdateWidget, &FirmwareUpdateSettingWidget::setFirmwareFilePath);
    
    ui->scrollAreaWidgetContents->layout()->addItem(
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)
    );
}

void DebugPage::initNav()
{
    ui->widgetTop->setProperty("nav", "top");
    QList<QToolButton *> btns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, btns) {
        btn->setCheckable(true);
        connect(btn, &QPushButton::clicked, this, &DebugPage::navBtnClicked);
    }
}


void DebugPage::navBtnClicked()
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
