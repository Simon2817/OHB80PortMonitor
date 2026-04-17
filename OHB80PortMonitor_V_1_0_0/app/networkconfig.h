#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QString>
#include <QVector>

struct OHBNetworkInfo
{
    QString ip;
    quint16 port;

    OHBNetworkInfo() : port(0) {}
    OHBNetworkInfo(const QString& ipAddr, quint16 portNum)
        : ip(ipAddr), port(portNum) {}
};

class NetworkConfig
{
public:
    static NetworkConfig& getInstance();

    QVector<OHBNetworkInfo> readNetworkConfig() const;
    bool writeNetworkConfig(const QVector<OHBNetworkInfo>& configs);

    QString getNetworkConfigPath() const;

private:
    NetworkConfig();
    ~NetworkConfig() = default;
    NetworkConfig(const NetworkConfig&) = delete;
    NetworkConfig& operator=(const NetworkConfig&) = delete;

    QString m_configFilePath;
};

#endif // NETWORKCONFIG_H
