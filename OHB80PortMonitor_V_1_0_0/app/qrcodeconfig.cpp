#include "qrcodeconfig.h"
#include "appconfig.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

QRCodeConfig& QRCodeConfig::getInstance()
{
    static QRCodeConfig instance;
    return instance;
}

QRCodeConfig::QRCodeConfig()
{
    QString configDir = AppConfig::getInstance().getConfigDir();
    m_configFilePath = configDir + "/qrcode.ini";
    
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
}

QString QRCodeConfig::getQRCodeConfigPath() const
{
    return m_configFilePath;
}

QVector<QString> QRCodeConfig::readQRCodeMapping() const
{
    QVector<QString> mappings;
    
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    settings.beginGroup("QRCode");
    
    // 按数字顺序读取 OHB1 到 OHB80
    for (int i = 1; i <= 80; ++i) {
        QString key = QString("OHB%1").arg(i);
        QString qrCode = settings.value(key, "").toString();
        if (!qrCode.isEmpty()) {
            mappings.append(qrCode);
            qDebug() << "QRCodeConfig: 读取二维码映射 -" << key << "=" << qrCode;
        }
    }
    
    settings.endGroup();
    
    qDebug() << "QRCodeConfig: 读取了" << mappings.size() << "个OHB二维码映射";
    return mappings;
}

bool QRCodeConfig::writeQRCodeMapping(const QVector<QString>& mappings)
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    settings.remove("QRCode");
    
    settings.beginGroup("QRCode");
    for (int i = 0; i < mappings.size(); ++i) {
        QString key = QString("OHB%1").arg(i + 1);
        settings.setValue(key, mappings[i]);
    }
    settings.endGroup();
    
    settings.sync();
    
    qDebug() << "QRCodeConfig: 写入了" << mappings.size() << "个OHB二维码映射到" << m_configFilePath;
    
    return settings.status() == QSettings::NoError;
}

QVector<QString> QRCodeConfig::readMasterDevices() const
{
    QVector<QString> masterDevices;
    
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    settings.beginGroup("MasterDevices");
    QString listStr = settings.value("list", "").toString();
    settings.endGroup();
    
    if (!listStr.isEmpty()) {
        QStringList items = listStr.split(',', Qt::SkipEmptyParts);
        for (const QString& item : items) {
            QString deviceId = item.trimmed();
            if (!deviceId.isEmpty()) {
                masterDevices.append(deviceId);
                qDebug() << "QRCodeConfig: 读取主设备 -" << deviceId;
            }
        }
    }
    
    qDebug() << "QRCodeConfig: 读取了" << masterDevices.size() << "个主设备";
    return masterDevices;
}

bool QRCodeConfig::writeMasterDevices(const QVector<QString>& masterDevices)
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    
    QStringList items;
    for (const QString& device : masterDevices) {
        items.append(device);
    }
    QString listStr = items.join(",");
    
    settings.beginGroup("MasterDevices");
    settings.setValue("list", listStr);
    settings.endGroup();
    
    settings.sync();
    
    qDebug() << "QRCodeConfig: 写入了" << masterDevices.size() << "个主设备到" << m_configFilePath;
    
    return settings.status() == QSettings::NoError;
}
