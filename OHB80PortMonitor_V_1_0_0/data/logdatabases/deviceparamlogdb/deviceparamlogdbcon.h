#ifndef DEVICEPARAMLOGDBCON_H
#define DEVICEPARAMLOGDBCON_H

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include "deviceparamlogsqllogic.h"
#include "writesqldbcon.h"

namespace LogDB {

class DeviceParamLogDBCon : public QObject
{
    Q_OBJECT

public:
    // 必须传入外部 WriteSqlDBCon，本类不拥有其生命周期
    DeviceParamLogDBCon(const QString& databasePath, WriteSqlDBCon* externalWriteCon, QObject* parent = nullptr);
    ~DeviceParamLogDBCon();

    // 禁用默认构造、拷贝、赋值
    DeviceParamLogDBCon() = delete;
    DeviceParamLogDBCon(const DeviceParamLogDBCon&) = delete;
    DeviceParamLogDBCon& operator=(const DeviceParamLogDBCon&) = delete;

    // 初始化
    bool initialize();
    void cleanup();

    // 查询接口
    QList<QVariantMap> queryPageWithConditions(const QString& qrCode,
                                               const QString& startTime,
                                               const QString& endTime,
                                               int pageSize,
                                               int pageNumber);

    int queryTotalCount();

    int queryTotalCountWithConditions(const QString& qrCode,
                                      const QString& startTime,
                                      const QString& endTime);

    int queryMonthRange();

    // 插入接口
    // userPermission: 触发该采样的用户权限级别（UserPermission 枚举），
    //                 默认 0（UserPermission::Guest），兼容旧调用方
    void insertRecord(const QString& qrCode,
                      const QString& recordTime,
                      double inletPressure,
                      double outletPressure,
                      double inletFlow,
                      double humidity,
                      double temperature,
                      int foupStatus,
                      int userPermission = 0);

    // 删除接口
    void deleteByTimeRange(const QString& startTime, const QString& endTime);

private slots:
    void onWriteTaskCompleted(const WriteResult& result);

private:
    QThread* m_workerThread;
    DeviceParamLogSqlLogic* m_sqlLogic;
    WriteSqlDBCon* m_writeCon;
};

} // namespace LogDB

#endif // DEVICEPARAMLOGDBCON_H
