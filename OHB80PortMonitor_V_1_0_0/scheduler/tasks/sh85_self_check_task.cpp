#include "sh85_self_check_task.h"

#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/modbuscommandsender.h"
#include "modbustcpmastermanager/modbuscommand/commandpool.h"
#include "app/applogger.h"
#include "loggermanager.h"

#include <QDebug>

namespace {
constexpr const char *kCmdStart = "StartSelfCheck";
constexpr const char *kCmdRead  = "ReadSelfCheckStatus";

constexpr quint16 kStatusIdle             = 0;
constexpr quint16 kStatusInProgress       = 1;
constexpr quint16 kStatusSuccess          = 2;
constexpr quint16 kStatusHumidityFail     = 3;
constexpr quint16 kStatusSensorCommFail   = 4;
constexpr quint16 kStatusThresholdParam   = 5;
} // namespace

// ============================================================
// 构造 / 析构
// ============================================================

SH85SelfCheckTask::SH85SelfCheckTask(const QString &qrcode, QObject *parent)
    : SchedulerTask(parent)
    , m_qrcode(qrcode)
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] 创建任务: qrcode=" << qrcode;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 创建任务: qrcode=%1").arg(qrcode).toStdString());
}

SH85SelfCheckTask::~SH85SelfCheckTask()
{
    qDebug() << "[Scheduler][SH85SelfCheckTask] 任务销毁: qrcode=" << m_qrcode;
}

// ============================================================
// start()
// ============================================================

void SH85SelfCheckTask::start()
{
    setState(Running);
    m_stopped         = false;
    m_finishedEmitted = false;
    m_phase           = Phase::Idle;
    m_phase1Triggered = false;
    m_phase2Triggered = false;
    m_lastPhase2Sec   = -1;
    m_pendingMap.clear();
    m_connections.clear();

    if (m_qrcode.isEmpty()) {
        finishWith(false, Result::NetworkError, "Network Error");
        return;
    }

    if (!ensureMasterAndConnect()) {
        finishWith(false, Result::NetworkError, "Network Error");
        return;
    }

    if (!submitStartSelfCheck()) {
        finishWith(false, Result::NetworkError, "Network Error");
        return;
    }
    m_phase = Phase::StartingSelfCheck;

    // 启动 1Hz tick 与整体计时
    m_elapsed.start();
    if (!m_tickTimer) {
        m_tickTimer = new QTimer(this);
        m_tickTimer->setInterval(kCountdownTickMs);
        connect(m_tickTimer, &QTimer::timeout, this, &SH85SelfCheckTask::onTick);
    }
    m_tickTimer->start();

    // 立即发送一次倒计时初值
    emit countdownTick(kTotalDurationMs / 1000, m_qrcode);
}

// ============================================================
// stop()
// ============================================================

void SH85SelfCheckTask::stop()
{
    if (m_finishedEmitted) return;
    m_stopped = true;
    qDebug() << "[Scheduler][SH85SelfCheckTask] stop() 被调用: qrcode=" << m_qrcode;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 任务被取消: qrcode=%1").arg(m_qrcode).toStdString());
    finishWith(false, Result::Cancelled, "Cancelled");
}

// ============================================================
// 信号连接 / Master 校验
// ============================================================

bool SH85SelfCheckTask::ensureMasterAndConnect()
{
    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();

    CommandPool *pool = mgr.commandPool();
    if (!pool || !pool->contains(kCmdStart) || !pool->contains(kCmdRead)) {
        qWarning() << "[Scheduler][SH85SelfCheckTask] 指令池缺少 StartSelfCheck 或 ReadSelfCheckStatus";
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            "[Scheduler][SH85SelfCheckTask] 指令池缺少 StartSelfCheck 或 ReadSelfCheckStatus");
        return false;
    }

    ModbusTcpMaster *master = mgr.getMaster(m_qrcode);
    if (!master || !master->isConnected() || !master->sender()) {
        qWarning() << "[Scheduler][SH85SelfCheckTask] master 不可用 / 未连接: qrcode=" << m_qrcode;
        LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::WARN,
            QString("[Scheduler][SH85SelfCheckTask] 设备不可用: qrcode=%1").arg(m_qrcode).toStdString());
        return false;
    }

    auto conn = connect(master->sender(), &ModbusCommandSender::commandFinished,
                        this, &SH85SelfCheckTask::onCommandFinished,
                        Qt::QueuedConnection);
    m_connections.append(conn);
    return true;
}

// ============================================================
// 下发 StartSelfCheck（不允许超时重发）
// ============================================================

bool SH85SelfCheckTask::submitStartSelfCheck()
{
    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();
    ModbusTcpMaster *master = mgr.getMaster(m_qrcode);
    if (!master || !master->sender()) return false;

    CommandPool *pool = mgr.commandPool();
    ModbusCommand cmd = pool->clone(kCmdStart);
    if (!cmd.isValid()) return false;

    cmd.module        = CommandModule::BusinessCommandIssuer;
    cmd.maxRetryCount = 0;   // 关键：禁止超时重发

    m_pendingMap[cmd.uuid] = CmdKind::StartSelfCheck;

    qDebug() << "[Scheduler][SH85SelfCheckTask] 下发 StartSelfCheck (no retry) uuid=" << cmd.uuid;
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 下发 StartSelfCheck uuid=%1").arg(cmd.uuid).toStdString());

    ModbusCommandSender *sender = master->sender();
    QMetaObject::invokeMethod(sender, [sender, cmd]() {
        sender->submit(cmd);
    }, Qt::QueuedConnection);
    return true;
}

// ============================================================
// 下发 ReadSelfCheckStatus（阶段 1 / 阶段 2）
// ============================================================

bool SH85SelfCheckTask::submitReadStatus(CmdKind kind)
{
    ModbusTcpMasterManager &mgr = ModbusTcpMasterManager::instance();
    ModbusTcpMaster *master = mgr.getMaster(m_qrcode);
    if (!master || !master->sender()) return false;

    CommandPool *pool = mgr.commandPool();
    ModbusCommand cmd = pool->clone(kCmdRead);
    if (!cmd.isValid()) return false;

    cmd.module = CommandModule::BusinessCommandIssuer;

    // 阶段 1 前置确认快速失败：仅允许 1 次重试（初始 + 1 重试 ≈ 2s 内得出结论）
    // 阶段 2 轮询保留默认重试次数以容忍偶发抖动
    if (kind == CmdKind::ReadStatusEarly) {
        cmd.maxRetryCount = 1;
    }

    m_pendingMap[cmd.uuid] = kind;

    const char *kindStr = (kind == CmdKind::ReadStatusEarly) ? "EARLY" : "POLL";
    qDebug() << "[Scheduler][SH85SelfCheckTask] 下发 ReadSelfCheckStatus" << kindStr
             << "uuid=" << cmd.uuid << "elapsed=" << m_elapsed.elapsed() << "ms";
    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(), Level::INFO,
        QString("[Scheduler][SH85SelfCheckTask] 下发 ReadSelfCheckStatus[%1] uuid=%2 elapsed=%3ms")
            .arg(kindStr).arg(cmd.uuid).arg(m_elapsed.elapsed()).toStdString());

    ModbusCommandSender *sender = master->sender();
    QMetaObject::invokeMethod(sender, [sender, cmd]() {
        sender->submit(cmd);
    }, Qt::QueuedConnection);
    return true;
}

// ============================================================
// onTick — 1Hz 心跳，驱动倒计时与阶段切换
// ============================================================

void SH85SelfCheckTask::onTick()
{
    if (m_stopped || m_finishedEmitted) return;

    const qint64 ms = m_elapsed.elapsed();

    // 1) 倒计时
    emit countdownTick(remainingSecondsTotal(), m_qrcode);

    // 2) 阶段 1 触发：5s 起，仅触发一次（在 WaitingPhase1 阶段）
    if (!m_phase1Triggered && m_phase == Phase::WaitingPhase1 && ms >= kPhase1ProbeMs) {
        m_phase1Triggered = true;
        m_phase = Phase::ReadingStatusEarly;
        if (!submitReadStatus(CmdKind::ReadStatusEarly)) {
            finishWith(false, Result::NetworkError, "Network Error");
            return;
        }
    }

    // 3) 阶段 2 触发：60s 起，仅触发一次首发；之后 value==1 由响应处理重发
    if (!m_phase2Triggered && m_phase == Phase::WaitingPhase2 && ms >= kPhase2StartMs) {
        m_phase2Triggered = true;
        m_phase = Phase::PollingStatus;
        emit statusChanged(QString("Checking (%1)").arg(remainingSecondsPhase2()), m_qrcode);
        m_lastPhase2Sec = remainingSecondsPhase2();
        if (!submitReadStatus(CmdKind::ReadStatusPoll)) {
            finishWith(false, Result::NetworkError, "Network Error");
            return;
        }
    }

    // 4) 阶段 2 倒计时刷新："Checking (N)"
    if (m_phase == Phase::PollingStatus) {
        const int sec = remainingSecondsPhase2();
        if (sec != m_lastPhase2Sec) {
            m_lastPhase2Sec = sec;
            emit statusChanged(QString("Checking (%1)").arg(sec), m_qrcode);
        }
    }

    // 5) 整体兜底：仅当处于 PollingStatus 且当前没有等待中的响应时触发
    //    （若有未响应指令，等响应回来再判定，避免误杀刚回来的终态值）
    if (m_phase == Phase::PollingStatus && ms > kTotalDurationMs && m_pendingMap.isEmpty()) {
        finishWith(false, Result::SelfCheckFunctionAbnormal, "Self-check function abnormal");
        return;
    }
}

// ============================================================
// onCommandFinished
// ============================================================

void SH85SelfCheckTask::onCommandFinished(ModbusCommand cmd, const QString &masterId)
{
    if (m_stopped || m_finishedEmitted) return;
    if (masterId != m_qrcode) return;
    if (!m_pendingMap.contains(cmd.uuid)) return;

    const CmdKind kind = m_pendingMap.take(cmd.uuid);
    const bool ok = cmd.received && !cmd.timedOut && !cmd.checksumError && !cmd.deviceBusy;

    qDebug() << "[Scheduler][SH85SelfCheckTask] 响应 kind="
             << (kind == CmdKind::StartSelfCheck ? "Start"
               : kind == CmdKind::ReadStatusEarly ? "ReadEarly" : "ReadPoll")
             << "ok=" << ok
             << "timedOut=" << cmd.timedOut
             << "elapsed=" << m_elapsed.elapsed() << "ms";

    switch (kind) {
    // -------- StartSelfCheck --------
    case CmdKind::StartSelfCheck: {
        if (!ok) {
            finishWith(false, Result::NetworkError, "Network Error");
            return;
        }
        m_phase = Phase::WaitingPhase1;
        return;
    }

    // -------- 阶段 1 ReadSelfCheckStatus --------
    case CmdKind::ReadStatusEarly: {
        if (!ok) {
            finishWith(false, Result::NetworkError, "Network Error");
            return;
        }
        const quint16 v = parseStatusValue(cmd);
        if (v != kStatusInProgress) {
            qWarning() << "[Scheduler][SH85SelfCheckTask] 阶段 1 设备未进入自检 value=" << v;
            // 根据设备返回的值给出更具体的失败原因
            switch (v) {
            case kStatusHumidityFail:    finishWith(false, Result::HumidityExceeded,    "Humidity exceeded threshold"); return;
            case kStatusSensorCommFail:  finishWith(false, Result::SensorCommError,     "SH85 sensor comm error");      return;
            case kStatusThresholdParam:  finishWith(false, Result::ThresholdParamError, "Threshold parameter error");   return;
            default:                     finishWith(false, Result::DeviceNotEntered,    "Device not in self-check");    return;
            }
        }
        // value == 1，正常进入等待阶段 2
        m_phase = Phase::WaitingPhase2;
        return;
    }

    // -------- 阶段 2 ReadSelfCheckStatus 轮询 --------
    case CmdKind::ReadStatusPoll: {
        if (!ok) {
            finishWith(false, Result::NetworkError, "Network Error");
            return;
        }
        const quint16 v = parseStatusValue(cmd);
        switch (v) {
        case kStatusSuccess:
            finishWith(true,  Result::Success,             "Self-check OK");
            return;
        case kStatusHumidityFail:
            finishWith(false, Result::HumidityExceeded,    "Humidity exceeded threshold");
            return;
        case kStatusSensorCommFail:
            finishWith(false, Result::SensorCommError,     "SH85 sensor comm error");
            return;
        case kStatusThresholdParam:
            finishWith(false, Result::ThresholdParamError, "Threshold parameter error");
            return;
        case kStatusInProgress:
            // 仍在自检中：检查 70s 兜底，否则立即重发
            if (m_elapsed.elapsed() > kTotalDurationMs) {
                finishWith(false, Result::SelfCheckFunctionAbnormal, "Self-check function abnormal");
                return;
            }
            if (!submitReadStatus(CmdKind::ReadStatusPoll)) {
                finishWith(false, Result::NetworkError, "Network Error");
            }
            return;
        default:
            qWarning() << "[Scheduler][SH85SelfCheckTask] 阶段 2 收到未知 value=" << v;
            finishWith(false, Result::SelfCheckFunctionAbnormal, "Self-check function abnormal");
            return;
        }
    }
    }
}

// ============================================================
// 内部辅助
// ============================================================

quint16 SH85SelfCheckTask::parseStatusValue(const ModbusCommand &cmd) const
{
    const QByteArray &v = cmd.response.registerValue;
    if (v.size() < 2) return 0xFFFF;
    return (static_cast<quint16>(static_cast<quint8>(v[0])) << 8)
         |  static_cast<quint16>(static_cast<quint8>(v[1]));
}

int SH85SelfCheckTask::remainingSecondsTotal() const
{
    const qint64 ms = m_elapsed.isValid() ? m_elapsed.elapsed() : 0;
    const int rem = static_cast<int>((kTotalDurationMs - ms) / 1000);
    return rem < 0 ? 0 : rem;
}

int SH85SelfCheckTask::remainingSecondsPhase2() const
{
    return remainingSecondsTotal();   // 阶段 2 区间为 60~70s，剩余秒数即整体剩余
}

void SH85SelfCheckTask::disconnectAll()
{
    for (const QMetaObject::Connection &c : qAsConst(m_connections))
        QObject::disconnect(c);
    m_connections.clear();
}

void SH85SelfCheckTask::finishWith(bool success, Result result, const QString &uiText)
{
    if (m_finishedEmitted) return;
    m_finishedEmitted = true;

    if (m_tickTimer) m_tickTimer->stop();
    disconnectAll();
    m_pendingMap.clear();
    m_phase = Phase::Done;

    setState(success ? Finished : (result == Result::Cancelled ? Cancelled : Failed));

    emit statusChanged(uiText, m_qrcode);
    emit allFinished(success, result, m_qrcode);
    emit finished(success,
                  QString("SH85SelfCheckTask: qrcode=%1 result=%2 (%3)")
                      .arg(m_qrcode).arg(resultToText(result)).arg(uiText));

    LoggerManager::instance().log(AppLogger::SystemLoggerPath().toStdString(),
        success ? Level::INFO : Level::WARN,
        QString("[Scheduler][SH85SelfCheckTask] 任务结束 qrcode=%1 result=%2 ui='%3'")
            .arg(m_qrcode).arg(resultToText(result)).arg(uiText).toStdString());
}

QString SH85SelfCheckTask::resultToText(Result r)
{
    switch (r) {
    case Result::Success:                   return "Success";
    case Result::NetworkError:              return "NetworkError";
    case Result::DeviceNotEntered:          return "DeviceNotEntered";
    case Result::HumidityExceeded:          return "HumidityExceeded";
    case Result::SensorCommError:           return "SensorCommError";
    case Result::ThresholdParamError:       return "ThresholdParamError";
    case Result::SelfCheckFunctionAbnormal: return "SelfCheckFunctionAbnormal";
    case Result::Cancelled:                 return "Cancelled";
    }
    return "Unknown";
}
