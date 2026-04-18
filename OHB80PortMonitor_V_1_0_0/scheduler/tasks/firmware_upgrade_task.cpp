#include "firmware_upgrade_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "tool/binfilereader/binfilereader.h"
#include "loggermanager.h"
#include "app/applogger.h"

FirmwareUpgradeTask::FirmwareUpgradeTask(const QStringList &deviceIds,
                                         const QString &binFilePath,
                                         QObject *parent)
    : SchedulerTask(parent)
    , m_deviceIds(deviceIds)
    , m_binFilePath(binFilePath)
{
}

FirmwareUpgradeTask::~FirmwareUpgradeTask()
{
    delete m_binFileReader;
}

void FirmwareUpgradeTask::start()
{
    setState(Running);
    m_stopped      = false;
    m_finishedCount = 0;
    m_totalCount   = 0;
    m_upgraderMap.clear();

    qDebug() << "[Scheduler][FirmwareUpgradeTask][start] 固件升级任务启动";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
        "[Scheduler][FirmwareUpgradeTask][start] 固件升级任务启动");

    if (m_deviceIds.isEmpty()) {
        setState(Finished);
        qDebug() << "[Scheduler][FirmwareUpgradeTask][start] 没有待升级的设备";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN, 
            "[Scheduler][FirmwareUpgradeTask][start] 没有待升级的设备");
        emit finished(false, "没有待升级的设备");
        return;
    }

    if (m_binFilePath.isEmpty()) {
        setState(Failed);
        qDebug() << "[Scheduler][FirmwareUpgradeTask][start] 未指定固件文件路径";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::ERROR, 
            "[Scheduler][FirmwareUpgradeTask][start] 未指定固件文件路径");
        emit finished(false, "未指定固件文件路径");
        return;
    }

    // 构建共享 BinFileReader，按 {248, 256} 规则分包
    m_binFileReader = new BinFileReader(this);
    m_binFileReader->setPacketSize(QVector<int>{248, 256});

    connect(m_binFileReader, &BinFileReader::sigReadFinished,
            this, &FirmwareUpgradeTask::onBinFileReadFinished);

    qDebug() << "[Scheduler][FirmwareUpgradeTask][start] 开始读取固件文件:" << m_binFilePath;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
        QString("[Scheduler][FirmwareUpgradeTask][start] 开始读取固件文件: %1").arg(m_binFilePath).toStdString());
    
    m_binFileReader->readBinFile(m_binFilePath);
}

void FirmwareUpgradeTask::stop()
{
    m_stopped = true;

    qDebug() << "[Scheduler][FirmwareUpgradeTask][stop] 停止固件升级任务";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
        "[Scheduler][FirmwareUpgradeTask][stop] 停止固件升级任务");

    for (auto it = m_upgraderMap.constBegin(); it != m_upgraderMap.constEnd(); ++it) {
        FirmwareUpgrader *upgrader = it.value();
        if (upgrader && upgrader->isRunning()) {
            QMetaObject::invokeMethod(upgrader, [upgrader]() {
                upgrader->stop();
            }, Qt::QueuedConnection);
        }
    }

    setState(Cancelled);
    emit finished(false, "固件升级任务已取消");
}

void FirmwareUpgradeTask::onBinFileReadFinished(bool success, const QString &errorMsg)
{
    if (m_stopped) return;

    if (!success) {
        setState(Failed);
        QString msg = QString("读取固件文件失败: %1").arg(errorMsg);
        qDebug() << "[Scheduler][FirmwareUpgradeTask][onBinFileReadFinished]" << msg;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::ERROR, 
            QString("[Scheduler][FirmwareUpgradeTask][onBinFileReadFinished] %1").arg(msg).toStdString());
        emit finished(false, msg);
        return;
    }

    qDebug() << "[Scheduler][FirmwareUpgradeTask][onBinFileReadFinished] 固件文件读取成功，开始升级";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
        "[Scheduler][FirmwareUpgradeTask][onBinFileReadFinished] 固件文件读取成功，开始升级");
    
    startUpgrading();
}

void FirmwareUpgradeTask::startUpgrading()
{
    ModbusTcpMasterManager &manager = ModbusTcpMasterManager::instance();

    for (const QString &deviceId : m_deviceIds) {
        ModbusTcpMaster *master = manager.getMaster(deviceId);
        if (!master) {
            QString msg = QString("设备 %1 不存在").arg(deviceId);
            qDebug() << "[Scheduler][FirmwareUpgradeTask][startUpgrading]" << msg;
            LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN, 
                QString("[Scheduler][FirmwareUpgradeTask][startUpgrading] %1").arg(msg).toStdString());
            emit deviceFinished(deviceId, false, msg);
            continue;
        }

        FirmwareUpgrader *upgrader = master->firmwareUpgrader();
        if (!upgrader) {
            QString msg = QString("设备 %1 固件升级子模块不可用").arg(deviceId);
            qDebug() << "[Scheduler][FirmwareUpgradeTask][startUpgrading]" << msg;
            LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN, 
                QString("[Scheduler][FirmwareUpgradeTask][startUpgrading] %1").arg(msg).toStdString());
            emit deviceFinished(deviceId, false, msg);
            continue;
        }

        m_upgraderMap.insert(deviceId, upgrader);
        m_totalCount++;

        upgrader->setBinFileReader(m_binFileReader);

        connect(upgrader, &FirmwareUpgrader::stateChanged,
                this, &FirmwareUpgradeTask::onUpgraderStateChanged,
                Qt::QueuedConnection);
        connect(upgrader, &FirmwareUpgrader::progress,
                this, &FirmwareUpgradeTask::onUpgraderProgress,
                Qt::QueuedConnection);
        connect(upgrader, &FirmwareUpgrader::finished,
                this, &FirmwareUpgradeTask::onUpgraderFinished,
                Qt::QueuedConnection);

        // upgrader 属于 worker 线程，必须通过 QueuedConnection 在其线程中调用 start()
        QString filePath = m_binFilePath;
        QMetaObject::invokeMethod(upgrader, [upgrader, filePath]() {
            upgrader->start(filePath);
        }, Qt::QueuedConnection);

        qDebug() << "[Scheduler][FirmwareUpgradeTask][startUpgrading] 启动设备升级:" << deviceId;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
            QString("[Scheduler][FirmwareUpgradeTask][startUpgrading] 启动设备升级: %1").arg(deviceId).toStdString());
    }

    if (m_totalCount == 0) {
        setState(Failed);
        qDebug() << "[Scheduler][FirmwareUpgradeTask][startUpgrading] 所有设备均无法启动固件升级";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::ERROR, 
            "[Scheduler][FirmwareUpgradeTask][startUpgrading] 所有设备均无法启动固件升级");
        emit finished(false, "所有设备均无法启动固件升级");
    } else {
        qDebug() << "[Scheduler][FirmwareUpgradeTask][startUpgrading] 共启动" << m_totalCount << "台设备升级";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
            QString("[Scheduler][FirmwareUpgradeTask][startUpgrading] 共启动 %1 台设备升级").arg(m_totalCount).toStdString());
    }
}

void FirmwareUpgradeTask::onUpgraderStateChanged(const QString &masterId,
                                                  FirmwareUpgrader::UpgradeState state,
                                                  const QString &logMessage,
                                                  const QByteArray &frame)
{
    emit deviceStateLog(masterId, state, logMessage, frame);
}

void FirmwareUpgradeTask::onUpgraderProgress(const QString &masterId, int percent)
{
    emit deviceProgress(masterId, percent);
}

void FirmwareUpgradeTask::onUpgraderFinished(const QString &masterId,
                                              bool success,
                                              FirmwareUpgrader::UpgradeState /*state*/,
                                              const QString &errorMessage)
{
    if (m_stopped) return;

    FirmwareUpgrader *upgrader = m_upgraderMap.value(masterId, nullptr);
    if (!upgrader) return;

    // 断开该 upgrader 的信号，避免重复处理
    disconnect(upgrader, nullptr, this, nullptr);

    QString resultMsg = success
        ? QString("[%1] 固件升级成功").arg(masterId)
        : QString("[%1] 固件升级失败: %2").arg(masterId, errorMessage);

    qDebug() << "[Scheduler][FirmwareUpgradeTask][onUpgraderFinished]" << resultMsg;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), 
        success ? Level::INFO : Level::ERROR,
        QString("[Scheduler][FirmwareUpgradeTask][onUpgraderFinished] %1").arg(resultMsg).toStdString());

    emit deviceFinished(masterId, success, resultMsg);

    m_finishedCount++;
    emit allProgress(m_finishedCount, m_totalCount);
    
    int overallPercent = (m_totalCount > 0) ? (m_finishedCount * 100 / m_totalCount) : 0;
    emit progress(overallPercent,
                  QString("已完成 %1/%2").arg(m_finishedCount).arg(m_totalCount));

    if (m_finishedCount >= m_totalCount) {
        setState(Finished);
        QString msg = QString("固件升级全部完成，共 %1 台设备").arg(m_totalCount);
        qDebug() << "[Scheduler][FirmwareUpgradeTask][onUpgraderFinished]" << msg;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO, 
            QString("[Scheduler][FirmwareUpgradeTask][onUpgraderFinished] %1").arg(msg).toStdString());
        emit finished(true, msg);
    }
}
