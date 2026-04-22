#ifndef QRCODECONFIG_H
#define QRCODECONFIG_H

#include <QString>
#include <QVector>

class QRCodeConfig
{
public:
    static QRCodeConfig& getInstance();

    QVector<QString> readQRCodeMapping() const;
    bool writeQRCodeMapping(const QVector<QString>& mappings);

    QVector<QString> readMasterDevices() const;
    bool writeMasterDevices(const QVector<QString>& masterDevices);

    QString getQRCodeConfigPath() const;

private:
    QRCodeConfig();
    ~QRCodeConfig() = default;
    QRCodeConfig(const QRCodeConfig&) = delete;
    QRCodeConfig& operator=(const QRCodeConfig&) = delete;

    QString m_configFilePath;
};

#endif // QRCODECONFIG_H
