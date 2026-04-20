#include "debugpage.h"
#include "ui_debugpage.h"
#include "customwidget/debugpage/firmwareupdateconfigsettingwidget.h"

DebugPage::DebugPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPage)
    , m_firmwareConfigWidget(nullptr)
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
    
    // 创建并添加固件更新配置控件
    m_firmwareConfigWidget = new FirmwareUpdateConfigSettingWidget(this);
    ui->scrollAreaWidgetContents->layout()->addWidget(m_firmwareConfigWidget);
    
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
