#ifndef MULTIMODULELOGWIDGET_H
#define MULTIMODULELOGWIDGET_H

#include <QWidget>
#include "logmodel.h"

QT_BEGIN_NAMESPACE
class QListView;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class LogFilterProxyModel;

namespace Ui {
class MultiModuleLogWidget;
}

class MultiModuleLogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiModuleLogWidget(QWidget *parent = nullptr);
    ~MultiModuleLogWidget();

    // 公开方法
    void setPreviewLogCount(int count);
    void setTotalLogCount(int count);
    void setRollbackLogCount(int count);
    void setLogFileName(const QString &fileName);
    void setLogFile(const QString &fileName);  // 直接设置LogModel的文件名
    void setMaxLogFileSize(qint64 sizeInBytes);  // 设置最大日志文件大小（字节）
    void addLog(const QString &qrcode, LogModel::LogLevel level, const QString &message);

    // 获取当前设置
    int previewLogCount() const { return m_previewLogCount; }
    int totalLogCount() const { return m_totalLogCount; }
    int rollbackLogCount() const { return m_rollbackLogCount; }
    QString logFileName() const { return m_logModel->logFile(); } // 从LogModel获取
    qint64 maxLogFileSize() const { return m_maxLogFileSize; }

private slots:
    void onTraceListViewScrolled(int value);
    void onModuleListViewScrolled(int value);

private:
    void setupConnections();
    void updateStatusBar();

    // UI组件 - 来自UI文件
    Ui::MultiModuleLogWidget *ui;
    
    // 数据模型 - 只使用一个模型 + 代理模型
    LogModel *m_logModel;
    LogFilterProxyModel *m_proxyModel;
    
    // 设置 - 这些参数用于UI显示，实际存储在LogModel中
    int m_previewLogCount;    // 预览日志条数
    int m_totalLogCount;      // 日志总条数
    int m_rollbackLogCount;   // 回滚日志条数
    QString m_logFileName;    // 日志文件名（用于初始化）
    qint64 m_maxLogFileSize;  // 最大日志文件大小（字节），默认10MB
    
    // 滚动控制标志
    bool m_isUpdatingModel;   // 标记模型正在更新，避免滚动事件触发
};

#endif // MULTIMODULELOGWIDGET_H
