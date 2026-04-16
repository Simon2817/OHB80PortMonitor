#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

class LogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // 日志级别枚举
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
    Q_ENUM(LogLevel)
    
    enum LogRoles {
        MessageRole = Qt::UserRole + 1,
        TimestampRole,
        LevelRole,
        DisplayRole,
        ColorRole
    };
    
    // 日志条目结构
    struct LogEntry {
        QString message;
        QDateTime timestamp;
        LogLevel level;
        
        LogEntry(const QString &msg, LogLevel lvl = Info)
            : message(msg), timestamp(QDateTime::currentDateTime()), level(lvl) {}
    };

    explicit LogModel(QObject *parent = nullptr);
    ~LogModel();

    // 基本模型功能
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 日志管理功能
    Q_INVOKABLE void addLog(const QString &message, LogLevel level = Info);
    Q_INVOKABLE void clearLogs();
    Q_INVOKABLE void saveToFile(const QString &fileName = "");
    
    // 工具方法：将枚举转换为字符串
    static QString logLevelToString(LogLevel level);
    
    // 工具方法：获取日志级别对应的颜色
    static QColor logLevelToColor(LogLevel level);
    

    // 设置功能
    void setMaxDisplayCount(int count);
    void setMaxStorageCount(int count);
    void setDeleteOldCount(int count);
    void setLogFile(const QString &fileName);
    void setMaxLogFileSize(qint64 sizeInBytes);

    // 获取当前设置
    int maxDisplayCount() const { return m_maxDisplayCount; }
    int maxStorageCount() const { return m_maxStorageCount; }
    int deleteOldCount() const { return m_deleteOldCount; }
    QString logFile() const { return m_logFile; }
    qint64 maxLogFileSize() const { return m_maxLogFileSize; }

    // 获取日志统计
    int totalLogCount() const { return m_allLogs.size(); }
    int displayLogCount() const { return m_displayLogs.size(); }
    
    // 分块加载功能
    void loadBlock(int blockIndex, bool isScrollingUp);
    void removeOldestBlock();
    int getBlockCount() const;
    int getBlockSize() const { return m_blockSize; }
    void setBlockSize(int size);
    QList<LogEntry> getLogsInRange(int startIndex, int endIndex) const;
    int currentDisplayStart() const { return m_currentDisplayStart; }
    int currentDisplayEnd() const { return m_currentDisplayEnd; }
    void updateDisplayLogsFromBlocks();

signals:
    void logAdded(const QString &message, LogLevel level);
    void logsCleared();
    void logsSaved(const QString &fileName);
    void oldLogsDeleted(int deletedCount);
    void displayLogsUpdated(); // 显示日志更新时发出

private slots:
    void checkLogLimits();

private:
    void updateDisplayLogs();
    void deleteOldLogs();
    void asyncWriteLogsToFile(const QList<LogEntry> &logs, const QString &fileName);
    static void writeLogsToFile(const QList<LogEntry> &logs, const QString &fileName, 
                                const QString &currentLogFile, qint64 maxLogFileSize);
    QString generateTimestampedFileName(const QString &baseFileName);
    static qint64 getFileSize(const QString &fileName);
    
    QList<LogEntry> m_allLogs;        // 所有存储的日志
    QList<LogEntry> m_displayLogs;    // 当前显示的日志
    
    int m_maxDisplayCount;    // 最大显示条数
    int m_maxStorageCount;    // 最大存储条数
    int m_deleteOldCount;     // 删除旧日志条数
    QString m_logFile;        // 日志文件名（基础文件名）
    QString m_currentLogFile; // 当前实际使用的日志文件名（带时间戳）
    qint64 m_maxLogFileSize;  // 最大日志文件大小（字节）
    
    // 分块加载相关
    struct LogBlock {
        int startIndex;
        int endIndex;
        qint64 lastAccessTime;
        
        LogBlock(int start, int end)
            : startIndex(start), endIndex(end), lastAccessTime(QDateTime::currentMSecsSinceEpoch()) {}
    };
    
    QList<LogBlock> m_loadedBlocks;  // 已加载的块列表
    int m_blockSize;                 // 每块的大小
    int m_maxBlocks;                 // 最多保留的块数
    int m_currentDisplayStart;       // 当前显示的起始索引
    int m_currentDisplayEnd;         // 当前显示的结束索引
    
    QTimer *m_checkTimer;     // 定时检查日志限制
};

#endif // LOGMODEL_H
