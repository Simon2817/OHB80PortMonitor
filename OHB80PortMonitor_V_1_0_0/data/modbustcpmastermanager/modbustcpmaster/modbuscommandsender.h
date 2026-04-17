#ifndef MODBUSCOMMANDSENDER_H
#define MODBUSCOMMANDSENDER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QTcpSocket>
#include <QMutex>
#include "modbuscommand.h"
#include "modbuscommandreceiver.h"

// ============================================================
// ModbusCommandSender - Modbus 指令发送器
//
// 负责管理多优先级指令队列并串行下发指令。
// 优先级（高 → 低）：重发队列 > 初始下发 > 业务下发 > 定时发送
// ============================================================
class ModbusCommandSender : public QObject
{
    Q_OBJECT

    friend class ModbusTcpMaster;  // 允许 ModbusTcpMaster 访问私有方法

public:
    static constexpr int DEFAULT_QUEUE_CAPACITY   = 32;  // 每队列默认最大容量
    static constexpr int DEFAULT_MAX_RETRY         = 3;   // 默认最大重发次数

    explicit ModbusCommandSender(QTcpSocket& socket, const QString& masterId = QString(), QObject* parent = nullptr);

    // 禁用拷贝和赋值（引用成员不支持）
    ModbusCommandSender(const ModbusCommandSender&) = delete;
    ModbusCommandSender& operator=(const ModbusCommandSender&) = delete;

    // 提交一条指令（根据 cmd.module 自动路由到对应优先级队列）
    void submit(const ModbusCommand& cmd);

    // 设置每个队列的最大容量
    void setQueueCapacity(int capacity);

signals:
    // 指令发送完毕信号（无论成功或失败）
    void commandFinished(ModbusCommand cmd);

private slots:
    void dispatch();  // 调度一次：取出优先级最高的就绪指令并发送
    void onReceiverSucceeded(ModbusCommand cmd);
    void onReceiverFailed(ModbusCommand cmd, bool timedOut, bool checksumError);

private:
    // 启动调度循环
    void start();

    // 停止调度循环
    void stop();

    ModbusCommandReceiver* receiver() const { return m_receiver; }

    // 队列状态
    struct QueueState {
        QQueue<ModbusCommand> queue;
    };

    void doSend(ModbusCommand cmd);                       // 统一下发（根据功能码查表）
    bool enqueue(QueueState& qs, const ModbusCommand& cmd);          // 入队（含容量检查）
    QByteArray buildRequestFrame(const ModbusCommand& cmd) const;
    void finishCurrentCommand(ModbusCommand cmd);
    void handleFailedCommand(ModbusCommand cmd, const QString& errorMessage, bool timedOut, bool checksumError);
    void addToRetryQueue(ModbusCommand cmd);
    static quint16 crc16(const QByteArray& data);

    QTcpSocket* m_socket = nullptr;
    ModbusCommandReceiver* m_receiver = nullptr;
    QString& m_masterId;                // Master 设备 ID（引用）

    // 四个优先级队列（优先级递减）
    QueueState m_retryState;     // 重发队列（优先级最高）
    QueueState m_initialState;   // 初始下发队列
    QueueState m_businessState;  // 业务下发队列
    QueueState m_periodicState;  // 定时发送队列（优先级最低）

    int m_queueCapacity = DEFAULT_QUEUE_CAPACITY;  // 每队列最大容量

    bool                 m_running = false;
    bool                 m_hasPendingCommand = false;
    ModbusCommand        m_pendingCommand;
    mutable QMutex       m_mutex;  // 线程安全互斥锁
};

#endif // MODBUSCOMMANDSENDER_H
