#ifndef APPLOGGER_H
#define APPLOGGER_H

#include <QString>

class AppLogger
{
public:
    AppLogger();
    ~AppLogger();
    
    // 初始化日志系统
    bool initialize(const QString& logDir);
    
    // 关闭日志系统
    void shutdown();
    
    // 注册崩溃信号处理器
    void registerCrashHandler();
    
    // 崩溃信号处理器（静态方法，用于信号处理）
    static void crashHandler(int sig);
    
    // 获取日志目录
    QString getLogDir() const { return m_logDir; }
    
    // 日志路径管理方法
    // 获取 ModbusMaster 日志路径: modbus/deviceid_{id}
    static QString getModbusMasterLogPath(const QString& id);

    // 获取 Modbus 帧消息日志路径: modbus/frame_message/device_id_{id}
    static QString getModbusFrameMessageLogPath(const QString& id);

    // 获取数据层业务日志路径: modbus/data
    static QString getDataLayerLogPath();

    // 获取 ModbusMaster 日志路径（带扩展名）
    static QString getModbusMasterLogPath(const QString& id, const QString& extension);

private:
    bool m_initialized;
    QString m_logDir;           // 日志目录
};

#endif // APPLOGGER_H
