#include "modbuscommand.h"
#include <QDebug>

// ============================================================
// ModbusCommand - Modbus 指令对象实现
// ============================================================

// 默认构造函数
ModbusCommand::ModbusCommand()
{
    // 默认初始化所有成员变量
}

// 拷贝构造函数
ModbusCommand::ModbusCommand(const ModbusCommand& other)
    : id(other.id),
      uuid(other.uuid),
      request(other.request),
      response(other.response),
      module(other.module),
      errorMessage(other.errorMessage),
      maxRetryCount(other.maxRetryCount),
      timeoutMs(other.timeoutMs),
      received(other.received),
      timedOut(other.timedOut),
      checksumError(other.checksumError),
      deviceBusy(other.deviceBusy),
      sendCount(other.sendCount),
      sentMs(other.sentMs),
      responseMs(other.responseMs)
{
    // 拷贝所有成员变量
}

// 拷贝赋值操作符
ModbusCommand& ModbusCommand::operator=(const ModbusCommand& other)
{
    if (this != &other) {
        id = other.id;
        uuid = other.uuid;
        request = other.request;
        response = other.response;
        module = other.module;
        errorMessage = other.errorMessage;
        maxRetryCount = other.maxRetryCount;
        timeoutMs = other.timeoutMs;
        received = other.received;
        timedOut = other.timedOut;
        checksumError = other.checksumError;
        deviceBusy = other.deviceBusy;
        sendCount = other.sendCount;
        sentMs = other.sentMs;
        responseMs = other.responseMs;
    }
    return *this;
}

QByteArray ModbusCommand::fromHexString(const QString& hexStr)
{
    QByteArray result;
    const QStringList tokens = hexStr.trimmed().split(' ', QString::SkipEmptyParts);
    for (const QString& token : tokens) {
        bool ok = false;
        const uint byte = token.toUInt(&ok, 16);
        if (ok) {
            result.append(static_cast<char>(byte & 0xFF));
        } else {
            qDebug() << "ModbusCommand: 无效的十六进制字符串片段:" << token;
        }
    }
    return result;
}

