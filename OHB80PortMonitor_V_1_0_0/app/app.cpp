#include "app.h"
#include "appconfig.h"
#include "applogger.h"
#include "shareddata.h"
#include "qthelper.h"
#include "metatypes.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "scheduler/scheduler.h"
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
QString App::executableDir;
QString App::configDir;
QString App::debugLogDir;
QString App::userLogDir;
AppLogger* App::s_logger = nullptr;

bool App::initialize()
{
    if (s_initialized) {
        return true;
    }
    
    qDebug() << "Initializing application...";
    
    // 注册元类型
    MetaTypes::registerTypes();

    // 获取并缓存路径信息
    QString executablePath = QCoreApplication::applicationFilePath();
    executableDir = QFileInfo(executablePath).absolutePath();  // bin/x32 或 bin/x64
    
    // config、log、data目录在bin目录下（executableDir的上一级）
    QDir binDir(executableDir);
    binDir.cdUp();  // 返回上一级目录，即bin目录
    QString binPath = binDir.absolutePath();
    configDir = binPath + "/config";
    debugLogDir = binPath + "/log/debug";
    userLogDir = binPath + "/log/user";
    
    // 单实例检查
    if (!QtHelper::checkSingleInstance(getAppName())) {
        return false;
    }
    
    // 连接应用程序退出信号
    QObject::connect(qApp, &QApplication::aboutToQuit, []() {
        App::onAboutToQuit();
    });
    
    // 初始化目录
    if (!initializeDirectories()) {
        qCritical() << "Failed to initialize directories";
        return false;
    }
    
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
    qDebug() << "Config Dir:" << configDir;
    qDebug() << "Debug Log Dir:" << debugLogDir;
    qDebug() << "User Log Dir:" << userLogDir;
    
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

bool App::initializeDirectories()
{
    // 检查并创建必要的目录
    QStringList dirs = {
        executableDir,
        configDir,
        debugLogDir,
        userLogDir
    };
    
    for (const QString& dir : dirs) {
        QDir directory(dir);
        if (!directory.exists()) {
            if (!directory.mkpath(".")) {
                qCritical() << "Failed to create directory:" << dir;
                return false;
            }
            qDebug() << "Created directory:" << dir;
        }
    }
    
    return true;
}

bool App::initializeLogging()
{
    // 创建并初始化日志管理器
    if (!s_logger) {
        s_logger = new AppLogger();
    }
    
    // 初始化日志系统（使用调试日志目录）
    if (!s_logger->initialize(debugLogDir)) {
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

    qDebug() << "调度器已启动";
}

void App::onAboutToQuit()
{
    cleanup();
}
