#ifndef APP_H
#define APP_H

#include <QObject>
#include <QApplication>
#include "appconfig.h"
#include "applogger.h"

class App
{
public:
    // 静态初始化和清理方法
    static bool initialize();
    static void cleanup();

    // 获取配置
    static AppConfig* getConfig() { return &AppConfig::getInstance(); }
    
    // 获取日志管理器
    static AppLogger* getLogger() { return s_logger; }
    
    // 应用信息
    static QString getAppName();        // 获取应用名称
    static QString getAppVersion();     // 获取应用版本
    static QString getDisplayName();    // 获取显示名称（名称 V版本号）
    
    // 路径信息（直接访问静态变量）
    static QString executableDir;       // 可执行文件目录
    static QString configDir;           // 配置文件目录
    static QString debugLogDir;         // 调试日志目录
    static QString userLogDir;         // 用户日志目录

private:
    static void onAboutToQuit();
    static bool initializeLogging();
    static bool initializeDirectories();
    
    static bool s_initialized;
    static QString s_appVersion;
    static AppLogger* s_logger;         // 日志管理器实例
};

#endif // APP_H
