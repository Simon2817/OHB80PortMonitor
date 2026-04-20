#include "customlogger.h"
#include "appconfig.h"
#include <QDir>

QString CustomLogger::FirmwareUpgradeLoggerPath()
{
    AppConfig &config = AppConfig::getInstance();
    QString logDir = config.getRootDir() + "/log/firmware_upgrade";

    QDir dir(logDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return logDir;
}

QString CustomLogger::FirmwareUpgradeCapturePath()
{
    AppConfig &config = AppConfig::getInstance();
    QString captureDir = config.getRootDir() + "/log/firmware_upgrade/capture";

    QDir dir(captureDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return captureDir;
}
