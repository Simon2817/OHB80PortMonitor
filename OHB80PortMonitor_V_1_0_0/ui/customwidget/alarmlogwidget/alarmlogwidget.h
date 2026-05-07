#ifndef ALARMLOGWIDGET_H
#define ALARMLOGWIDGET_H

#include <QWidget>
#include <QVariantMap>

namespace Ui {
class AlarmLogWidget;
}

class AlarmLogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmLogWidget(QWidget *parent = nullptr);
    ~AlarmLogWidget();

    // 初始化 UI 控件的可选项（下拉框选项 / 数值输入框范围 等）。
    // 由 MainWindow 在执行查询测试前调用，使界面具备可用的查询条件输入。
    void initUi();

    // 由外部（如主界面 / 权限模块）设置"客户是否可见"过滤条件。
    // 传入 -1 表示不应用该条件；0 表示仅查询客户不可见；1 表示仅查询客户可见。
    // 每次提交查询时都会使用此值作为过滤条件。
    void setCustomerVisibleFilter(int value);
    int  customerVisibleFilter() const;

private slots:
    void onCheckBoxAllStateChanged(int state);
    void onSearchClicked();
    void onSetStartTimeClicked();
    void onSetResolvedTimeClicked();
    void onPaginationPageChanged(int page);
    void onPageWithConditionsResult(const QList<QVariantMap>& records);
    void onTotalCountWithConditionsResult(int totalCount);
    void onRecordInserted(const QVariantMap& row);
    void onRecordResolved(const QString& qrCode,
                          const QString& alarmType,
                          const QString& resolveTime);

private:
    Ui::AlarmLogWidget *ui;

    // 分页状态
    int m_currentPage;
    int m_pageSize;
    int m_totalPages;

    // 缓存最近一次查询条件，用于翻页时复用
    int     m_lastAlarmLevel;       // -1 表示未启用
    QString m_lastQRCode;
    QString m_lastAlarmType;        // 空表示未启用，存储为 alarm_type 整数对应的字符串
    int     m_lastIsResolved;       // -1 表示未启用
    QString m_lastStartTime;
    QString m_lastEndTime;

    // 外部注入的"客户是否可见"过滤条件，默认 -1（不过滤）
    int m_customerVisibleFilter = -1;

    void submitQuery(int page);
    void setHistoryLogData(const QList<QVariantMap>& data);

    // 初始化 live log 表（建立 model + 表头 + 订阅 DBCon::recordInserted）
    void initLiveLog();

    // 启动时从 DB 加载 is_resolved=0 的警报到 live log，
    // 让本次启动能看到上一次未解决的警报。
    void loadUnresolvedToLiveLog();

    // live log 行数上限（超过后从末尾裁剪）
    static constexpr int kLiveLogMaxRows = 500;
};

#endif // ALARMLOGWIDGET_H
