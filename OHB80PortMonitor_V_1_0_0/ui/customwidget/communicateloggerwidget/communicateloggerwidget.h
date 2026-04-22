#pragma once
#include <QWidget>
#include <QJsonObject>
#include <QStringList>
#include <QHash>
#include <QVector>
#include <functional>
#include "commitemstyle.h"
#include "commlogicalfilesystem.h"
#include "commhistoryquery.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CommunicateLoggerWidget; }
QT_END_NAMESPACE

class QPushButton;
class QTableView;
class CommLogPageTableModel;
class CommLogTableItemDelegate;
class HistoryCalendarDialog;

// 通信日志控件：实时监控 + 历史查询
class CommunicateLoggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommunicateLoggerWidget(QWidget *parent = nullptr);
    ~CommunicateLoggerWidget();

    // 配置接口
    void setRootPath(const QString &path);           // 日志根目录
    void setPageSize(int size);                      // 分页大小
    void setMaxFileBytes(qint64 bytes);              // 单文件最大字节数
    void setItemStyler(std::function<void(const QStringList &record, CommItemStyle &style)> fn);  // 样式回调

    // 实时表格
    void initQrcodeList(const QStringList &qrcodes); // 预填充 qrcode 列表（同时设置 qrcode 过滤框范围）
    void setCommandIds(const QStringList &ids);      // 填充 CommandId 下拉框（首项为空：不过滤）
    void writeLog(const QString &qrcode, const QString &time, const QString &commandId,
                  const QString &durationMs, const QString &request, const QString &response);  // 写入日志

    // 初始化
    void initialize();                               // 配置完成后调用

signals:
    void logWritten(bool success);                   // 日志写入完成

private slots:
    // 实时 Tab 信号槽
    void onPageReady(const CommPage &page, bool isPrev);
    void onLoadFailed(const QString &reason);
    void onLogAppended(bool success, bool pageChanged);
    void onNavigationUpdated(bool hasPrev, bool hasNext);

    // 历史查询 Tab 信号槽
    void onSearchClicked();
    void onHistoryReady(const CommHistoryResult &result);
    void onHistoryPageClicked(int pageIndex);
    void onSelectDateClicked();
    void onAvailableDatesReady(const QSet<QDate> &dates);

private:
    // 初始化
    void setupConnections();
    void setupLiveTab();
    void setupHistoryTab();
    void setupTableColumnWidths(QTableView *tableView);

    // 历史查询辅助
    void rebuildHistoryPageBar();
    QDate selectedDate() const;
    void  setSelectedDate(const QDate &d);

    // UI
    Ui::CommunicateLoggerWidget *ui = nullptr;
    QString m_rootPath;                              // 日志根目录
    CommLogicalFileSystem *m_lfs = nullptr;           // 逻辑文件系统

    // 实时 Tab
    CommLogPageTableModel    *m_liveModel    = nullptr;
    CommLogTableItemDelegate *m_liveDelegate = nullptr;
    std::function<void(const QStringList &record, CommItemStyle &style)> m_stylerFn;
    QVector<QStringList> m_liveRecords;              // 实时数据
    QHash<QString, int>  m_qrcodeRow;                // qrcode → 行索引

    // 历史查询 Tab
    HistoryCalendarDialog    *m_calendarDlg  = nullptr;
    CommLogPageTableModel    *m_histModel    = nullptr;
    CommLogTableItemDelegate *m_histDelegate = nullptr;
    CommHistoryQuery  m_lastQuery;                    // 上次查询
    CommHistoryResult m_lastResult;                   // 上次结果
    bool          m_histIsNewSearch      = false;

    // 常量
    static const QStringList kLiveHeaders;           // 列头
    static const int kDefaultRowCount = 80;           // 默认行数
};
