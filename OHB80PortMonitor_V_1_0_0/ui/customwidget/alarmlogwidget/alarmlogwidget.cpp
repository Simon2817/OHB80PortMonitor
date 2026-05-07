#include "alarmlogwidget.h"
#include "ui_alarmlogwidget.h"
#include "datetimesetdialog.h"
#include "alarmtype.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/alarmlogquerytask.h"
#include "paginationwidget.h"
#include "logdatabases/databasemanager.h"
#include "logdatabases/alarmlogdb/alarmlogdbcon.h"
#include <QStandardItemModel>
#include <QDebug>

AlarmLogWidget::AlarmLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmLogWidget)
    , m_currentPage(1)
    , m_pageSize(500)
    , m_totalPages(0)
    , m_lastAlarmLevel(-1)
    , m_lastIsResolved(-1)
{
    ui->setupUi(this);

    connect(ui->checkBoxAll, &QCheckBox::stateChanged,
            this, &AlarmLogWidget::onCheckBoxAllStateChanged);

    connect(ui->pushButtonSetStartTime, &QPushButton::clicked,
            this, &AlarmLogWidget::onSetStartTimeClicked);
    connect(ui->pushButtonSetResolvedTime, &QPushButton::clicked,
            this, &AlarmLogWidget::onSetResolvedTimeClicked);

    connect(ui->pushButtonSearch, &QPushButton::clicked,
            this, &AlarmLogWidget::onSearchClicked);

    connect(ui->widgetPaginate, &PaginationWidget::currentPageChanged,
            this, &AlarmLogWidget::onPaginationPageChanged);

    // ---- checkBox 联动：勾选才启用对应输入控件 ----
    auto bindEnable = [](QCheckBox* cb, const QList<QWidget*>& targets) {
        auto apply = [cb, targets]() {
            const bool on = cb->isChecked();
            for (QWidget* w : targets) if (w) w->setEnabled(on);
        };
        QObject::connect(cb, &QCheckBox::toggled, cb, apply);
        apply(); // 初始化为当前 checkBox 状态
    };
    bindEnable(ui->checkBoxQRCode,       { ui->spinBoxQRCode });
    bindEnable(ui->checkBoxAlarmLevel,   { ui->comboBoxAlarmLevel });
    bindEnable(ui->checkBoxAlarmType,    { ui->comboBoxAlarmType });
    bindEnable(ui->checkBoxIsResolved,   { ui->comboBoxIsResolved });
    bindEnable(ui->checkBoxStartTime,    { ui->lineEditStartTime, ui->lineEditEndTime, ui->pushButtonSetStartTime });
    bindEnable(ui->checkBoxResolvedTime, { ui->lineEditResolvedTime, ui->lineEditResolvedEndTime, ui->pushButtonSetResolvedTime });

    // ---- 时间 lineEdit 设为只读（仅通过 DateTimeSetDialog 设值）----
    ui->lineEditStartTime->setReadOnly(true);
    ui->lineEditEndTime->setReadOnly(true);
    ui->lineEditResolvedTime->setReadOnly(true);
    ui->lineEditResolvedEndTime->setReadOnly(true);

    initLiveLog();

    // history log 表：最后一列拉伸充满剩余宽度
    ui->tableViewHistoryLog->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewHistoryLog->verticalHeader()->setVisible(false);
}

void AlarmLogWidget::initLiveLog()
{
    // 表头与历史查询表保持一致（除 id 列——live log 无法提供）
    auto* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "Alarm Level", "Occur Time", "QRCode", "Alarm Type",
        "Is Resolved", "Resolve Time", "Customer Visible", "Description"
    });
    ui->tableViewLiveLog->setModel(model);
    ui->tableViewLiveLog->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewLiveLog->verticalHeader()->setVisible(false);

    if (auto* db = LogDB::DatabaseManager::instance().alarmLogCon()) {
        connect(db, &LogDB::AlarmLogDBCon::recordInserted,
                this, &AlarmLogWidget::onRecordInserted);
        connect(db, &LogDB::AlarmLogDBCon::recordResolved,
                this, &AlarmLogWidget::onRecordResolved);
    }

    // 启动时预点状态：将上一次未解决的警报载入 live log
    loadUnresolvedToLiveLog();
}

void AlarmLogWidget::loadUnresolvedToLiveLog()
{
    auto* db = LogDB::DatabaseManager::instance().alarmLogCon();
    if (!db) return;

    // 查询 is_resolved=0 的未解决记录（SQL 结果按 occur_time DESC）
    const QList<QVariantMap> rows = db->queryPageWithConditions(
        /*alarmLevel*/ -1,
        /*qrCode*/ QString(),
        /*alarmType*/ QString(),
        /*isResolved*/ 0,
        /*customerVisible*/ -1,
        /*startTime*/ QString(),
        /*endTime*/ QString(),
        /*pageSize*/ kLiveLogMaxRows,
        /*pageNumber*/ 1);

    // onRecordInserted 采用 insertRow(0)（最新在顶），
    // 需逆序递交才能让最新一条最后插入、位于 row 0
    for (auto it = rows.crbegin(); it != rows.crend(); ++it) {
        onRecordInserted(*it);
    }
}

void AlarmLogWidget::onRecordResolved(const QString& qrCode,
                                      const QString& alarmType,
                                      const QString& resolveTime)
{
    auto* model = qobject_cast<QStandardItemModel*>(ui->tableViewLiveLog->model());
    if (!model) return;

    // alarm_type 列存的是友好名称（alarmTypeName），使同样映射后再比较
    bool typeOk = false;
    const int typeVal = alarmType.toInt(&typeOk);
    const QString typeText = typeOk ? alarmTypeName(typeVal) : alarmType;
    const QString resolvedText = alarmResolvedStatusName(1);

    // 列顺序：0=Level 1=OccurTime 2=QRCode 3=AlarmType 4=IsResolved 5=ResolveTime ...
    constexpr int kColQrCode      = 2;
    constexpr int kColAlarmType   = 3;
    constexpr int kColIsResolved  = 4;
    constexpr int kColResolveTime = 5;

    // 从顶部（最新）向下查找首个未解决且 (qrCode, alarmType) 匹配的行
    const int rows = model->rowCount();
    for (int r = 0; r < rows; ++r) {
        const QString rowQr   = model->item(r, kColQrCode)   ? model->item(r, kColQrCode)->text()   : QString();
        const QString rowType = model->item(r, kColAlarmType)? model->item(r, kColAlarmType)->text(): QString();
        const QString rowRes  = model->item(r, kColIsResolved)? model->item(r, kColIsResolved)->text(): QString();
        if (rowQr == qrCode && rowType == typeText && rowRes != resolvedText) {
            if (auto* it = model->item(r, kColIsResolved))  it->setText(resolvedText);
            if (auto* it = model->item(r, kColResolveTime)) it->setText(resolveTime);
            return;
        }
    }
}

void AlarmLogWidget::onRecordInserted(const QVariantMap& row)
{
    auto* model = qobject_cast<QStandardItemModel*>(ui->tableViewLiveLog->model());
    if (!model) return;

    // alarm_level / alarm_type / is_resolved / customer_visible 做友好化映射
    const int levelVal = row.value("alarm_level").toInt();
    const QString levelText = alarmLevelName(levelVal);

    const QString typeRaw = row.value("alarm_type").toString();
    bool typeOk = false;
    const int typeVal = typeRaw.toInt(&typeOk);
    const QString typeText = typeOk ? alarmTypeName(typeVal) : typeRaw;

    const int resolvedVal = row.value("is_resolved").toInt();
    const QString resolvedText = alarmResolvedStatusName(resolvedVal);

    const int cv = row.value("customer_visible").toInt();
    const QString cvText = (cv == 1) ? QStringLiteral("Visible") : QStringLiteral("Hidden");

    QList<QStandardItem*> items;
    items << new QStandardItem(levelText)
          << new QStandardItem(row.value("occur_time").toString())
          << new QStandardItem(row.value("qr_code").toString())
          << new QStandardItem(typeText)
          << new QStandardItem(resolvedText)
          << new QStandardItem(row.value("resolve_time").toString())
          << new QStandardItem(cvText)
          << new QStandardItem(row.value("description").toString());

    model->insertRow(0, items);
    while (model->rowCount() > kLiveLogMaxRows) {
        model->removeRow(model->rowCount() - 1);
    }
}

void AlarmLogWidget::initUi()
{
    // QRCode 设备编号范围 1~10（与写入测试中的 DEVICE-0001..DEVICE-0010 对齐）
    ui->spinBoxQRCode->setRange(1, 10);
    ui->spinBoxQRCode->setValue(1);

    // 警报级别：按 alarmtype.h 中定义的 AlarmLevel 枚举填充
    ui->comboBoxAlarmLevel->clear();
    for (const auto& it : alarmLevelList()) {
        ui->comboBoxAlarmLevel->addItem(it.first, it.second);
    }

    // 警报类型：按 alarmtype.h 中定义的 AlarmType 枚举填充（显示名称，data=枚举值）
    ui->comboBoxAlarmType->clear();
    for (const auto& it : alarmTypeList()) {
        ui->comboBoxAlarmType->addItem(it.first, it.second);
    }

    // 是否解决：按 alarmtype.h 中定义的 AlarmResolvedStatus 枚举填充
    ui->comboBoxIsResolved->clear();
    for (const auto& it : alarmResolvedStatusList()) {
        ui->comboBoxIsResolved->addItem(it.first, it.second);
    }
}

void AlarmLogWidget::setCustomerVisibleFilter(int value)
{
    // -1: 不应用该条件；0: 客户不可见；1: 客户可见
    m_customerVisibleFilter = value;
}

int AlarmLogWidget::customerVisibleFilter() const
{
    return m_customerVisibleFilter;
}

void AlarmLogWidget::onSetStartTimeClicked()
{
    DateTimeSetDialog dialog(this);
    dialog.setStartTime(ui->lineEditStartTime->text().trimmed());
    dialog.setEndTime(ui->lineEditEndTime->text().trimmed());
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    // 仅当对话框中对应的开关启用时才回填文本，否则清空
    const QString startText = dialog.isStartTimeEnabled() ? dialog.getStartTime() : QString();
    const QString endText   = dialog.isEndTimeEnabled()   ? dialog.getEndTime()   : QString();
    ui->lineEditStartTime->setText(startText);
    ui->lineEditEndTime->setText(endText);

    // 用户在对话框中启用了任一时间 → 自动勾选 checkBoxStartTime；都未启用 → 取消勾选
    const bool anyEnabled = !startText.isEmpty() || !endText.isEmpty();
    ui->checkBoxStartTime->setChecked(anyEnabled);
}

void AlarmLogWidget::onSetResolvedTimeClicked()
{
    DateTimeSetDialog dialog(this);
    dialog.setStartTime(ui->lineEditResolvedTime->text().trimmed());
    dialog.setEndTime(ui->lineEditResolvedEndTime->text().trimmed());
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString startText = dialog.isStartTimeEnabled() ? dialog.getStartTime() : QString();
    const QString endText   = dialog.isEndTimeEnabled()   ? dialog.getEndTime()   : QString();
    ui->lineEditResolvedTime->setText(startText);
    ui->lineEditResolvedEndTime->setText(endText);

    const bool anyEnabled = !startText.isEmpty() || !endText.isEmpty();
    ui->checkBoxResolvedTime->setChecked(anyEnabled);
}

void AlarmLogWidget::onSearchClicked()
{
    // 从 UI 采集查询条件并缓存，供翻页复用
    m_lastAlarmLevel = -1;
    m_lastQRCode.clear();
    m_lastAlarmType.clear();
    m_lastIsResolved = -1;
    m_lastStartTime.clear();
    m_lastEndTime.clear();

    if (ui->checkBoxQRCode->isChecked()) {
        m_lastQRCode = QString::number(ui->spinBoxQRCode->value());
    }

    if (ui->checkBoxAlarmLevel->isChecked()) {
        QVariant data = ui->comboBoxAlarmLevel->currentData();
        bool ok = false;
        int level = data.isValid() ? data.toInt(&ok)
                                   : ui->comboBoxAlarmLevel->currentText().toInt(&ok);
        if (ok) {
            m_lastAlarmLevel = level;
        }
    }

    if (ui->checkBoxAlarmType->isChecked()) {
        QVariant data = ui->comboBoxAlarmType->currentData();
        bool ok = false;
        int typeValue = data.isValid() ? data.toInt(&ok) : 0;
        if (ok && typeValue > 0) {
            // alarm_type 列为 TEXT，以枚举整数的字符串形式存入 / 查询
            m_lastAlarmType = QString::number(typeValue);
        }
    }

    if (ui->checkBoxIsResolved->isChecked()) {
        QVariant data = ui->comboBoxIsResolved->currentData();
        bool ok = false;
        int v = data.isValid() ? data.toInt(&ok)
                               : ui->comboBoxIsResolved->currentText().toInt(&ok);
        if (ok) {
            m_lastIsResolved = v;
        }
    }

    if (ui->checkBoxStartTime->isChecked()) {
        QString startTime = ui->lineEditStartTime->text().trimmed();
        QString endTime   = ui->lineEditEndTime->text().trimmed();
        if (!startTime.isEmpty() || !endTime.isEmpty()) {
            m_lastStartTime = startTime;
            m_lastEndTime   = endTime;
        }
    }

    // 点击 Search 总是回到第 1 页
    m_currentPage = 1;
    submitQuery(m_currentPage);
}

void AlarmLogWidget::onPaginationPageChanged(int page)
{
    if (page <= 0) return;
    m_currentPage = page;
    submitQuery(page);
}

void AlarmLogWidget::submitQuery(int page)
{
    AlarmLogQueryTask* task = new AlarmLogQueryTask();
    task->setPageNumber(page);
    task->setPageSize(m_pageSize);

    if (m_lastAlarmLevel != -1)      task->setAlarmLevel(m_lastAlarmLevel);
    if (!m_lastQRCode.isEmpty())     task->setQRCode(m_lastQRCode);
    if (!m_lastAlarmType.isEmpty())  task->setAlarmType(m_lastAlarmType);
    if (m_lastIsResolved != -1)      task->setIsResolved(m_lastIsResolved);
    // 外部注入的客户可见过滤，每次提交查询都会使用
    if (m_customerVisibleFilter != -1) {
        task->setCustomerVisible(m_customerVisibleFilter);
    }
    if (!m_lastStartTime.isEmpty() || !m_lastEndTime.isEmpty()) {
        QString s = m_lastStartTime;
        QString e = m_lastEndTime;
        // 自动纠正顺序颠倒：开始时间晚于结束时间会让 BETWEEN 返回空集
        if (!s.isEmpty() && !e.isEmpty() && s > e) {
            qSwap(s, e);
        }
        task->setOccurTimeRange(s, e);
    }

    connect(task, &AlarmLogQueryTask::pageWithConditionsResult,
            this, &AlarmLogWidget::onPageWithConditionsResult, Qt::QueuedConnection);
    connect(task, &AlarmLogQueryTask::totalCountWithConditionsResult,
            this, &AlarmLogWidget::onTotalCountWithConditionsResult, Qt::QueuedConnection);

    Scheduler::instance()->submitTask(task);
}

void AlarmLogWidget::onPageWithConditionsResult(const QList<QVariantMap>& records)
{
    setHistoryLogData(records);
}

void AlarmLogWidget::onTotalCountWithConditionsResult(int totalCount)
{
    const int pageSize = (m_pageSize > 0) ? m_pageSize : 1;
    m_totalPages = (totalCount + pageSize - 1) / pageSize;
    ui->widgetPaginate->setTotalPages(m_totalPages);
    ui->widgetPaginate->setCurrentPage(m_currentPage);
    qDebug() << "[AlarmLogWidget] 条件查询 当前页/总页数:"
             << m_currentPage << "/" << m_totalPages
             << " 总记录数:" << totalCount;
}

void AlarmLogWidget::setHistoryLogData(const QList<QVariantMap>& data)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableViewHistoryLog->model());
    if (!model) {
        model = new QStandardItemModel(this);
        ui->tableViewHistoryLog->setModel(model);
    }

    model->clear();

    if (data.isEmpty()) {
        return;
    }

    QStringList headers;
    headers << "ID" << "Alarm Level" << "Occur Time" << "QRCode" << "Alarm Type"
            << "Is Resolved" << "Resolve Time" << "Customer Visible" << "Description";
    model->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < data.size(); ++row) {
        const QVariantMap& r = data[row];
        // alarm_level 以枚举名称显示
        const int levelVal = r.value("alarm_level").toInt();
        const QString levelText = alarmLevelName(levelVal);
        // alarm_type 存为 TEXT（枚举整数字符串），转成名称显示
        const QString typeRaw = r.value("alarm_type").toString();
        bool typeOk = false;
        const int typeVal = typeRaw.toInt(&typeOk);
        const QString typeText = typeOk ? alarmTypeName(typeVal) : typeRaw;
        // is_resolved 以枚举名称显示
        const int resolvedVal = r.value("is_resolved").toInt();
        const QString resolvedText = alarmResolvedStatusName(resolvedVal);
        // customer_visible 显示友好化
        const int cv = r.value("customer_visible").toInt();
        const QString cvText = (cv == 1) ? QStringLiteral("Visible") : QStringLiteral("Hidden");

        model->setItem(row, 0, new QStandardItem(r.value("id").toString()));
        model->setItem(row, 1, new QStandardItem(levelText));
        model->setItem(row, 2, new QStandardItem(r.value("occur_time").toString()));
        model->setItem(row, 3, new QStandardItem(r.value("qr_code").toString()));
        model->setItem(row, 4, new QStandardItem(typeText));
        model->setItem(row, 5, new QStandardItem(resolvedText));
        model->setItem(row, 6, new QStandardItem(r.value("resolve_time").toString()));
        model->setItem(row, 7, new QStandardItem(cvText));
        model->setItem(row, 8, new QStandardItem(r.value("description").toString()));
    }

    ui->tableViewHistoryLog->resizeColumnsToContents();
}

AlarmLogWidget::~AlarmLogWidget()
{
    delete ui;
}

void AlarmLogWidget::onCheckBoxAllStateChanged(int state)
{
    bool checked = (state == Qt::Checked);
    ui->checkBoxQRCode->setChecked(checked);
    ui->checkBoxAlarmLevel->setChecked(checked);
    ui->checkBoxAlarmType->setChecked(checked);
    ui->checkBoxIsResolved->setChecked(checked);
    ui->checkBoxStartTime->setChecked(checked);
    ui->checkBoxResolvedTime->setChecked(checked);
}
