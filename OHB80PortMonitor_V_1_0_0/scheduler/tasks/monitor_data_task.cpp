#include "monitor_data_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include "modbustcpmastermanager/modbustcpmaster/periodiccommandsender.h"
#include "app/shareddata.h"
#include "classes/setofohbinfo.h"
#include "classes/foupofohbinfo.h"

#include <QDebug>

MonitorDataTask::MonitorDataTask(QObject *parent)
    : SchedulerTask(parent)
{
}

void MonitorDataTask::start()
{
    setState(Running);
    m_stopped = false;
    m_totalCount = 0;

    ModbusTcpMasterManager& manager = ModbusTcpMasterManager::instance();
    QStringList ids = manager.masterIds();
    m_totalCount = ids.size();

    for (const QString& id : ids) {
        ModbusTcpMaster* master = manager.getMaster(id);
        if (!master) continue;

        PeriodicCommandSender* periodicSender = master->periodicSender();
        if (!periodicSender) {
            qWarning() << "[MonitorDataTask] 设备" << id << "的 PeriodicCommandSender 为空";
            continue;
        }

        // 直接连接 commandCompleted 信号（信号已携带 masterId）
        connect(periodicSender, &PeriodicCommandSender::commandCompleted,
                this, &MonitorDataTask::onCommandCompleted,
                Qt::QueuedConnection);
    }

    qDebug() << "[MonitorDataTask] 启动，监听" << m_totalCount << "个设备的 ReadFoupStatus 响应";
    emit progress(0, QString("开始监控 %1 个设备的实时数据").arg(m_totalCount));
}

void MonitorDataTask::stop()
{
    m_stopped = true;

    ModbusTcpMasterManager& manager = ModbusTcpMasterManager::instance();
    QStringList ids = manager.masterIds();
    for (const QString& id : ids) {
        ModbusTcpMaster* master = manager.getMaster(id);
        if (!master) continue;
        PeriodicCommandSender* periodicSender = master->periodicSender();
        if (periodicSender) {
            disconnect(periodicSender, &PeriodicCommandSender::commandCompleted,
                       this, &MonitorDataTask::onCommandCompleted);
        }
    }

    setState(Cancelled);
    emit finished(false, "监控任务被取消");
    qDebug() << "[MonitorDataTask] 已停止";
}

void MonitorDataTask::onCommandCompleted(ModbusCommand cmd, const QString& masterId)
{
    if (m_stopped) return;

    // 只处理 ReadFoupStatus 指令的成功响应
    if (cmd.id != "ReadFoupStatus") return;
    if (!cmd.received) return;

    // 解析响应数据
    QVariantMap data = parseReadFoupStatusResponse(cmd);
    if (data.isEmpty()) {
        qDebug() << "[MonitorDataTask] ReadFoupStatus 响应解析失败";
        return;
    }

    // 直接使用捕获的 masterId（qrCode）更新对应的 FoupOfOHBInfo
    updateFoupInfo(masterId, data);
}

void MonitorDataTask::updateFoupInfo(const QString& masterId, const QVariantMap& data)
{
    // 通过 SharedData 直接获取对应 qrCode 的 FoupOfOHBInfo 指针
    FoupOfOHBInfo* foup = SharedData::getFoupByQRCode(masterId);
    if (!foup) {
        qWarning() << "[MonitorDataTask] 未找到设备" << masterId << "对应的 FoupOfOHBInfo";
        return;
    }

    // 更新 Foup 数据（根据服务端真实数据格式）
    foup->inletPressure = data.value("inletPressure").toDouble();
    foup->inletFlow     = data.value("inFlow").toDouble();
    foup->RH            = data.value("humidity").toDouble();
    foup->foupIn        = data.value("hasFoup").toBool();
    
    // 更新时间相关字段
    quint32 startTimeSec = data.value("startTimeSec").toUInt();
    foup->startTime = QDateTime::fromSecsSinceEpoch(startTimeSec).time();
    
    foup->purgeTimeMs = data.value("purgeTimeMs").toUInt();
    foup->idleTimeMs  = data.value("idleTimeMs").toUInt();

    qDebug() << "[MonitorDataTask] 设备" << masterId
             << "数据已更新 压力=" << foup->inletPressure
             << "流量=" << foup->inletFlow
             << "湿度=" << foup->RH
             << "FoupIn=" << foup->foupIn
             << "StartTime=" << foup->startTime.toString()
             << "PurgeTime=" << foup->purgeTimeMs
             << "IdleTime=" << foup->idleTimeMs;
}

QVariantMap MonitorDataTask::parseReadFoupStatusResponse(const ModbusCommand& cmd) const
{
    QVariantMap result;

    // ReadFoupStatus 响应：9 个寄存器 = 18 字节（存放在 response.registerValue）
    const QByteArray& payload = cmd.response.registerValue;
    if (payload.size() < 18) {
        qWarning() << "[MonitorDataTask] ReadFoupStatus 响应字节数不足，实际=" << payload.size();
        return result;
    }

    // 辅助 lambda：读取大端 2 字节整数并除以 100.0（符合 Modbus 标准）
    auto getU16BEAsDouble = [&payload](int offset) -> double {
        if (offset + 1 >= payload.size()) return 0.0;
        quint16 raw = (static_cast<quint8>(payload.at(offset)) << 8)
                    | static_cast<quint8>(payload.at(offset + 1));
        return raw / 100.0;
    };

    // 辅助 lambda：读取大端 4 字节整数（时间戳）
    auto getU32BE = [&payload](int offset) -> quint32 {
        if (offset + 3 >= payload.size()) return 0;
        return (static_cast<quint8>(payload.at(offset)) << 24)
             | (static_cast<quint8>(payload.at(offset + 1)) << 16)
             | (static_cast<quint8>(payload.at(offset + 2)) << 8)
             | static_cast<quint8>(payload.at(offset + 3));
    };

    // 辅助 lambda：读取大端 2 字节整数（毫秒）
    auto getU16BE = [&payload](int offset) -> quint16 {
        if (offset + 1 >= payload.size()) return 0;
        return (static_cast<quint8>(payload.at(offset)) << 8)
             | static_cast<quint8>(payload.at(offset + 1));
    };

    // 服务端数据格式（假设符合 Modbus 标准使用大端序）：
    // 字节0-1: 进气压力（大端，/100.0）
    // 字节2-3: 进气流量（大端，/100.0）
    // 字节4-5: 相对湿度（大端，/100.0）
    // 字节6-9: 开始时间戳（大端，秒）
    // 字节10-11: 充气时间ms（大端）
    // 字节12-13: idle时间ms（大端）
    // 字节14-15: FoupIn状态（大端）
    // 字节16-19: 填充（0x00）

    // 1. 进气压力 (0.1-1.0 MPa)
    result["inletPressure"] = getU16BEAsDouble(0);

    // 2. 进气流量 (0-100 L/Min)
    result["inFlow"] = getU16BEAsDouble(2);

    // 3. 相对湿度 (0-100%)
    result["humidity"] = getU16BEAsDouble(4);

    // 4. 开始时间戳（秒）
    quint32 startTimeSec = getU32BE(6);
    result["startTimeSec"] = startTimeSec;

    // 5. 充气时间（毫秒）
    quint16 chargeTimeMs = getU16BE(10);
    result["purgeTimeMs"] = chargeTimeMs;

    // 6. idle时间（毫秒）
    quint16 idleTimeMs = getU16BE(12);
    result["idleTimeMs"] = idleTimeMs;

    // 7. FoupIn状态（0或1）
    quint16 foupInStatus = getU16BE(14);
    result["hasFoup"] = (foupInStatus != 0);

    return result;
}
