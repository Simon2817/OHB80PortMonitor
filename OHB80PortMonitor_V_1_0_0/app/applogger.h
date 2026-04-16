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

private:
    bool m_initialized;
    QString m_logDir;           // 日志目录
};

#endif // APPLOGGER_H
