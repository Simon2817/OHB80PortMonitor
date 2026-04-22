#pragma once
#include <QDate>
#include <QHash>
#include <QList>
#include <QSet>
#include <QTimer>
#include <QWidget>
#include "alarmid.h"
#include "alarminfo.h"
#include "alarmheaderconfig.h"

class QTableWidget;
class AlarmLogicSystem;

QT_BEGIN_NAMESPACE
namespace Ui { class AlarmLoggerWidget; }
QT_END_NAMESPACE

// ====================================================================
// AlarmLoggerWidget — 警报日志控件
//
// 用法：
//   qint64 id = makeAlarmId(12001, AlarmCode::GeneralOverHumidity);
//   widget->submitAlarm(AlarmLevel::Warn, "12001", id, "超湿报警已触发");
//   widget->submitResolve(id);
//
// 其他控件可通过 logicSystem() 连接 alarmPublished / alarmResolved 信号。
// ====================================================================
class AlarmLoggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmLoggerWidget(QWidget *parent = nullptr);
    explicit AlarmLoggerWidget(const AlarmHeaderConfig &config, QWidget *parent = nullptr);
    ~AlarmLoggerWidget();

    // 提交一条警报（由 AlarmLogicSystem 去重后写入表格）
    void setRootDir(const QString &dir);
    QString rootDir() const;

    void submitAlarm(AlarmLevel level, const QString &qrCode,
                     qint64 alarmId, const QString &message);

    // 提交警报解决（由 AlarmLogicSystem 确认后更新表格）
    void submitResolve(qint64 alarmId);

    // 访问内部逻辑系统，供外部连接 alarmPublished / alarmResolved 信号
    AlarmLogicSystem *logicSystem() const;

    // 判断指定的警报是否处于活跃状态
    bool isActive(qint64 alarmId) const;

    // 设置每次清理周期最多清理的已解决行数（默认：40）
    void setResolvedPurgeBatchSize(int size);

signals:
    // 转发 AlarmLogicSystem 的 alarmPublished 信号
    void alarmPublished(const AlarmInfo &info);
    // 转发 AlarmLogicSystem 的 alarmResolved 信号
    void alarmResolved(const AlarmInfo &info);

private:
    void initTable();
    void setResolvedCell(int row, bool resolved);

    // 仅由 AlarmLogicSystem 信号驱动，外部不直接调用
    void writeRecord(const AlarmInfo &info);
    void resolveRecord(qint64 alarmId);

    // 从实时表中清理已解决的行
    void purgeResolvedRows();

    // 设定凌晨自动重置定时器
    void scheduleMidnightReset();
    // 凌晨跨日时执行：清空表格 + 清空活跃列表
    void onDayRollover();

    // 历史记录 tab
    void initHistoryTab();
    void onSelectDateClicked();
    void onHistoryDateSelected(const QDate &date);
    void applyTimeFilter();
    void renderHistoryPage(int page);
    void updatePagination();
    void onPrevPage();
    void onNextPage();
    void goToPage(int page);
    void onSearchClicked();
    void onPrevMatch();
    void onNextMatch();
    void jumpToMatch(int matchIdx);
    QSet<QDate> collectAvailableDates() const;

    Ui::AlarmLoggerWidget    *ui      = nullptr;
    QTableWidget             *m_table = nullptr;
    AlarmLogicSystem         *m_logic = nullptr;
    QHash<qint64, QList<int>> m_idRowMap;
    AlarmHeaderConfig         m_headerConfig;
    QTimer                   *m_midnightTimer = nullptr;

    // 已解决行清理
    QList<int>                m_resolvedRows;
    QTimer                   *m_purgeTimer = nullptr;
    int                       m_purgeBatchSize = 40;

    QWidget           *m_histPageBar = nullptr;  // 分页栏容器，仅多页时显示

    // 历史记录数据
    QList<QStringList> m_histAllRecords;
    QList<QStringList> m_histFiltered;
    int                m_histCurrentPage  = 0;
    QList<int>         m_histMatchIndices;
    int                m_histCurrentMatch = -1;
    static constexpr int kHistPageSize   = 50;

    int headerToColumn(const QString &header);
};
