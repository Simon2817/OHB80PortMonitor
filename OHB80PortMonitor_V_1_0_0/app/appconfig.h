#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>

// 前置声明
class NetworkConfig;
class QRCodeConfig;
class UserInfoConfig;

class AppConfig
{
public:
    static AppConfig& getInstance();
    
    // 应用信息
    QString getAppName() const;    // 获取应用名称
    QString getAppVersion() const; // 获取应用版本
    QString getOSType() const;     // 获取操作系统类型（x32/x64）
    
    // 路径信息
    QString getRootDir() const;        // 获取软件根目录（bin目录）
    QString getExecutableDir() const;  // 获取可执行文件目录
    QString getConfigDir() const;      // 获取配置文件目录
    QString getDebugLogDir() const;    // 获取调试日志目录
    QString getUserLogDir() const;     // 获取用户日志目录
    QString getGraphConfigPath() const; // 获取天车地图配置文件路径
    QString getNetworkConfigPath() const; // 获取网络配置文件路径
    QString getQRCodeConfigPath() const;  // 获取二维码配置文件路径
    QString getUserInfoConfigPath() const; // 获取用户信息配置文件路径
    QString getModbusConfigPath() const;  // 获取 Modbus 配置文件路径
    
    // 配置文件操作
    void reload();
    bool setValue(const QString& key, const QVariant& value);
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;

    // 配置类实例获取
    NetworkConfig& getNetworkConfig() const;
    QRCodeConfig& getQRCodeConfig() const;
    UserInfoConfig& getUserInfoConfig() const;

private:
    AppConfig();
    ~AppConfig() = default;
    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;
    
    void initializePaths();
    
    QSettings* m_settings;
    QString m_rootDir;
    QString m_executableDir;
    QString m_configDir;
    QString m_debugLogDir;
    QString m_userLogDir;
    
    mutable NetworkConfig* m_networkConfig;
    mutable QRCodeConfig* m_qrCodeConfig;
    mutable UserInfoConfig* m_userInfoConfig;
};

#endif // APPCONFIG_H
