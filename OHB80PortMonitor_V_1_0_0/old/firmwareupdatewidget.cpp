#include "firmwareupdatewidget.h"
#include "ui_firmwareupdatewidget.h"
#include "modbus_tcp_master_manager.h"
#include "modbus_tcp_master.h"
#include "loggertablewidget.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/firmware_upgrade_task.h"
#include "scheduler/tasks/repeated_firmware_upgrade_task.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QComboBox>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QSet>
#include <QMetaType>
#include <QPointer>
#include <QDebug>
#include <QRegularExpression>
#include <algorithm>
#include <QDateTime>
#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <QScrollBar>
#include <QApplication>
#include <QTimer>

#include "mtm_types.h"

// 注册枚举类型为Qt元类型
Q_DECLARE_METATYPE(Mtm::FirmwareUpgradeResultCode)

static QString upgradeStateToPhase(MtmFirmwareUpgrader::UpgradeState state)
{
    using S = MtmFirmwareUpgrader::UpgradeState;
    switch (state) {
    case S::Preparing:
    case S::PrepareCmdSent:
    case S::PrepareCmdFinished:     return QStringLiteral("准备阶段");
    case S::WaitingDevice:
    case S::WaitingDeviceFinished:  return QStringLiteral("等待设备");
    case S::DataTransferStarted:
    case S::SendingDataFrame:
    case S::SendingLastFrame:
    case S::DataTransferFinished:   return QStringLiteral("数据传输");
    case S::VersionCmdSent:
    case S::VersionCmdFinished:     return QStringLiteral("版本验证");
    case S::Finished:               return QStringLiteral("完成");
    }
    return QStringLiteral("未知");
}

FirmwareUpdateWidget::FirmwareUpdateWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FirmwareUpdateWidget)
    , m_totalDevices(0)
    , m_completedDevices(0)
    , m_successCount(0)
    , m_failCount(0)
    , m_captureDirectory(QApplication::applicationDirPath() + "/firmware/capture")
{
    // 注册枚举类型为Qt元类型，支持跨线程信号传递
    qRegisterMetaType<Mtm::FirmwareUpgradeResultCode>("Mtm::FirmwareUpgradeResultCode");
    
    ui->setupUi(this);
    initUI();
}

void FirmwareUpdateWidget::initUI()
{
    // 初始化表格
    initTableWidgetSelectedDevices();
    
    // 初始化进度条
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    ui->progressBar->setFormat("0/0");  // 初始格式

    // 选中设备进行固件升级
    connect(ui->btnAddDevice, &QPushButton::clicked, this, &FirmwareUpdateWidget::onAddDeviceForUpdate);
    connect(ui->btnAddAllDevices, &QPushButton::clicked, this, &FirmwareUpdateWidget::onAddAllDevices);
    connect(ui->btnClear, &QPushButton::clicked, this, &FirmwareUpdateWidget::onClear);
    connect(ui->btnUdateSelected, &QPushButton::clicked, this, &FirmwareUpdateWidget::onUpdateSelectedDevices);
    
    // 连接ModbusTcpMasterManager的信号
    ModbusTcpMasterManager *manager = ModbusTcpMasterManager::instance();
    if (manager) {

    }

}

FirmwareUpdateWidget::~FirmwareUpdateWidget()
{
    delete ui;
}

void FirmwareUpdateWidget::onAddDeviceForUpdate()
{
    // 获取QRCode并查询设备是否存在
    QString qrCode = ui->lineEditQRCode->text().trimmed();
    if (qrCode.isEmpty()) {
        return;
    }
    
    // 检查表格中是否已存在该QRCode
    bool deviceExists = false;
    for (int row = 0; row < ui->tableWidgetSelectedDevices->rowCount(); ++row) {
        QLabel* qrCodeLabel = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 0));
        if (qrCodeLabel && qrCodeLabel->text() == qrCode) {
            deviceExists = true;
            break;
        }
    }
    
    if (deviceExists) {
        // 设备已存在于表格中，可以添加提示信息
        QMessageBox::information(this, "Information", "Device already exists in the update list!");
        return;
    }
    
    // 查询设备是否存在
    ModbusTcpMasterManager *manager = ModbusTcpMasterManager::instance();
    if (!manager->contains(qrCode)) {
        // 设备不存在，可以在这里添加提示信息
        QMessageBox::warning(this, "Warning", "Device not found!");
        return;
    }
    
    // 设备存在，添加到表格
    ModbusTcpMaster *master = manager->getMaster(qrCode);
    if (master) {
        addDeviceToTable(master);
    }
}

void FirmwareUpdateWidget::onAddAllDevices()
{
    // 获取 ModbusTcpMasterManager 实例
    ModbusTcpMasterManager *manager = ModbusTcpMasterManager::instance();
    if (!manager) {
        // QMessageBox::warning(this, "Warning", "ModbusTcpMasterManager not available!");
        return;
    }
    
    // 获取所有设备的 QRCode 列表
    QStringList allDeviceIds = manager->masterIds();
    
    if (allDeviceIds.isEmpty()) {
        // QMessageBox::information(this, "Information", "No devices found!");
        return;
    }
    
    // 先清空表格
    int previousCount = ui->tableWidgetSelectedDevices->rowCount();
    ui->tableWidgetSelectedDevices->setRowCount(0);
    
    // 按数字排序 QRCode
    std::sort(allDeviceIds.begin(), allDeviceIds.end(), [](const QString &a, const QString &b) {
        // 从 QRCode 字符串中提取数字部分进行比较
        QRegularExpression re("\\d+");
        QRegularExpressionMatch matchA = re.match(a);
        QRegularExpressionMatch matchB = re.match(b);
        
        if (matchA.hasMatch() && matchB.hasMatch()) {
            int numA = matchA.captured().toInt();
            int numB = matchB.captured().toInt();
            return numA < numB;
        }
        
        // 如果没有数字，按字符串比较
        return a < b;
    });
    
    // 添加排序后的所有设备到表格
    int addedCount = 0;
    for (const QString &qrCode : allDeviceIds) {
        ModbusTcpMaster *master = manager->getMaster(qrCode);
        if (master) {
            addDeviceToTable(master);
            addedCount++;
        }
    }
    
    // 显示添加结果
    QString message = QString("Cleared %1 device(s) from table.\nAdded %2 device(s) in numerical order.")
                          .arg(previousCount)
                          .arg(addedCount);
    // QMessageBox::information(this, "Add All Devices", message);
    
    qDebug() << "[FirmwareUpdateWidget] Cleared" << previousCount << "devices, added" << addedCount << "devices in numerical order";
    
    // 更新表格高度
    updateTableHeight();
    
    // 重置进度状态
    resetProgress();
}

void FirmwareUpdateWidget::onClear()
{
    // 检查表格是否有数据
    int rowCount = ui->tableWidgetSelectedDevices->rowCount();
    
    if (rowCount == 0) {
        // QMessageBox::information(this, "Clear Table", "Table is already empty!");
        return;
    }
    
    // 确认对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Clear All Devices",
        QString("Are you sure you want to clear all %1 device(s) from the table?").arg(rowCount),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        ui->tableWidgetSelectedDevices->setRowCount(0);
        m_qrcodeRowMap.clear();
        
        // QMessageBox::information(this, "Clear Table",
        //                          QString("Successfully cleared %1 device(s) from the table.").arg(rowCount));
        
        // 更新表格高度
        updateTableHeight();
        
        // 重置进度状态
        resetProgress();
    }
}

void FirmwareUpdateWidget::onUpdateSelectedDevices()
{
    qDebug() << "[FirmwareUpdateWidget] onUpdateSelectedDevices called";

    // 1. 验证固件文件是否存在
    if (m_firmwareFilePath.isEmpty()) {
        qDebug() << "[FirmwareUpdateWidget] Firmware file path is empty";
        QMessageBox::warning(this, "Warning", "Please import .bin file first!");
        return;
    }
    
    QFile file(m_firmwareFilePath);
    if (!file.exists()) {
        qDebug() << "[FirmwareUpdateWidget] Firmware file not found:" << m_firmwareFilePath;
        QMessageBox::warning(this, "Warning",
            QString("Firmware file not found!\n%1\nPlease import .bin file first.").arg(m_firmwareFilePath));
        return;
    }
    
    qDebug() << "[FirmwareUpdateWidget] Firmware file OK:" << m_firmwareFilePath;
    
    // 2. 遍历表格获取所有设备的QRCode列表
    QStringList selectedDeviceIds;
    
    if (ui->tableWidgetSelectedDevices->rowCount() == 0) {
        qDebug() << "[FirmwareUpdateWidget] No devices in table";
        QMessageBox::information(this, "Information", "No devices in the update list!");
        return;
    }
    
    qDebug() << "[FirmwareUpdateWidget] Table has" << ui->tableWidgetSelectedDevices->rowCount() << "devices";
    
    // 遍历所有行获取QRCode
    for (int row = 0; row < ui->tableWidgetSelectedDevices->rowCount(); ++row) {
        QLabel* qrCodeLabel = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 0));
        if (qrCodeLabel) {
            selectedDeviceIds.append(qrCodeLabel->text());
            qDebug() << "[FirmwareUpdateWidget] Added device:" << qrCodeLabel->text();
        }
    }
    
    if (selectedDeviceIds.isEmpty()) {
        qDebug() << "[FirmwareUpdateWidget] No valid devices found";
        QMessageBox::warning(this, "Warning", "No valid devices selected!");
        return;
    }
    
    // 启动前确认
    QFileInfo fi(m_firmwareFilePath);
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Firmware Update",
        QString("Start upgrading %1 device(s)?\n\nFirmware: %2")
            .arg(selectedDeviceIds.count())
            .arg(fi.fileName()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (confirm != QMessageBox::Yes) return;

    qDebug() << "[FirmwareUpdateWidget] Starting upgrade for" << selectedDeviceIds.count() << "devices";

    // 重置成功/失败计数
    m_successCount = 0;
    m_failCount = 0;

    // 3. 将表格中对应设备状态设置为 Wait（准备中）
    for (int row = 0; row < ui->tableWidgetSelectedDevices->rowCount(); ++row) {
        QLabel* qrCodeLabel = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 0));
        if (qrCodeLabel && selectedDeviceIds.contains(qrCodeLabel->text())) {
            QLabel* statusLabel = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 2));
            if (statusLabel) {
                statusLabel->setText(getStatusText(Waiting));
                statusLabel->setStyleSheet(getStatusStyle(Waiting));
                qDebug() << "[FirmwareUpdateWidget] Set status to Wait for device:" << qrCodeLabel->text();
            }
        }
    }
    
    // 4. 设置进度跟踪
    m_totalDevices = selectedDeviceIds.count();
    m_completedDevices = 0;
    m_completedDeviceIds.clear();
    updateProgressBar();
    
    // 5. 创建固件升级调度任务并连接信号

    // ---- [测试模式] 使用 RepeatedFirmwareUpgradeTask 重复升级（暂时注释）----
    // int repeatCount = 100;
    // RepeatedFirmwareUpgradeTask *task = new RepeatedFirmwareUpgradeTask(
    //     selectedDeviceIds, m_firmwareFilePath, repeatCount);
    // connect(task, &RepeatedFirmwareUpgradeTask::deviceProgress,  this, &FirmwareUpdateWidget::onTaskDeviceProgress,  Qt::QueuedConnection);
    // connect(task, &RepeatedFirmwareUpgradeTask::deviceStateLog,  this, &FirmwareUpdateWidget::onTaskDeviceStateLog,  Qt::QueuedConnection);
    // connect(task, &RepeatedFirmwareUpgradeTask::deviceFinished,  this, &FirmwareUpdateWidget::onTaskDeviceFinished,  Qt::QueuedConnection);
    // connect(task, &RepeatedFirmwareUpgradeTask::allProgress,     this, &FirmwareUpdateWidget::onTaskAllProgress,     Qt::QueuedConnection);
    // connect(task, &RepeatedFirmwareUpgradeTask::roundStarted,    this, &FirmwareUpdateWidget::onRoundStarted,         Qt::QueuedConnection);
    // connect(task, &RepeatedFirmwareUpgradeTask::roundFinished,   this, &FirmwareUpdateWidget::onRoundFinished,        Qt::QueuedConnection);
    // Scheduler::instance()->submitTask(task);
    // qDebug() << "[FirmwareUpdateWidget] RepeatedFirmwareUpgradeTask submitted, repeatCount=" << repeatCount;

    // ---- [正式模式] 使用 FirmwareUpgradeTask 单次升级 ----
    FirmwareUpgradeTask *task = new FirmwareUpgradeTask(selectedDeviceIds, m_firmwareFilePath);
    connect(task, &FirmwareUpgradeTask::deviceProgress,  this, &FirmwareUpdateWidget::onTaskDeviceProgress,  Qt::QueuedConnection);
    connect(task, &FirmwareUpgradeTask::deviceStateLog,  this, &FirmwareUpdateWidget::onTaskDeviceStateLog,  Qt::QueuedConnection);
    connect(task, &FirmwareUpgradeTask::deviceFinished,  this, &FirmwareUpdateWidget::onTaskDeviceFinished,  Qt::QueuedConnection);
    connect(task, &FirmwareUpgradeTask::allProgress,     this, &FirmwareUpdateWidget::onTaskAllProgress,     Qt::QueuedConnection);
    Scheduler::instance()->submitTask(task);
    qDebug() << "[FirmwareUpdateWidget] FirmwareUpgradeTask submitted to Scheduler";
}

// 设置固件升级文件路径
void FirmwareUpdateWidget::setFirmwareFilePath(const QString &filePath)
{
    m_firmwareFilePath = filePath;
}

QString FirmwareUpdateWidget::firmwareFilePath() const
{
    return m_firmwareFilePath;
}

QString FirmwareUpdateWidget::parseVersionFromFilePath(const QString &filePath) const
{
    if (filePath.isEmpty()) {
        return QString();
    }
    
    // 获取文件名（不包含路径）
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    
    // 解析文件名格式：OHB_V_1_0_APP.bin
    // 查找 "V_" 的位置
    int vPos = fileName.indexOf("V_");
    if (vPos < 0) {
        qDebug() << "[FirmwareUpdateWidget] Cannot find 'V_' in filename:" << fileName;
        return QString();
    }
    
    // 提取版本部分：从 V_ 之后开始，格式为 "1_0"
    QString versionPart = fileName.mid(vPos + 2); // 跳过 "V_"
    
    // 查找版本号结束位置：找到第二个 "_" 或 "."
    // 对于 "1_0_APP.bin"，我们需要提取 "1_0" 部分
    QStringList parts = versionPart.split('_');
    if (parts.size() >= 2) {
        // 取前两个部分作为版本号，如 ["1", "0", "APP.bin"] → "1_0"
        versionPart = parts[0] + "_" + parts[1];
    } else {
        // 如果没有足够的部分，尝试用 "." 分割
        parts = versionPart.split('.');
        if (parts.size() >= 2) {
            versionPart = parts[0] + "_" + parts[1];
        } else {
            // 如果都没有，直接使用整个字符串
            versionPart = parts[0];
        }
    }
    
    // 替换 "_" 为 "."，例如 "1_0" → "1.0"
    versionPart.replace('_', '.');
    
    qDebug() << "[FirmwareUpdateWidget] Parsed version from" << fileName << "→" << versionPart;
    return versionPart;
}

void FirmwareUpdateWidget::initTableWidget(QTableWidget *table, const QStringList &headers)
{
    // 设置表格列数和表头
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::MultiSelection);
    table->setAlternatingRowColors(true);
    
    // 设置列宽
    table->setColumnWidth(0, 150);  // QRCode
    table->setColumnWidth(1, 200);  // 进度条
    table->setColumnWidth(2, 100);  // 状态
    table->setColumnWidth(3, 200);  // Result
    table->setColumnWidth(4, 80);   // Action
    
    // 设置表头样式
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}

void FirmwareUpdateWidget::initTableWidgetSelectedDevices()
{
    QStringList headers = {"QRCode", "Progress", "Status", "Result Message", "Action"};
    initTableWidget(ui->tableWidgetSelectedDevices, headers);
    // 禁用竖向滚动条：配合 updateTableHeight() 始终展开全部行，确保截图完整
    ui->tableWidgetSelectedDevices->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void FirmwareUpdateWidget::addDeviceToTable(ModbusTcpMaster *master)
{
    int row = ui->tableWidgetSelectedDevices->rowCount();
    ui->tableWidgetSelectedDevices->insertRow(row);

    QString qrCode = master ? master->id() : "Unknown";

    // 第0列：QRCode
    QLabel *qrCodeLabel = new QLabel(qrCode);
    qrCodeLabel->setAlignment(Qt::AlignCenter);
    ui->tableWidgetSelectedDevices->setCellWidget(row, 0, qrCodeLabel);

    // 第1列：进度条
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setAlignment(Qt::AlignCenter);
    ui->tableWidgetSelectedDevices->setCellWidget(row, 1, progressBar);

    // 第2列：状态
    QLabel *statusLabel = new QLabel(getStatusText(Idle));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(getStatusStyle(Idle));
    ui->tableWidgetSelectedDevices->setCellWidget(row, 2, statusLabel);

    // 第3列：Result Message
    QLabel *resultMessageLabel = new QLabel("");
    resultMessageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    resultMessageLabel->setWordWrap(true);
    ui->tableWidgetSelectedDevices->setCellWidget(row, 3, resultMessageLabel);

    // 第4列：删除按钮
    QPushButton *deleteButton = new QPushButton("Delete");
    deleteButton->setMaximumWidth(70);
    ui->tableWidgetSelectedDevices->setCellWidget(row, 4, deleteButton);

    Q_UNUSED(master)

    // O(1) 行索引缓存
    m_qrcodeRowMap[qrCode] = row;

    connect(deleteButton, &QPushButton::clicked, this, [this, qrCode]() {
        int r = m_qrcodeRowMap.value(qrCode, -1);
        if (r >= 0) {
            ui->tableWidgetSelectedDevices->removeRow(r);
            // 重建 hash（删除行后行号软移）
            m_qrcodeRowMap.clear();
            for (int i = 0; i < ui->tableWidgetSelectedDevices->rowCount(); ++i) {
                QLabel *lbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(i, 0));
                if (lbl) m_qrcodeRowMap[lbl->text()] = i;
            }
            updateTableHeight();
        }
    });

    updateTableHeight();
}

void FirmwareUpdateWidget::updateTableHeight()
{
    int rowCount = ui->tableWidgetSelectedDevices->rowCount();
    
    // 每行高度设置为30像素
    const int rowHeight = 30;
    
    // 表头高度
    int headerHeight = ui->tableWidgetSelectedDevices->horizontalHeader()->height();
    
    // 计算总高度：表头 + 所有行 + 边框余量
    int totalHeight = headerHeight + (rowCount * rowHeight) + 4; // +4 为边框留余量
    
    // 设置最小高度和最大高度为相同值，实现固定高度
    ui->tableWidgetSelectedDevices->setMinimumHeight(totalHeight);
    ui->tableWidgetSelectedDevices->setMaximumHeight(totalHeight);
    
    // 如果没有行，设置一个最小高度以保持界面美观
    if (rowCount == 0) {
        ui->tableWidgetSelectedDevices->setMinimumHeight(headerHeight + 20);
        ui->tableWidgetSelectedDevices->setMaximumHeight(headerHeight + 20);
    }
    
    // 更新布局
    ui->tableWidgetSelectedDevices->updateGeometry();
    
    qDebug() << "[FirmwareUpdateWidget] Updated table height to" << totalHeight << "pixels for" << rowCount << "rows";
}

void FirmwareUpdateWidget::updateProgressBar()
{
    // 更新进度条格式：已完成数/总数
    QString format = QString("%1/%2").arg(m_completedDevices).arg(m_totalDevices);
    ui->progressBar->setFormat(format);
    
    // 计算百分比
    int percentage = 0;
    if (m_totalDevices > 0) {
        percentage = (m_completedDevices * 100) / m_totalDevices;
    }
    ui->progressBar->setValue(percentage);
    
}

void FirmwareUpdateWidget::resetProgress()
{
    m_totalDevices = 0;
    m_completedDevices = 0;
    m_successCount = 0;
    m_failCount = 0;
    m_completedDeviceIds.clear();
    updateProgressBar();
    
    qDebug() << "[FirmwareUpdateWidget] Progress reset";
}

QString FirmwareUpdateWidget::getStatusText(DeviceStatus status) const
{
    switch (status) {
    case Idle:
        return "Idle";
    case Waiting:
        return "Waiting";
    case Updating:
        return "Updating";
    case Success:
        return "Success";
    case Failed:
        return "Failed";
    default:
        return "Unknown";
    }
}

QString FirmwareUpdateWidget::getStatusStyle(DeviceStatus status) const
{
    switch (status) {
    case Idle:
        return "QLabel { color: green; font-weight: bold; }";
    case Waiting:
        return "QLabel { color: orange; font-weight: bold; }";
    case Updating:
        return "QLabel { color: blue; font-weight: bold; }";
    case Success:
        return "QLabel { color: #00AA00; font-weight: bold; }";
    case Failed:
        return "QLabel { color: red; font-weight: bold; }";
    default:
        return "QLabel { color: gray; font-weight: bold; }";
    }
}

// ======================================================
// 固件升级任务信号处理槽
// ======================================================
void FirmwareUpdateWidget::onTaskDeviceProgress(const QString &qrcode, int percent)
{
    int row = m_qrcodeRowMap.value(qrcode, -1);
    if (row < 0) return;
    QProgressBar *bar       = qobject_cast<QProgressBar*>(ui->tableWidgetSelectedDevices->cellWidget(row, 1));
    QLabel       *statusLbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 2));
    if (bar) bar->setValue(percent);
    if (statusLbl && percent > 0 && percent < 100) {
        statusLbl->setText(getStatusText(Updating));
        statusLbl->setStyleSheet(getStatusStyle(Updating));
    }
}

void FirmwareUpdateWidget::onTaskDeviceStateLog(const QString &qrcode,
                                                MtmFirmwareUpgrader::UpgradeState state,
                                                const QString &logMessage,
                                                const QByteArray &frame)
{
    using S = MtmFirmwareUpgrader::UpgradeState;

    // 所有状态均写入日志（SendingDataFrame 含第 i/N 包信息，不应过滤）
    LoggerTableWidget::instance()->log(LogLevel::Info,
        {{"QRCode", qrcode},
         {"Phase",  upgradeStateToPhase(state)},
         {"Message", logMessage}});

    // 表格第3列实时刷新：SendingDataFrame 高频（~6次/秒/设备），跳过 QLabel 更新避免 UI 抖动
    if (state != S::SendingDataFrame) {
        int row = m_qrcodeRowMap.value(qrcode, -1);
        if (row >= 0) {
            QLabel *resultLbl = qobject_cast<QLabel*>(
                ui->tableWidgetSelectedDevices->cellWidget(row, 3));
            if (resultLbl) resultLbl->setText(logMessage);
        }
    }

    // 数据帧原始内容写入：DataTransferStarted / SendingDataFrame / SendingLastFrame 且 frame 非空
    if (!frame.isEmpty() &&
        (state == S::DataTransferStarted ||
         state == S::SendingDataFrame    ||
         state == S::SendingLastFrame))
    {
        const int total = frame.size();
        for (int offset = 0; offset < total; offset += 32) {
            const int end = qMin(offset + 32, total);
            QString hex;
            hex.reserve((end - offset) * 3);
            for (int i = offset; i < end; ++i) {
                if (i > offset) hex += ' ';
                hex += QString::number(static_cast<quint8>(frame[i]), 16).rightJustified(2, '0').toUpper();
            }
            LoggerTableWidget::instance()->log(LogLevel::Trace,
                {{"QRCode", qrcode},
                 {"Phase",  QStringLiteral("数据帧")},
                 {"Message", hex}});
        }
    }
}

void FirmwareUpdateWidget::onTaskDeviceFinished(const QString &qrcode, bool success, const QString &message)
{
    int row = m_qrcodeRowMap.value(qrcode, -1);
    if (row >= 0) {
        QProgressBar *bar       = qobject_cast<QProgressBar*>(ui->tableWidgetSelectedDevices->cellWidget(row, 1));
        QLabel       *statusLbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 2));
        QLabel       *resultLbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 3));
        if (success) {
            if (bar)       bar->setValue(100);
            if (statusLbl) { statusLbl->setText(getStatusText(Success)); statusLbl->setStyleSheet(getStatusStyle(Success)); }
            if (resultLbl) { resultLbl->setText("升级成功"); resultLbl->setStyleSheet("QLabel { color: green; }"); }
            ++m_successCount;
        } else {
            if (statusLbl) { statusLbl->setText(getStatusText(Failed)); statusLbl->setStyleSheet(getStatusStyle(Failed)); }
            if (resultLbl) { resultLbl->setText(message); resultLbl->setStyleSheet("QLabel { color: red; }"); }
            ++m_failCount;
        }
    }
    // 设备完成日志直接写入 LoggerTableWidget
    LoggerTableWidget::instance()->log(
        success ? LogLevel::Info : LogLevel::Error,
        {{"QRCode", qrcode},
         {"Phase",  QStringLiteral("完成")},
         {"Message", message}});
}

// void FirmwareUpdateWidget::onRoundStarted(int round, int totalRounds)
// {
//     ui->labDToBeUpdate->setText(
//         QString("第 %1 / %2 轮 — 升级中...").arg(round).arg(totalRounds));
//     for (int row = 0; row < ui->tableWidgetSelectedDevices->rowCount(); ++row) {
//         QProgressBar *bar       = qobject_cast<QProgressBar*>(ui->tableWidgetSelectedDevices->cellWidget(row, 1));
//         QLabel       *statusLbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 2));
//         QLabel       *resultLbl = qobject_cast<QLabel*>(ui->tableWidgetSelectedDevices->cellWidget(row, 3));
//         if (bar)       bar->setValue(0);
//         if (statusLbl) { statusLbl->setText(getStatusText(Waiting)); statusLbl->setStyleSheet(getStatusStyle(Waiting)); }
//         if (resultLbl) { resultLbl->setText(QString("第 %1/%2 轮").arg(round).arg(totalRounds)); resultLbl->setStyleSheet(""); }
//     }
//     m_completedDevices = 0;
//     updateProgressBar();
// }

// void FirmwareUpdateWidget::onRoundFinished(int round, int totalRounds, bool success, const QString &message)
// {
//     ui->labDToBeUpdate->setText(
//         QString("第 %1 / %2 轮 — %3")
//         .arg(round).arg(totalRounds)
//         .arg(success ? "成功" : "失败"));
//     LoggerTableWidget::instance()->log(
//         success ? LogLevel::Info : LogLevel::Error,
//         {{"QRCode", "ALL"},
//          {"Phase",  QStringLiteral("轮次汇总")},
//          {"Message", QString("[第%1/%2轮] %3").arg(round).arg(totalRounds).arg(message)}});
//     captureTableWidgetScreenshot();
// }

void FirmwareUpdateWidget::onTaskAllProgress(int completed, int total)
{
    m_completedDevices = completed;
    m_totalDevices     = total;
    updateProgressBar();

    if (completed >= total && total > 0) {
        QString summary;
        summary += QString("Firmware update finished.\n\n");
        summary += QString("Total:   %1 device(s)\n").arg(total);
        summary += QString("Success: %1 device(s)\n").arg(m_successCount);
        summary += QString("Failed:  %1 device(s)").arg(m_failCount);

        if (m_failCount == 0) {
            QMessageBox::information(this, "Update Complete", summary);
        } else {
            QMessageBox::warning(this, "Update Complete (with failures)", summary);
        }
    }
}

// ======================================================
// 截图功能
// ======================================================

void FirmwareUpdateWidget::setCaptureDirectory(const QString &dirPath)
{
    m_captureDirectory = dirPath;
}

QString FirmwareUpdateWidget::captureDirectory() const
{
    return m_captureDirectory;
}

bool FirmwareUpdateWidget::ensureCaptureDirectoryExists()
{
    QDir dir(m_captureDirectory);
    if (!dir.exists()) {
        return dir.mkpath(".");  // 递归创建目录
    }
    return true;
}

void FirmwareUpdateWidget::captureTableWidgetScreenshot()
{
    if (!ensureCaptureDirectoryExists()) {
        qDebug() << "[FirmwareUpdateWidget] 无法创建截图目录:" << m_captureDirectory;
        return;
    }

    QTableWidget *table = ui->tableWidgetSelectedDevices;
    if (table->rowCount() == 0) return;

    // --- 计算完整内容尺寸（使用表格实际行高/列宽，不受 viewport 限制）---
    int headerH  = table->horizontalHeader()->isVisible()
                   ? table->horizontalHeader()->height() : 0;
    int vHeaderW = table->verticalHeader()->isVisible()
                   ? table->verticalHeader()->width() : 0;

    QVector<int> colW(table->columnCount());
    int totalW = vHeaderW;
    for (int c = 0; c < table->columnCount(); c++) {
        colW[c] = table->columnWidth(c);
        totalW += colW[c];
    }

    QVector<int> rowH(table->rowCount());
    int contentH = 0;
    for (int r = 0; r < table->rowCount(); r++) {
        rowH[r]   = table->rowHeight(r);
        contentH += rowH[r];
    }
    int totalH = headerH + contentH;

    // --- 构建全尺寸 Pixmap ---
    QPixmap fullPixmap(totalW, totalH);
    fullPixmap.fill(table->palette().color(QPalette::Base));
    QPainter painter(&fullPixmap);

    // 绘制表头
    if (headerH > 0) {
        table->horizontalHeader()->render(
            &painter, QPoint(vHeaderW, 0), QRegion(),
            QWidget::DrawWindowBackground | QWidget::DrawChildren);
    }

    // 逐行逐列渲染 cellWidget（直接从 widget 对象绘制，不依赖 viewport）
    int yOff = headerH;
    const QColor gridColor = table->palette().color(QPalette::Mid);
    for (int row = 0; row < table->rowCount(); row++) {
        int xOff = vHeaderW;
        for (int col = 0; col < table->columnCount(); col++) {
            QWidget *w = table->cellWidget(row, col);
            if (w) {
                w->render(&painter, QPoint(xOff, yOff), QRegion(),
                          QWidget::DrawWindowBackground | QWidget::DrawChildren);
            } else {
                QTableWidgetItem *item = table->item(row, col);
                if (item) {
                    painter.drawText(
                        QRect(xOff + 4, yOff, colW[col] - 4, rowH[row]),
                        Qt::AlignVCenter | Qt::AlignLeft, item->text());
                }
            }
            xOff += colW[col];
        }
        // 行分隔线
        painter.setPen(gridColor);
        painter.drawLine(0, yOff + rowH[row] - 1, totalW, yOff + rowH[row] - 1);
        yOff += rowH[row];
    }

    // 列分隔线
    painter.setPen(gridColor);
    int xLine = vHeaderW;
    for (int c = 0; c < table->columnCount() - 1; c++) {
        xLine += colW[c];
        painter.drawLine(xLine, headerH, xLine, totalH);
    }

    painter.end();

    // --- 保存文件 ---
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString fileName  = QString("firmware_update_%1.png").arg(timestamp);
    QString filePath  = QDir(m_captureDirectory).filePath(fileName);

    if (fullPixmap.save(filePath, "PNG")) {
        qDebug() << "[FirmwareUpdateWidget] 截图已保存"
                 << QString("(%1x%2)").arg(totalW).arg(totalH) << filePath;
    } else {
        qDebug() << "[FirmwareUpdateWidget] 截图保存失败:" << filePath;
    }
}
