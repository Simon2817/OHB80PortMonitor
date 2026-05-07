#ifndef ALARMLOGDBCON_H
#define ALARMLOGDBCON_H

#include <QObject>
#include <QThread>
#include <QVariantMap>
#include "alarmlogsqllogic.h"
#include "writesqldbcon.h"

namespace LogDB {

class AlarmLogDBCon : public QObject
{
    Q_OBJECT

public:
    // 必须传入外部 WriteSqlDBCon，本类不拥有其生命周期
    AlarmLogDBCon(const QString& databasePath, WriteSqlDBCon* externalWriteCon, QObject* parent = nullptr);
    ~AlarmLogDBCon();

    // 禁用默认构造、拷贝、赋值
    AlarmLogDBCon() = delete;
    AlarmLogDBCon(const AlarmLogDBCon&) = delete;
    AlarmLogDBCon& operator=(const AlarmLogDBCon&) = delete;

    // 初始化
    bool initialize();
    void cleanup();

    // 查询接口
    QList<QVariantMap> queryPageWithConditions(int alarmLevel,
                                               const QString& qrCode,
                                               const QString& alarmType,
                                               int isResolved,
                                               int customerVisible,
                                               const QString& startTime,
                                               const QString& endTime,
                                               int pageSize,
                                               int pageNumber);

    int queryTotalCount();

    int queryTotalCountWithConditions(int alarmLevel,
                                      const QString& qrCode,
                                      const QString& alarmType,
                                      int isResolved,
                                      int customerVisible,
                                      const QString& startTime,
                                      const QString& endTime);

    int queryMonthRange();

    // 查询数据库中 occur_time 的最早 / 最晚时间。
    // 输出为 "yyyy-MM-dd HH:mm:ss" 格式；表为空时返回两个空字符串。
    void queryTimeBounds(QString& earliestTime, QString& latestTime);

    // 插入接口
    void insertRecord(int alarmLevel,
                      const QString& occurTime,
                      const QString& qrCode,
                      const QString& alarmType,
                      int isResolved,
                      const QString& resolveTime,
                      int customerVisible,
                      const QString& description);

    // 删除接口
    void deleteByTimeRange(const QString& startTime, const QString& endTime);

    // 更新接口：把指定 (qrCode, alarmType) 下 is_resolved=0 的记录原位标记为已解决
    void updateResolve(const QString& qrCode, const QString& alarmType, const QString& resolveTime);

signals:
    // 实时事件：本 DBCon 提交的 INSERT 已成功落库
    // row 字段对应 alarm_log 表列：alarm_level / occur_time / qr_code / alarm_type /
    //   is_resolved / resolve_time / customer_visible / description
    void recordInserted(const QVariantMap& row);

    // 实时事件：本 DBCon 提交的 UPDATE(update_resolve) 已成功执行
    // 用于驱动 UI 把对应 (qr_code, alarm_type) 行的 is_resolved/resolve_time 列原位刷新
    void recordResolved(const QString& qrCode,
                        const QString& alarmType,
                        const QString& resolveTime);

private slots:
    void onWriteTaskCompleted(const WriteResult& result);

private:
    QThread* m_workerThread;
    AlarmLogSqlLogic* m_sqlLogic;
    WriteSqlDBCon* m_writeCon;
};

} // namespace LogDB

#endif // ALARMLOGDBCON_H
