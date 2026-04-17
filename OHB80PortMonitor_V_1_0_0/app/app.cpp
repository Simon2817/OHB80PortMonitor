#include "app.h"
#include "appconfig.h"
#include "applogger.h"
#include "shareddata.h"
#include "qthelper.h"
#include "metatypes.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "scheduler/scheduler.h"
#include "scheduler/tasks/init_check_task.h"
#include "scheduler/tasks/network_status_task.h"
#include <qdir>
#include <qstandardpaths>
#include <qdebug>
#include <qfile>
#include <qtextstream>
#include <qdatetime>
#include <qicon>

// 静态成员变量定义
bool App::s_initialized = false;
QString App::s_appVersion = "1.0.0";
AppLogger* App::s_logger = nullptr;

bool App::initialize()
{
    if (s_initialized) {
        return true;
    }
    
    qDebug() << "Initializing application...";
    
    // 注册元类型
    MetaTypes::registerTypes();

    // 单实例检查
    if (!QtHelper::checkSingleInstance(getAppName())) {
        return false;
    }
    
    // 连接应用程序退出信号
    QObject::connect(qApp, &QApplication::aboutToQuit, []() {
        App::onAboutToQuit();
    });
    
    // 初始化日志
    if (!initializeLogging()) {
        qCritical() << "Failed to initialize logging";
        return false;
    }
    
    // ModbusTcpMasterManager加载配置文件
    if (!ModbusTcpMasterManager::instance().loadConfig(AppConfig::getInstance().getModbusConfigPath())) {
        qWarning() << "ModbusTcpMasterConfig.xml 配置文件加载失败，使用默认配置";
    }

    // 初始化共享数据
    getSharedData();

    // 初始化并启动调度器线程
    initScheduler();

    s_initialized = true;
    
    qDebug() << "Application initialized successfully";
    qDebug() << "App Name:" << getAppName();
    qDebug() << "App Version:" << getAppVersion();
    qDebug() << "Config Dir:" << getConfig()->getConfigDir();
    qDebug() << "Debug Log Dir:" << getConfig()->getDebugLogDir();
    qDebug() << "User Log Dir:" << getConfig()->getUserLogDir();
    
    return true;
}

void App::cleanup()
{
    qDebug() << "Cleaning up application...";
    
    // 关闭日志系统
    if (s_logger) {
        s_logger->shutdown();
        delete s_logger;
        s_logger = nullptr;
    }
    
    // 释放单实例共享内存
    QtHelper::releaseInstance();
    
    // 保存配置
    AppConfig::getInstance().reload();
    
    s_initialized = false;
}

bool App::initializeLogging()
{
    // 创建并初始化日志管理器
    if (!s_logger) {
        s_logger = new AppLogger();
    }
    
    // 初始化日志系统（使用调试日志目录）
    if (!s_logger->initialize(getConfig()->getDebugLogDir())) {
        return false;
    }

    // 注册崩溃信号处理器
    s_logger->registerCrashHandler();
    
    return true;
}

QString App::getAppName()
{
    return AppConfig::getInstance().getAppName();
}

QString App::getAppVersion()
{
    return AppConfig::getInstance().getAppVersion();
}

QString App::getDisplayName()
{
    QString appName = getAppName();
    QString appVersion = getAppVersion();
    
    if (!appVersion.isEmpty()) {
        return appName + " V" + appVersion;
    }
    
    return appName;
}

void App::getSharedData()
{
    // 创建静态实例以触发 SharedData 的构造函数初始化
    static SharedData sharedDataInstance;
    qDebug() << "SharedData initialized successfully";
}

void App::initScheduler()
{
    // 启动调度器
    Scheduler* scheduler = Scheduler::instance();
    scheduler->start();

    // 提交网络状态监控任务（长驻任务）
    // NetworkStatusTask 内部会在设备启动前先创建并管理 InitCheckTask
    NetworkStatusTask* networkStatusTask = new NetworkStatusTask();
    scheduler->submitTask(networkStatusTask);

    qDebug() << "调度器已启动，已提交网络状态监控任务";
}

void App::onAboutToQuit()
{
    cleanup();
}
