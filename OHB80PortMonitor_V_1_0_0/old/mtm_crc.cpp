#include "mtm_crc.h"

QByteArray MtmCrc::modbusCRC16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;

    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data.at(i));
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    // 低字节在前，高字节在后
    QByteArray result(2, 0);
    result[0] = static_cast<char>(crc & 0xFF);
    result[1] = static_cast<char>((crc >> 8) & 0xFF);
    return result;
}

QByteArray MtmCrc::calculate(const QString &method,
                              const QByteArray &data,
                              int crcBytes)
{
    Q_UNUSED(crcBytes)

    if (method.compare(QStringLiteral("ModbusCRC16"), Qt::CaseInsensitive) == 0) {
        return modbusCRC16(data);
    }

    // 未知方法，返回全0
    return QByteArray(crcBytes, '\x00');
}

bool MtmCrc::validate(const QByteArray &frame,
                       const QString &method,
                       int startOffset,
                       int crcBytes)
{
    if (frame.size() < startOffset + crcBytes + 1) {
        return false;
    }

    // 提取CRC计算范围: [startOffset, frame.size() - crcBytes)
    int dataLen = frame.size() - crcBytes - startOffset;
    if (dataLen <= 0) return false;

    QByteArray data = frame.mid(startOffset, dataLen);
    QByteArray expectedCrc = calculate(method, data, crcBytes);
    QByteArray actualCrc   = frame.right(crcBytes);

    return expectedCrc == actualCrc;
}
