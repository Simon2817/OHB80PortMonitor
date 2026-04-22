#include "communicatepage.h"
#include "customlogger.h"
#include "ui_communicatepage.h"
#include "app/shareddata.h"
#include "scheduler/tasks/monitor_data_task.h"
#include "modbustcpmastermanager/modbustcpmastermanager.h"
#include "modbustcpmastermanager/modbuscommand/commandpool.h"

#include <QJsonObject>
#include <QDateTime>

CommunicatePage::CommunicatePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CommunicatePage)
{
    ui->setupUi(this);
    initCommLoggerWidget();

    // 连接 MonitorDataTask 的通讯完成信号
    MonitorDataTask* monitorTask = SharedData::getMonitorDataTask();
    if (monitorTask) {
        connect(monitorTask, &MonitorDataTask::communicationCompleted,
                this, &CommunicatePage::onCommunicationCompleted,
                Qt::QueuedConnection);
    }
}

CommunicatePage::~CommunicatePage()
{
    delete ui;
}

void CommunicatePage::initCommLoggerWidget()
{
    // 配置日志根目录（表头由 CommunicateLoggerWidget 内部注册）
    ui->commLoggerWidget->setRootPath(CustomLogger::CommunicationLoggerPath());

    ui->commLoggerWidget->setPageSize(100);

    // 单个 CSV 分片大小上限（字节）；超过即自动滚动新文件，便于后续按分片并发读取与剪枝
    // 值越小：分片越多，读取越并行，但小文件数量增加；5MB 为经验折中
    ui->commLoggerWidget->setMaxFileBytes(5 * 1024 * 1024);

    // 预填充实时表格（每个 qrCode 一行），同时设置 qrcode 查询范围
    ui->commLoggerWidget->initQrcodeList(SharedData::getAllQrcodes());

    // 填充 CommandId 下拉框（用于历史查询）
    if (CommandPool *pool = ModbusTcpMasterManager::instance().commandPool()) {
        ui->commLoggerWidget->setCommandIds(pool->ids());
    }

    ui->commLoggerWidget->initialize();
}

void CommunicatePage::onCommunicationCompleted(ModbusCommand cmd, QString masterId)
{
    // 计算响应时间（ms）：发送到接收的时间差
    qint64 responseTimeMs = 0;
    if (cmd.sentMs > 0 && cmd.responseMs > 0) {
        responseTimeMs = cmd.responseMs - cmd.sentMs;
    }

    // 将原始数据帧转为十六进制字符串
    QString sendFrameHex = cmd.request.rawBytes.toHex(' ').toUpper();
    QString recvFrameHex = cmd.response.rawBytes.toHex(' ').toUpper();

    // 将发送时刻格式化为可读字符串
    QString sentTimeStr = cmd.sentMs > 0
        ? QDateTime::fromMSecsSinceEpoch(cmd.sentMs).toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"))
        : QStringLiteral("-");

    // 写入日志（key 必须匹配 CommunicateLoggerWidget::kLiveHeaders）
    ui->commLoggerWidget->writeLog(QJsonObject{
        {QStringLiteral("qrcode"),     masterId},
        {QStringLiteral("Time"),       sentTimeStr},
        {QStringLiteral("CommandId"),  cmd.id},
        {QStringLiteral("DurationMs"), QString::number(responseTimeMs)},
        {QStringLiteral("Request"),    sendFrameHex},
        {QStringLiteral("Response"),   recvFrameHex}
    });
}
