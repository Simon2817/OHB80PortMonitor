#include "ohbdeviceconfig.h"
#include "appconfig.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QDebug>

OHBDeviceConfig& OHBDeviceConfig::getInstance()
{
    static OHBDeviceConfig instance;
    return instance;
}

OHBDeviceConfig::OHBDeviceConfig()
{
    QString configDir = AppConfig::getInstance().getConfigDir();
    m_configFilePath = configDir + "/ohb_device.ini";

    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
}

QString OHBDeviceConfig::getConfigPath() const
{
    return m_configFilePath;
}

QVector<OHBDeviceInfo> OHBDeviceConfig::readDevices() const
{
    QVector<OHBDeviceInfo> devices;

    QSettings settings(m_configFilePath, QSettings::IniFormat);

    for (int i = 1; i <= 80; ++i) {
        QString groupName = QString("OHB%1").arg(i);
        settings.beginGroup(groupName);
        QString qrCode = settings.value("QRCode", "").toString();
        QString ip = settings.value("Ip", "").toString();
        quint16 port = settings.value("Port", 0).toUInt();
        settings.endGroup();

        if (!qrCode.isEmpty() && !ip.isEmpty() && port > 0) {
            devices.append(OHBDeviceInfo(qrCode, ip, port));
        }
    }

    qDebug() << "OHBDeviceConfig: 读取了" << devices.size() << "个 OHB 设备配置";
    return devices;
}

bool OHBDeviceConfig::writeDevices(const QVector<OHBDeviceInfo>& devices)
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);

    // 先备份 MasterDevices 节，clear 时会被清除
    QStringList masterList = settings.value("MasterDevices/list").toStringList();

    settings.clear();

    if (!masterList.isEmpty()) {
        settings.setValue("MasterDevices/list", masterList.join(","));
    }

    for (int i = 0; i < devices.size(); ++i) {
        QString groupName = QString("OHB%1").arg(i + 1);
        settings.beginGroup(groupName);
        settings.setValue("QRCode", devices[i].qrCode);
        settings.setValue("Ip", devices[i].ip);
        settings.setValue("Port", devices[i].port);
        settings.endGroup();
    }

    settings.sync();

    qDebug() << "OHBDeviceConfig: 写入了" << devices.size() << "个 OHB 设备到" << m_configFilePath;
    return settings.status() == QSettings::NoError;
}

QVector<QString> OHBDeviceConfig::readQRCodes() const
{
    QVector<QString> qrCodes;

    QSettings settings(m_configFilePath, QSettings::IniFormat);

    for (int i = 1; i <= 80; ++i) {
        QString groupName = QString("OHB%1").arg(i);
        QString qrCode = settings.value(groupName + "/QRCode", "").toString();
        if (!qrCode.isEmpty()) {
            qrCodes.append(qrCode);
        }
    }

    qDebug() << "OHBDeviceConfig: 读取了" << qrCodes.size() << "个 QR 码";
    return qrCodes;
}

QVector<QString> OHBDeviceConfig::readMasterDevices() const
{
    QVector<QString> masterDevices;

    QSettings settings(m_configFilePath, QSettings::IniFormat);

    settings.beginGroup("MasterDevices");
    // QSettings IniFormat 会将逗号分隔值自动解析为 QStringList，
    // 直接使用 toStringList() 同时兼容单值和多值两种情况
    QStringList items = settings.value("list").toStringList();
    settings.endGroup();

    qDebug() << "OHBDeviceConfig::readMasterDevices: 文件路径=" << m_configFilePath
             << "文件存在=" << QFile::exists(m_configFilePath)
             << "解析条数=" << items.size();

    for (const QString& item : items) {
        // 兼容中文逗号混入同一条目的情况
        const QStringList subItems = QString(item).replace(QString::fromUtf8("，"), ",")
                                         .split(',', Qt::SkipEmptyParts);
        for (const QString& sub : subItems) {
            QString deviceId = sub.trimmed();
            if (!deviceId.isEmpty()) {
                masterDevices.append(deviceId);
            }
        }
    }

    qDebug() << "OHBDeviceConfig: 读取了" << masterDevices.size() << "个主设备";
    return masterDevices;
}

bool OHBDeviceConfig::writeMasterDevices(const QVector<QString>& masterDevices)
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

    qDebug() << "OHBDeviceConfig: 写入了" << masterDevices.size() << "个主设备到" << m_configFilePath;
    return settings.status() == QSettings::NoError;
}

OHBDeviceInfo OHBDeviceConfig::getDeviceByQRCode(const QString& qrCode) const
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);

    for (int i = 1; i <= 80; ++i) {
        QString groupName = QString("OHB%1").arg(i);
        settings.beginGroup(groupName);
        QString qr = settings.value("QRCode", "").toString();
        if (qr == qrCode) {
            QString ip = settings.value("Ip", "").toString();
            quint16 port = settings.value("Port", 0).toUInt();
            settings.endGroup();
            return OHBDeviceInfo(qr, ip, port);
        }
        settings.endGroup();
    }

    return OHBDeviceInfo();
}

OHBDeviceInfo OHBDeviceConfig::getDeviceByMasterId(const QString& masterId) const
{
    QSettings settings(m_configFilePath, QSettings::IniFormat);
    settings.beginGroup(masterId);
    QString qrCode = settings.value("QRCode", "").toString();
    QString ip = settings.value("Ip", "").toString();
    quint16 port = settings.value("Port", 0).toUInt();
    settings.endGroup();

    return OHBDeviceInfo(qrCode, ip, port);
}
