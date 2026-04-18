#include "firmware_upgrade_task.h"
#include "modbus_tcp_master_manager.h"
#include "modbus_tcp_master.h"
#include "binfilereader.h"

FirmwareUpgradeTask::FirmwareUpgradeTask(const QStringList &qrcodes,
                                         const QString &binFilePath,
                                         QObject *parent)
    : SchedulerTask(parent)
    , m_qrcodes(qrcodes)
    , m_binFilePath(binFilePath)
{
}

FirmwareUpgradeTask::~FirmwareUpgradeTask()
{
    delete m_binFileReader;
}

// ======================================================
// start() — 构建 BinFileReader，异步读取 bin 文件
// ======================================================
void FirmwareUpgradeTask::start()
{
    setState(Running);
    m_stopped      = false;
    m_finishedCount = 0;
    m_totalCount   = 0;
    m_upgraderMap.clear();

    if (m_qrcodes.isEmpty()) {
        setState(Finished);
        emit finished(false, "没有待升级的设备");
        return;
    }

    if (m_binFilePath.isEmpty()) {
        setState(Failed);
        emit finished(false, "未指定固件文件路径");
        return;
    }

    // 构建共享 BinFileReader，按 {248, 256} 规则分包
    m_binFileReader = new BinFileReader(this);
    m_binFileReader->setPacketSize(QVector<int>{248, 256});

    connect(m_binFileReader, &BinFileReader::sigReadFinished,
            this, &FirmwareUpgradeTask::onBinFileReadFinished);

    m_binFileReader->readBinFile(m_binFilePath);
}

// ======================================================
// stop()
// ======================================================
void FirmwareUpgradeTask::stop()
{
    m_stopped = true;

    for (auto it = m_upgraderMap.constBegin(); it != m_upgraderMap.constEnd(); ++it) {
        MtmFirmwareUpgrader *upgrader = it.value();
        if (upgrader && upgrader->isRunning()) {
            QMetaObject::invokeMethod(upgrader, [upgrader]() {
                upgrader->stop();
            }, Qt::QueuedConnection);
        }
    }

    setState(Cancelled);
    emit finished(false, "固件升级任务已取消");
}

// ======================================================
// onBinFileReadFinished — bin 文件读取完毕后开始升级
// ======================================================
void FirmwareUpgradeTask::onBinFileReadFinished(bool success, const QString &errorMsg)
{
    if (m_stopped) return;

    if (!success) {
        setState(Failed);
        emit finished(false, QString("读取固件文件失败: %1").arg(errorMsg));
        return;
    }

    startUpgrading();
}

// ======================================================
// startUpgrading — 遍历 qrcode，给每个 master 启动升级
// ======================================================
void FirmwareUpgradeTask::startUpgrading()
{
    ModbusTcpMasterManager *manager = ModbusTcpMasterManager::instance();

    for (const QString &qrcode : m_qrcodes) {
        ModbusTcpMaster *master = manager->getMaster(qrcode);
        if (!master) {
            emit deviceFinished(qrcode, false, QString("设备 %1 不存在").arg(qrcode));
            continue;
        }

        MtmFirmwareUpgrader *upgrader = master->firmwareUpgrader();
        if (!upgrader) {
            emit deviceFinished(qrcode, false, QString("设备 %1 固件升级子模块不可用").arg(qrcode));
            continue;
        }

        m_upgraderMap.insert(qrcode, upgrader);
        m_totalCount++;

        upgrader->setBinFileReader(m_binFileReader);

        connect(upgrader, &MtmFirmwareUpgrader::stateChanged,
                this, &FirmwareUpgradeTask::onUpgraderStateChanged,
                Qt::QueuedConnection);
        connect(upgrader, &MtmFirmwareUpgrader::progress,
                this, &FirmwareUpgradeTask::onUpgraderProgress,
                Qt::QueuedConnection);
        connect(upgrader, &MtmFirmwareUpgrader::finished,
                this, &FirmwareUpgradeTask::onUpgraderFinished,
                Qt::QueuedConnection);

        // upgrader 属于 worker 线程，必须通过 QueuedConnection 在其线程中调用 start()
        // 直接调用会导致 QTimer 跨线程启动（未定义行为），超时永不触发
        QString filePath = m_binFilePath;
        QMetaObject::invokeMethod(upgrader, [upgrader, filePath]() {
            upgrader->start(filePath);
        }, Qt::QueuedConnection);
    }

    if (m_totalCount == 0) {
        setState(Failed);
        emit finished(false, "所有设备均无法启动固件升级");
    }
}

// ======================================================
// 单个设备信号处理
// ======================================================
void FirmwareUpgradeTask::onUpgraderStateChanged(MtmFirmwareUpgrader::UpgradeState state,
                                                  const QString &logMessage,
                                                  const QByteArray &frame)
{
    MtmFirmwareUpgrader *upgrader = qobject_cast<MtmFirmwareUpgrader*>(sender());
    QString qrcode = qrcodeOf(upgrader);
    if (qrcode.isEmpty()) return;

    emit deviceStateLog(qrcode, state, logMessage, frame);
}

void FirmwareUpgradeTask::onUpgraderProgress(int percent)
{
    MtmFirmwareUpgrader *upgrader = qobject_cast<MtmFirmwareUpgrader*>(sender());
    QString qrcode = qrcodeOf(upgrader);
    if (qrcode.isEmpty()) return;

    emit deviceProgress(qrcode, percent);
}

void FirmwareUpgradeTask::onUpgraderFinished(bool success,
                                              MtmFirmwareUpgrader::UpgradeState /*state*/,
                                              const QString &errorMessage)
{
    if (m_stopped) return;

    MtmFirmwareUpgrader *upgrader = qobject_cast<MtmFirmwareUpgrader*>(sender());
    QString qrcode = qrcodeOf(upgrader);
    if (qrcode.isEmpty()) return;

    // 断开该 upgrader 的信号，避免重复处理
    disconnect(upgrader, nullptr, this, nullptr);

    QString resultMsg = success
        ? QString("[%1] 固件升级成功").arg(qrcode)
        : QString("[%1] 固件升级失败: %2").arg(qrcode, errorMessage);

    emit deviceFinished(qrcode, success, resultMsg);

    m_finishedCount++;
    emit allProgress(m_finishedCount, m_totalCount);
    int overallPercent = (m_totalCount > 0) ? (m_finishedCount * 100 / m_totalCount) : 0;
    emit progress(overallPercent,
                  QString("已完成 %1/%2").arg(m_finishedCount).arg(m_totalCount));

    if (m_finishedCount >= m_totalCount) {
        setState(Finished);
        emit finished(true, QString("固件升级全部完成，共 %1 台设备").arg(m_totalCount));
    }
}

// ======================================================
// qrcodeOf — 根据 upgrader 指针反查 qrcode
// ======================================================
QString FirmwareUpgradeTask::qrcodeOf(MtmFirmwareUpgrader *upgrader) const
{
    if (!upgrader) return QString();
    return m_upgraderMap.key(upgrader, QString());
}
