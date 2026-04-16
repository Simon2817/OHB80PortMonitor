#include "logmodel.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QColor>

QString LogModel::logLevelToString(LogLevel level)
{
    switch (level) {
        case Debug: return "DEBUG";
        case Info: return "INFO";
        case Warning: return "WARNING";
        case Error: return "ERROR";
        case Critical: return "CRITICAL";
        default: return "INFO";
    }
}

QColor LogModel::logLevelToColor(LogLevel level)
{
    switch (level) {
        case Debug:
            return QColor(128, 128, 128); // 灰色
        case Info:
            return QColor(0, 0, 0);       // 黑色（默认）
        case Warning:
            return QColor(255, 165, 0);   // 橙黄色（工业警报色）
        case Error:
            return QColor(255, 0, 0);     // 红色（工业警报色）
        case Critical:
            return QColor(139, 0, 0);     // 深红色（严重警报）
        default:
            return QColor(0, 0, 0);       // 默认黑色
    }
}

LogModel::LogModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_maxDisplayCount(100)
    , m_maxStorageCount(1000)
    , m_deleteOldCount(100)
    , m_logFile("log.txt")
    , m_maxLogFileSize(10 * 1024 * 1024)  // 默认10MB
    , m_blockSize(50)
    , m_maxBlocks(10)  // 增加到10个块（500条日志），大幅减少移除频率
    , m_currentDisplayStart(-1)
    , m_currentDisplayEnd(-1)
{
    // 生成带时间戳的初始文件名
    m_currentLogFile = generateTimestampedFileName(m_logFile);
    qDebug() << "LogModel初始化，当前日志文件:" << m_currentLogFile;
    
    // 启动定时器，定期检查日志限制
    m_checkTimer = new QTimer(this);
    connect(m_checkTimer, &QTimer::timeout, this, &LogModel::checkLogLimits);
    m_checkTimer->start(1000); // 每秒检查一次
}

LogModel::~LogModel()
{
    if (m_allLogs.size() > 0) {
        qDebug() << "析构函数：保存" << m_allLogs.size() << "条日志到文件:" << m_logFile;
        saveToFile();
    }
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_displayLogs.size();
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_displayLogs.size())
        return QVariant();
    
    const LogModel::LogEntry &entry = m_displayLogs.at(index.row());
    
    switch (role) {
    case Qt::DisplayRole:
    case DisplayRole:
        return QString("[%1] %2: %3")
               .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
               .arg(logLevelToString(entry.level))
               .arg(entry.message);
    case MessageRole:
        return entry.message;
    case TimestampRole:
        return entry.timestamp.toString("yyyy-MM-dd hh:mm:ss");
    case LevelRole:
        return logLevelToString(entry.level);
    case ColorRole:
        return logLevelToColor(entry.level);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MessageRole] = "message";
    roles[TimestampRole] = "timestamp";
    roles[LevelRole] = "level";
    roles[DisplayRole] = "display";
    roles[ColorRole] = "color";
    return roles;
}

void LogModel::addLog(const QString &message, LogLevel level)
{
    LogModel::LogEntry entry(message, level);
    
    qDebug() << "添加日志:" << message << "级别:" << logLevelToString(level);
    
    // 添加到总日志列表末尾（最新的在最后）
    m_allLogs.append(entry);
    
    // 更新显示日志
    updateDisplayLogs();
    
    emit logAdded(message, level);
    
    // 立即检查是否需要删除旧日志
    checkLogLimits();
}

void LogModel::clearLogs()
{
    m_allLogs.clear();
    m_displayLogs.clear();
    
    beginResetModel();
    endResetModel();
    
    emit logsCleared();
}

void LogModel::saveToFile(const QString &fileName)
{
    QString fileToUse = fileName.isEmpty() ? m_logFile : fileName;
    qDebug() << "保存日志到文件:" << fileToUse << "，日志条数:" << m_allLogs.size();
    asyncWriteLogsToFile(m_allLogs, fileToUse);
    emit logsSaved(fileToUse);
}

void LogModel::setMaxDisplayCount(int count)
{
    if (count > 0 && count != m_maxDisplayCount) {
        m_maxDisplayCount = count;
        updateDisplayLogs();
    }
}

void LogModel::setMaxStorageCount(int count)
{
    if (count > 0 && count != m_maxStorageCount) {
        m_maxStorageCount = count;
        checkLogLimits();
    }
}

void LogModel::setDeleteOldCount(int count)
{
    if (count > 0 && count != m_deleteOldCount) {
        m_deleteOldCount = count;
    }
}

void LogModel::setLogFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        m_logFile = fileName;
        // 重新生成带时间戳的文件名
        m_currentLogFile = generateTimestampedFileName(m_logFile);
        qDebug() << "设置日志文件名为:" << m_logFile << "，当前文件:" << m_currentLogFile;
    }
}

void LogModel::setMaxLogFileSize(qint64 sizeInBytes)
{
    if (sizeInBytes > 0) {
        m_maxLogFileSize = sizeInBytes;
        qDebug() << "设置最大日志文件大小:" << sizeInBytes << "字节";
    }
}

void LogModel::checkLogLimits()
{
    // 检查存储限制
    if (m_allLogs.size() > m_maxStorageCount) {
        deleteOldLogs();
    }
}

void LogModel::updateDisplayLogs()
{
    beginResetModel();
    m_displayLogs.clear();
    
    // 显示所有日志，让用户可以自由滚动查看
    // m_maxDisplayCount 不再限制显示数量，只用于其他逻辑
    m_displayLogs = m_allLogs;
    
    endResetModel();
    
    qDebug() << "更新显示日志: 总数=" << m_allLogs.size() << "显示=" << m_displayLogs.size();
}

void LogModel::deleteOldLogs()
{
    if (m_allLogs.size() <= m_maxStorageCount) {
        return;
    }
    
    // 固定删除回滚条数
    int deleteCount = m_deleteOldCount;
    
    // 保存要删除的日志到文件（删除最旧的日志）
    QList<LogModel::LogEntry> logsToDelete;
    for (int i = 0; i < deleteCount && i < m_allLogs.size(); ++i) {
        logsToDelete.append(m_allLogs.at(i));
    }
    
    if (!logsToDelete.isEmpty()) {
        asyncWriteLogsToFile(logsToDelete, m_logFile);
    }
    
    // 从内存中删除旧日志（从列表开头删除）
    beginResetModel();
    for (int i = 0; i < deleteCount && !m_allLogs.isEmpty(); ++i) {
        m_allLogs.removeFirst();
    }
    updateDisplayLogs();
    endResetModel();
    
    emit oldLogsDeleted(deleteCount);
    
    qDebug() << "删除了" << deleteCount << "条旧日志，已保存到" << m_logFile;
}

void LogModel::setBlockSize(int size)
{
    if (size > 0 && size != m_blockSize) {
        m_blockSize = size;
        qDebug() << "设置块大小:" << size;
    }
}

int LogModel::getBlockCount() const
{
    if (m_allLogs.isEmpty()) {
        return 0;
    }
    return (m_allLogs.size() + m_blockSize - 1) / m_blockSize;
}

void LogModel::loadBlock(int blockIndex, bool isScrollingUp)
{
    if (blockIndex < 0 || blockIndex >= getBlockCount()) {
        qWarning() << "无效的块索引:" << blockIndex << "，总块数:" << getBlockCount();
        return;
    }
    
    int startIndex = blockIndex * m_blockSize;
    int endIndex = qMin(startIndex + m_blockSize - 1, m_allLogs.size() - 1);
    
    if (startIndex >= m_allLogs.size()) {
        qWarning() << "块起始索引超出范围:" << startIndex;
        return;
    }
    
    // 检查是否已经加载了这个块（精确匹配块索引）
    for (int i = 0; i < m_loadedBlocks.size(); ++i) {
        if (m_loadedBlocks[i].startIndex == startIndex) {
            // 块已加载，更新访问时间
            m_loadedBlocks[i].lastAccessTime = QDateTime::currentMSecsSinceEpoch();
            qDebug() << "块" << blockIndex << "已加载，更新访问时间";
            return;
        }
    }
    
    // 如果已经有2个块，先移除最旧的块
    if (m_loadedBlocks.size() >= m_maxBlocks) {
        removeOldestBlock();
    }
    
    // 创建并添加新块
    LogBlock newBlock(startIndex, endIndex);
    m_loadedBlocks.append(newBlock);
    
    // 更新显示范围
    updateDisplayLogsFromBlocks();
    
    qDebug() << "加载新块" << blockIndex << "，范围:[" << startIndex << "-" << endIndex << "]" 
             << "，已加载块数:" << m_loadedBlocks.size();
}

void LogModel::removeOldestBlock()
{
    if (m_loadedBlocks.isEmpty()) {
        return;
    }
    
    // 只有当块数超过8个时才开始移除，保持至少5个块的缓冲
    if (m_loadedBlocks.size() <= 5) {
        qDebug() << "块数未超过限制，不移除。当前块数:" << m_loadedBlocks.size();
        return;
    }
    
    // 找到最旧的块（最久未访问的）
    qint64 oldestTime = QDateTime::currentMSecsSinceEpoch();
    int oldestIndex = 0;
    for (int i = 0; i < m_loadedBlocks.size(); ++i) {
        if (m_loadedBlocks[i].lastAccessTime < oldestTime) {
            oldestTime = m_loadedBlocks[i].lastAccessTime;
            oldestIndex = i;
        }
    }
    
    LogBlock removedBlock = m_loadedBlocks.takeAt(oldestIndex);
    qDebug() << "移除最旧块，范围:" << removedBlock.startIndex << "-" << removedBlock.endIndex 
             << "，剩余块数:" << m_loadedBlocks.size();
}

QList<LogModel::LogEntry> LogModel::getLogsInRange(int startIndex, int endIndex) const
{
    QList<LogModel::LogEntry> result;
    
    if (startIndex < 0 || endIndex >= m_allLogs.size() || startIndex > endIndex) {
        return result;
    }
    
    for (int i = startIndex; i <= endIndex && i < m_allLogs.size(); ++i) {
        result.append(m_allLogs.at(i));
    }
    
    return result;
}

void LogModel::updateDisplayLogsFromBlocks()
{
    if (m_loadedBlocks.isEmpty()) {
        m_displayLogs.clear();
        m_currentDisplayStart = -1;
        m_currentDisplayEnd = -1;
        return;
    }
    
    // 找到所有已加载块的并集范围
    int minStart = m_allLogs.size();
    int maxEnd = -1;
    
    for (const LogBlock &block : m_loadedBlocks) {
        minStart = qMin(minStart, block.startIndex);
        maxEnd = qMax(maxEnd, block.endIndex);
    }
    
    m_currentDisplayStart = minStart;
    m_currentDisplayEnd = maxEnd;
    
    // 收集所有范围内的日志
    m_displayLogs.clear();
    for (int i = minStart; i <= maxEnd && i < m_allLogs.size(); ++i) {
        m_displayLogs.append(m_allLogs.at(i));
    }
    
    beginResetModel();
    endResetModel();
    
    qDebug() << "更新显示日志，范围:" << minStart << "-" << maxEnd 
             << "，显示条数:" << m_displayLogs.size();
}

void LogModel::writeLogsToFile(const QList<LogModel::LogEntry> &logs, const QString &fileName,
                               const QString &currentLogFile, qint64 maxLogFileSize)
{
    QString targetFile = currentLogFile;
    
    // 检查当前文件大小，如果超过限制则创建新文件
    qint64 currentSize = getFileSize(targetFile);
    if (currentSize >= maxLogFileSize) {
        qDebug() << "当前日志文件" << targetFile << "大小" << currentSize 
                 << "字节，超过限制" << maxLogFileSize << "字节，创建新文件";
        
        // 生成新的带时间戳的文件名
        QString name = fileName;
        QString extension;
        int dotIndex = fileName.lastIndexOf('.');
        if (dotIndex > 0) {
            name = fileName.left(dotIndex);
            extension = fileName.mid(dotIndex);
        }
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        targetFile = QString("%1_%2%3").arg(timestamp, name, extension);
        
        qDebug() << "新日志文件:" << targetFile;
    }
    
    // 使用目标日志文件（可能是新生成的）
    QFile file(targetFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "无法打开日志文件:" << targetFile;
        return;
    }
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    
    for (const LogModel::LogEntry &entry : logs) {
        out << QString("[%1] %2: %3\n")
               .arg(entry.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
               .arg(logLevelToString(entry.level))
               .arg(entry.message);
    }
    
    file.close();
    qDebug() << "写入" << logs.size() << "条日志到文件:" << targetFile;
}

QString LogModel::generateTimestampedFileName(const QString &baseFileName)
{
    // 从基础文件名中提取文件名和扩展名
    // 例如: "multimodule_log.txt" -> "multimodule_log" + ".txt"
    QString name = baseFileName;
    QString extension;
    
    int dotIndex = baseFileName.lastIndexOf('.');
    if (dotIndex > 0) {
        name = baseFileName.left(dotIndex);
        extension = baseFileName.mid(dotIndex);  // 包含点号
    }
    
    // 生成时间戳: 年月日_时分秒
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    
    // 组合: 年月日_时分秒_原文件名.txt
    QString timestampedFileName = QString("%1_%2%3").arg(timestamp, name, extension);
    
    qDebug() << "生成带时间戳的文件名:" << baseFileName << "->" << timestampedFileName;
    
    return timestampedFileName;
}

qint64 LogModel::getFileSize(const QString &fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        return 0;
    }
    return file.size();
}

void LogModel::asyncWriteLogsToFile(const QList<LogModel::LogEntry> &logs, const QString &fileName)
{
    // 复制必要的参数到局部变量，避免在异步线程中访问成员变量
    QString currentFile = m_currentLogFile;
    qint64 maxSize = m_maxLogFileSize;
    QString baseFile = fileName;
    
    qDebug() << "启动异步写入" << logs.size() << "条日志到文件";
    
    // 使用QtConcurrent::run在后台线程中执行写入操作
    QtConcurrent::run([logs, baseFile, currentFile, maxSize]() {
        LogModel::writeLogsToFile(logs, baseFile, currentFile, maxSize);
    });
}
