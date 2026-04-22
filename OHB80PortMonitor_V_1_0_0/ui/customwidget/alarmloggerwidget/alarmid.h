#pragma once
#include <QString>

// ====================================================================
// AlarmLevel — 警报级别（占 1 位）
// ====================================================================
enum class AlarmLevel : int {
    Warn  = 1,
    Error = 2
};

// ====================================================================
// AlarmType — 警报分类（占 1 位）
// ====================================================================
enum class AlarmType : int {
    General = 0,  // 通用
    Hardware = 1, // 硬件
    Software = 2  // 软件
};

// ====================================================================
// AlarmCode — 警报级别 + 类型 + 子类型（占 4 位，格式：[Level(1)][Type(1)][SubType(2)]）
//
//   Level 位: 1=Warn, 2=Error
//   Type 位:  0=通用, 1=硬件, 2=软件
//   SubType:  00-99 具体子类型编号
//
// 警报ID 完整格式（共 10 位）：[Level(1)][PortId(5)][AlarmCode(4)]
//   示例：makeAlarmId(12001, AlarmCode::GeneralOverTemperature)
//         → 1120011002
//         → Level=1(Warn), PortId=12001, AlarmCode=1002(通用类型超温报警)
// ====================================================================
enum class AlarmCode : int {

    // ---- 通用类型 Warn (Level=1, Type=0) ----
    GeneralOverHumidity      = 1001, // 超湿报警
    GeneralOverTemperature   = 1002, // 超温报警

    // ---- 通用类型 Error (Level=2, Type=0) ----
    GeneralPowerFailure      = 2001, // 电源故障
    GeneralNetworkDisconnect = 2002, // 网络断开
    GeneralStorageFull       = 2003, // 存储空间不足

    // ---- 硬件类型 Error (Level=2, Type=1) ----
    HardwareSensorFailure    = 2101, // 传感器故障
    HardwareMotorOverload    = 2102, // 电机过载
    HardwareValveFault       = 2103, // 阀门故障
    HardwareCameraError      = 2104, // 相机错误

    // ---- 软件类型 Error (Level=2, Type=2) ----
    SoftwareAlgorithmTimeout = 2201, // 算法超时
    SoftwareConfigError      = 2202, // 配置错误
    SoftwareDatabaseError    = 2203, // 数据库错误
    SoftwareLicenseExpired   = 2204, // 许可证过期
    SoftwareConnectionLost   = 2205 // 连接断开
};

// ====================================================================
// 工具函数
// ====================================================================

// 从 AlarmCode 推导 AlarmLevel（第一位）
inline AlarmLevel alarmCodeLevel(AlarmCode code)
{
    int val = static_cast<int>(code);
    return static_cast<AlarmLevel>(val / 1000);
}

// 构造 10 位警报 ID：[Level(1)][PortId(5)][AlarmCode(4)]
// Level 根据 AlarmCode 自动推导
inline qint64 makeAlarmId(int portId, AlarmCode code)
{
    AlarmLevel level = alarmCodeLevel(code);
    return static_cast<qint64>(level)   * 1000000000LL
         + static_cast<qint64>(portId)  * 10000LL
         + static_cast<int>(code);
}

// 从警报 ID 解析级别
inline AlarmLevel alarmIdLevel(qint64 id)
{
    return static_cast<AlarmLevel>(id / 1000000000LL);
}

// 从警报 ID 解析 PortId
inline int alarmIdPortId(qint64 id)
{
    return static_cast<int>((id / 10000LL) % 100000LL);
}

// 从警报 ID 解析 AlarmCode（4 位）
inline AlarmCode alarmIdCode(qint64 id)
{
    return static_cast<AlarmCode>(id % 10000LL);
}

// 从警报 ID 解析警报类型分类（第二位：0=通用, 1=硬件, 2=软件）
inline int alarmIdCategory(qint64 id)
{
    return static_cast<int>((id % 10000LL) / 100LL) % 10;
}

// 从警报 ID 解析具体子类型编号（后两位：00~99）
inline int alarmIdSubType(qint64 id)
{
    return static_cast<int>(id % 100LL);
}

// 将警报 ID 格式化为零填充 10 位字符串
inline QString alarmIdToString(qint64 id)
{
    return QString("%1").arg(id, 10, 10, QChar('0'));
}

// 警报级别转字符串（与 LoggerWidget::MsgType 字符串保持一致）
inline QString alarmLevelToString(AlarmLevel level)
{
    switch (level) {
    case AlarmLevel::Warn:  return QStringLiteral("warn");
    case AlarmLevel::Error: return QStringLiteral("Error");
    default:                return QStringLiteral("unknown");
    }
}

// AlarmCode 转中文描述
inline QString alarmCodeToDescription(AlarmCode code)
{
    switch (code) {
    case AlarmCode::GeneralOverHumidity:      return QStringLiteral("超湿报警");
    case AlarmCode::GeneralOverTemperature:   return QStringLiteral("超温报警");
    case AlarmCode::GeneralPowerFailure:      return QStringLiteral("电源故障");
    case AlarmCode::GeneralNetworkDisconnect: return QStringLiteral("网络断开");
    case AlarmCode::GeneralStorageFull:       return QStringLiteral("存储空间不足");
    case AlarmCode::HardwareSensorFailure:    return QStringLiteral("传感器故障");
    case AlarmCode::HardwareMotorOverload:    return QStringLiteral("电机过载");
    case AlarmCode::HardwareValveFault:       return QStringLiteral("阀门故障");
    case AlarmCode::HardwareCameraError:      return QStringLiteral("相机错误");
    case AlarmCode::SoftwareAlgorithmTimeout: return QStringLiteral("算法超时");
    case AlarmCode::SoftwareConfigError:      return QStringLiteral("配置错误");
    case AlarmCode::SoftwareDatabaseError:    return QStringLiteral("数据库错误");
    case AlarmCode::SoftwareLicenseExpired:   return QStringLiteral("许可证过期");
    case AlarmCode::SoftwareConnectionLost: return QStringLiteral("连接断开");
    default:                                  return QStringLiteral("未知警报");
    }
}
