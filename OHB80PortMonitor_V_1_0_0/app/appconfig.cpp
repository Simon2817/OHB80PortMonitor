#include "appconfig.h"
#include <QCoreApplication>
#include <QDebug>

AppConfig& AppConfig::getInstance()
{
    static AppConfig instance;
    return instance;
}

AppConfig::AppConfig()
{
    initializePaths();
    
    QString configPath = m_configDir + "/app.ini";
    m_settings = new QSettings(configPath, QSettings::IniFormat);
    
    qDebug() << "Config file:" << configPath;
    qDebug() << "Root dir:" << m_rootDir;
    qDebug() << "Executable dir:" << m_executableDir;
    qDebug() << "Config dir:" << m_configDir;
    qDebug() << "Debug log dir:" << m_debugLogDir;
    qDebug() << "User log dir:" << m_userLogDir;
}

void AppConfig::initializePaths()
{
    // 获取可执行文件所在目录
    QString executablePath = QCoreApplication::applicationFilePath();
    m_executableDir = QFileInfo(executablePath).absolutePath();  // bin/x32 或 bin/x64
    
    // config、log、data目录在bin目录下（executableDir的上一级）
    QDir binDir(m_executableDir);
    binDir.cdUp();  // 返回上一级目录，即bin目录
    m_rootDir = binDir.absolutePath();
    
    m_configDir = m_rootDir + "/config";
    m_debugLogDir = m_executableDir + "/log";
    m_userLogDir = m_rootDir + "/log/user";
    
    // 确保目录存在
    QDir dir;
    if (!dir.exists(m_configDir)) {
        dir.mkpath(m_configDir);
    }
    if (!dir.exists(m_debugLogDir)) {
        dir.mkpath(m_debugLogDir);
    }
    if (!dir.exists(m_userLogDir)) {
        dir.mkpath(m_userLogDir);
    }
}

QString AppConfig::getAppName() const
{
    return getValue("Application/AppName", "Template").toString();
}

QString AppConfig::getAppVersion() const
{
    return getValue("Application/AppVersion", "1.0.0").toString();
}

QString AppConfig::getOSType() const
{
    return getValue("Application/OSType", "x64").toString();
}

QString AppConfig::getRootDir() const
{
    return m_rootDir;
}

QString AppConfig::getExecutableDir() const
{
    return m_executableDir;
}

QString AppConfig::getConfigDir() const
{
    return m_configDir;
}

QString AppConfig::getDebugLogDir() const
{
    return m_debugLogDir;
}

QString AppConfig::getUserLogDir() const
{
    return m_userLogDir;
}

void AppConfig::reload()
{
    if (m_settings) {
        delete m_settings;
    }
    
    QString configPath = m_configDir + "/app.ini";
    m_settings = new QSettings(configPath, QSettings::IniFormat);
}

bool AppConfig::setValue(const QString& key, const QVariant& value)
{
    if (!m_settings) return false;
    
    m_settings->setValue(key, value);
    m_settings->sync();
    return true;
}

QVariant AppConfig::getValue(const QString& key, const QVariant& defaultValue) const
{
    if (!m_settings) return defaultValue;
    
    return m_settings->value(key, defaultValue);
}
