#include "networkconfig.h"
#include "appconfig.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

NetworkConfig& NetworkConfig::getInstance()
{
    static NetworkConfig instance;
    return instance;
}

NetworkConfig::NetworkConfig()
{
    QString configDir = AppConfig::getInstance().getConfigDir();
    m_configFilePath = configDir + "/network.ini";
    
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
}

QString NetworkConfig::getNetworkConfigPath() const
{
    return m_configFilePath;
}

QVector<OHBNetworkInfo> NetworkConfig::readNetworkConfig() const
{
    QVector<OHBNetworkInfo> configs;
    
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    // 按数字顺序读取 OHB1 到 OHB80
    for (int i = 1; i <= 80; ++i) {
        QString groupName = QString("OHB%1").arg(i);
        settings.beginGroup(groupName);
        QString ip = settings.value("Ip", "").toString();
        quint16 port = settings.value("Port", 0).toUInt();
        settings.endGroup();
        
        if (!ip.isEmpty() && port > 0) {
            configs.append(OHBNetworkInfo(ip, port));
            qDebug() << "NetworkConfig: 读取配置 -" << groupName << "IP:" << ip << "Port:" << port;
        }
    }
    
    qDebug() << "NetworkConfig: 读取了" << configs.size() << "个OHB网络配置";
    return configs;
}

bool NetworkConfig::writeNetworkConfig(const QVector<OHBNetworkInfo>& configs)
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    settings.clear();
    
    for (int i = 0; i < configs.size(); ++i) {
        QString groupName = QString("OHB%1").arg(i + 1);
        settings.beginGroup(groupName);
        settings.setValue("Ip", configs[i].ip);
        settings.setValue("Port", configs[i].port);
        settings.endGroup();
    }
    
    settings.sync();
    
    qDebug() << "NetworkConfig: 写入了" << configs.size() << "个OHB网络配置到" << m_configFilePath;
    
    return settings.status() == QSettings::NoError;
}
