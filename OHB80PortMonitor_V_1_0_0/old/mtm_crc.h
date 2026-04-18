#ifndef MTM_CRC_H
#define MTM_CRC_H

#include <QByteArray>
#include <QString>

class MtmCrc
{
public:
    // Modbus CRC16校验计算
    // data: 待校验数据
    // 返回: 2字节CRC值（低字节在前，高字节在后）
    static QByteArray modbusCRC16(const QByteArray &data);

    // 根据方法名称计算CRC
    // method: 校验方法名（如 "ModbusCRC16"）
    // data: 待校验数据
    // crcBytes: CRC字节数
    // 返回: CRC字节数据
    static QByteArray calculate(const QString &method,
                                const QByteArray &data,
                                int crcBytes);

    // 验证CRC是否正确
    // frame: 完整帧数据
    // method: 校验方法名
    // startOffset: CRC计算起始偏移（0-indexed）
    // crcBytes: CRC占用字节数
    static bool validate(const QByteArray &frame,
                         const QString &method,
                         int startOffset,
                         int crcBytes);
};

#endif // MTM_CRC_H
