#ifndef FIRMWARE_UPGRADE_TASK_H
#define FIRMWARE_UPGRADE_TASK_H

#include "../scheduler_task.h"
#include "ModbusTcpMaster/mtm_firmware_upgrader.h"

#include <QStringList>
#include <QHash>

class BinFileReader;
class ModbusTcpMaster;

class FirmwareUpgradeTask : public SchedulerTask
{
    Q_OBJECT

public:
    explicit FirmwareUpgradeTask(const QStringList &qrcodes,
                                 const QString &binFilePath,
                                 QObject *parent = nullptr);
    ~FirmwareUpgradeTask() override;

    // SchedulerTask 接口
    void start() override;
    void stop() override;
    QString taskType() const override { return "FirmwareUpgradeTask"; }

signals:
    // 单个设备升级进度（0-100），用于更新表格进度条
    void deviceProgress(const QString &qrcode, int percent);

    // 单个设备状态日志，用于显示到 MultiModuleLogWidget
    void deviceStateLog(const QString &qrcode,
                        MtmFirmwareUpgrader::UpgradeState state,
                        const QString &logMessage,
                        const QByteArray &frame);

    // 单个设备升级完毕，用于最终结论显示
    void deviceFinished(const QString &qrcode, bool success, const QString &message);

    // 总体完成进度（已完成设备数 / 总设备数），用于刷新 progressBar
    void allProgress(int completed, int total);

private slots:
    void onBinFileReadFinished(bool success, const QString &errorMsg);
    void onUpgraderStateChanged(MtmFirmwareUpgrader::UpgradeState state,
                                const QString &logMessage,
                                const QByteArray &frame);
    void onUpgraderProgress(int percent);
    void onUpgraderFinished(bool success,
                            MtmFirmwareUpgrader::UpgradeState state,
                            const QString &errorMessage);

private:
    void startUpgrading();
    QString qrcodeOf(MtmFirmwareUpgrader *upgrader) const;

    QStringList   m_qrcodes;
    QString       m_binFilePath;
    BinFileReader *m_binFileReader = nullptr;

    // qrcode → upgrader 映射（用于信号槽中反查设备）
    QHash<QString, MtmFirmwareUpgrader*> m_upgraderMap;

    int  m_totalCount    = 0;
    int  m_finishedCount = 0;
    bool m_stopped       = false;
};

#endif // FIRMWARE_UPGRADE_TASK_H
