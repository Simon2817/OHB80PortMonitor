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
// AlarmCode — 警报类型 + 具体子类型（占 3 位，格式：[类型(1)][子类型(2)]）
//
//   全局类型  (0xx): 与设备无关的系统级警报
//   硬件类型  (1xx): 某设备的硬件故障
//   软件类型  (2xx): 某设备的软件 / 算法异常
//
// 警报ID 完整格式（共 9 位）：[Level(1)][PortId(5)][AlarmCode(3)]
//   示例：makeAlarmId(AlarmLevel::Warn, 12001, AlarmCode::GlobalOverHumidity)
//         → 112001000
// ====================================================================
enum class AlarmCode : int {

    // ---- 全局类型 (Global, Category=0) ----
    GlobalOverHumidity       = 0,   // 超湿报警
    GlobalOverTemperature    = 1,   // 超温报警
    GlobalPowerFailure       = 2,   // 电源故障
    GlobalNetworkDisconnect  = 3,   // 网络断开
    GlobalStorageFull        = 4,   // 存储空间不足

    // ---- 硬件类型 (Hardware, Category=1) ----
    HardwareSensorFailure    = 100, // 传感器故障
    HardwareMotorOverload    = 101, // 电机过载
    HardwareValveFault       = 102, // 阀门故障
    HardwareCameraError      = 103, // 相机错误

    // ---- 软件类型 (Software, Category=2) ----
    SoftwareAlgorithmTimeout = 200, // 算法超时
    SoftwareConfigError      = 201, // 配置错误
    SoftwareDatabaseError    = 202, // 数据库错误
    SoftwareLicenseExpired   = 203  // 许可证过期
};

// ====================================================================
// 工具函数
// ====================================================================

// 构造 9 位警报 ID：[Level(1)][PortId(5)][AlarmCode(3)]
inline qint64 makeAlarmId(AlarmLevel level, int portId, AlarmCode code)
{
    return static_cast<qint64>(level)   * 100000000LL
         + static_cast<qint64>(portId)  * 1000LL
         + static_cast<int>(code);
}

// 从警报 ID 解析级别
inline AlarmLevel alarmIdLevel(qint64 id)
{
    return static_cast<AlarmLevel>(id / 100000000LL);
}

// 从警报 ID 解析 PortId
inline int alarmIdPortId(qint64 id)
{
    return static_cast<int>((id / 1000LL) % 100000LL);
}

// 从警报 ID 解析 AlarmCode（类型 + 子类型，3 位）
inline AlarmCode alarmIdCode(qint64 id)
{
    return static_cast<AlarmCode>(id % 1000LL);
}

// 从警报 ID 解析警报类型分类（0=全局, 1=硬件, 2=软件）
inline int alarmIdCategory(qint64 id)
{
    return static_cast<int>((id % 1000LL) / 100LL);
}

// 从警报 ID 解析具体子类型编号（00~99）
inline int alarmIdSubType(qint64 id)
{
    return static_cast<int>(id % 100LL);
}

// 将警报 ID 格式化为零填充 9 位字符串
inline QString alarmIdToString(qint64 id)
{
    return QString("%1").arg(id, 9, 10, QChar('0'));
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
    case AlarmCode::GlobalOverHumidity:       return QStringLiteral("超湿报警");
    case AlarmCode::GlobalOverTemperature:    return QStringLiteral("超温报警");
    case AlarmCode::GlobalPowerFailure:       return QStringLiteral("电源故障");
    case AlarmCode::GlobalNetworkDisconnect:  return QStringLiteral("网络断开");
    case AlarmCode::GlobalStorageFull:        return QStringLiteral("存储空间不足");
    case AlarmCode::HardwareSensorFailure:    return QStringLiteral("传感器故障");
    case AlarmCode::HardwareMotorOverload:    return QStringLiteral("电机过载");
    case AlarmCode::HardwareValveFault:       return QStringLiteral("阀门故障");
    case AlarmCode::HardwareCameraError:      return QStringLiteral("相机错误");
    case AlarmCode::SoftwareAlgorithmTimeout: return QStringLiteral("算法超时");
    case AlarmCode::SoftwareConfigError:      return QStringLiteral("配置错误");
    case AlarmCode::SoftwareDatabaseError:    return QStringLiteral("数据库错误");
    case AlarmCode::SoftwareLicenseExpired:   return QStringLiteral("许可证过期");
    default:                                  return QStringLiteral("未知警报");
    }
}
