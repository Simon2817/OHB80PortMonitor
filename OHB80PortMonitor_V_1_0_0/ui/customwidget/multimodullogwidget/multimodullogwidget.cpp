#include "multimodullogwidget.h"
#include "ui_multimodullogwidget.h"
#include "logmodel.h"
#include "logfilterproxymodel.h"
#include "logdelegate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>

MultiModuleLogWidget::MultiModuleLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MultiModuleLogWidget)
    , m_logModel(nullptr)
    , m_proxyModel(nullptr)
    , m_previewLogCount(100)
    , m_totalLogCount(1000)
    , m_rollbackLogCount(100)
    , m_logFileName("multimodule_log.txt")
    , m_maxLogFileSize(10 * 1024 * 1024)  // 默认10MB
    , m_isUpdatingModel(false)
{
    qDebug() << "MultiModuleLogWidget 构造函数开始，初始文件名:" << m_logFileName;
    ui->setupUi(this);
    setupConnections();
    qDebug() << "MultiModuleLogWidget 构造函数完成，当前文件名:" << m_logFileName;
}

MultiModuleLogWidget::~MultiModuleLogWidget()
{
    delete ui;
    // LogModel会在自己的析构函数中保存日志，不需要重复保存
}

void MultiModuleLogWidget::setupConnections()
{
    // 创建单个数据模型
    m_logModel = new LogModel(this);
    qDebug() << "LogModel 创建完成，默认文件名:" << m_logModel->logFile();
    
    // 设置初始值（必须在创建模型后立即设置）
    qDebug() << "准备设置文件名为:" << m_logFileName;
    m_logModel->setLogFile(m_logFileName); // 首先设置日志文件名
    qDebug() << "文件名设置后，LogModel 的文件名:" << m_logModel->logFile();
    
    m_logModel->setMaxDisplayCount(m_previewLogCount);
    m_logModel->setMaxStorageCount(m_totalLogCount);
    m_logModel->setDeleteOldCount(m_rollbackLogCount);
    m_logModel->setBlockSize(m_previewLogCount); // 设置块大小
    m_logModel->setMaxLogFileSize(m_maxLogFileSize); // 设置最大文件大小
    
    // 创建代理模型用于过滤
    m_proxyModel = new LogFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_logModel);
    m_proxyModel->setFilterEnabled(false); // 初始不启用过滤
    
    // 设置模型
    ui->moduleListView->setModel(m_proxyModel);  // 模块日志使用代理模型
    ui->traceListView->setModel(m_logModel);      // 汇总日志使用原始模型
    
    // 设置自定义委托，实现不同级别的颜色显示
    LogDelegate *traceDelegate = new LogDelegate(this);
    ui->traceListView->setItemDelegate(traceDelegate);
    
    LogDelegate *moduleDelegate = new LogDelegate(this);
    ui->moduleListView->setItemDelegate(moduleDelegate);
    
    // 调试信息
    qDebug() << "模型设置完成 - 原始模型行数:" << m_logModel->rowCount()
             << "代理模型行数:" << m_proxyModel->rowCount();
    
    // 连接滚动条信号
    connect(ui->traceListView->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MultiModuleLogWidget::onTraceListViewScrolled);
    connect(ui->moduleListView->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MultiModuleLogWidget::onModuleListViewScrolled);
    
    // 连接信号槽 - 只需要实时过滤
    connect(ui->qrcodeEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        qDebug() << "QRCode输入框内容变化:" << text;
        if (text.isEmpty()) {
            // 如果QRCode输入框为空，禁用过滤，模块日志显示空白
            m_proxyModel->setFilterEnabled(false);
        } else {
            // 启用过滤并设置QRCode
            m_proxyModel->setFilterEnabled(true);
            m_proxyModel->setFilterQRCode(text);
        }
    });
}

void MultiModuleLogWidget::setPreviewLogCount(int count)
{
    if (count > 0 && count != m_previewLogCount) {
        m_previewLogCount = count;
        m_logModel->setMaxDisplayCount(count);
    }
}

void MultiModuleLogWidget::setTotalLogCount(int count)
{
    if (count > 0 && count != m_totalLogCount) {
        m_totalLogCount = count;
        m_logModel->setMaxStorageCount(count);
        // LogModel会自动检查限制，不需要手动调用
    }
}

void MultiModuleLogWidget::setRollbackLogCount(int count)
{
    if (count > 0 && count != m_rollbackLogCount) {
        m_rollbackLogCount = count;
        m_logModel->setDeleteOldCount(count);
    }
}

void MultiModuleLogWidget::setLogFileName(const QString &fileName)
{
    // 保存文件名并设置到LogModel
    m_logFileName = fileName;
    if (m_logModel) {
        m_logModel->setLogFile(fileName);
    }
}

void MultiModuleLogWidget::setLogFile(const QString &fileName)
{
    // 保存文件名并设置到LogModel
    qDebug() << "MultiModuleLogWidget::setLogFile() 调用，文件名:" << fileName;
    m_logFileName = fileName;
    if (m_logModel) {
        m_logModel->setLogFile(fileName);
    } else {
        qDebug() << "m_logModel 尚未创建，文件名将在创建时设置";
    }
}

void MultiModuleLogWidget::setMaxLogFileSize(qint64 sizeInBytes)
{
    if (sizeInBytes > 0) {
        m_maxLogFileSize = sizeInBytes;
        if (m_logModel) {
            m_logModel->setMaxLogFileSize(sizeInBytes);
        }
        qDebug() << "设置最大日志文件大小:" << sizeInBytes << "字节 (" 
                 << (sizeInBytes / 1024.0 / 1024.0) << "MB)";
    }
}

void MultiModuleLogWidget::addLog(const QString &qrcode, LogModel::LogLevel level, const QString &message)
{
    // 格式：[QR001] INFO: 系统启动完成1
    // 不再重复添加级别信息，因为LogModel会自动添加
    QString logMessage = QString("[%1] %2").arg(qrcode, message);
    m_logModel->addLog(logMessage, level);
    
    // LogModel的updateDisplayLogs已经处理了显示更新
    // 不需要在这里加载块，分块加载只用于用户主动滚动浏览历史
    
    // 让两个ListView的滚动条自动滚动到底部
    ui->traceListView->scrollToBottom();
    ui->moduleListView->scrollToBottom();
    
    qDebug() << "添加日志:" << qrcode << LogModel::logLevelToString(level) << message 
             << "总行数:" << m_logModel->rowCount() 
             << "过滤后行数:" << m_proxyModel->rowCount();
}

void MultiModuleLogWidget::updateStatusBar()
{
    // 预留方法，用于未来可能的状态栏更新
}

void MultiModuleLogWidget::onTraceListViewScrolled(int value)
{
    // 如果模型正在更新，忽略滚动事件
    if (m_isUpdatingModel) {
        return;
    }
    
    QScrollBar *scrollBar = ui->traceListView->verticalScrollBar();
    int maximum = scrollBar->maximum();
    int minimum = scrollBar->minimum();
    
    // 分块加载功能暂时禁用
    // 当前实现中，updateDisplayLogs已经处理了最新日志的显示
    // 分块加载会导致显示范围跳跃，影响用户体验
    // TODO: 重新设计分块加载机制，使其适用于查看历史日志
    
    Q_UNUSED(value)
    Q_UNUSED(maximum)
    Q_UNUSED(minimum)
    
    /*
    int currentDisplayStart = m_logModel->currentDisplayStart();
    int currentDisplayEnd = m_logModel->currentDisplayEnd();
    int totalBlocks = m_logModel->getBlockCount();
    int totalLogs = m_logModel->totalLogCount();
    int blockSize = m_logModel->getBlockSize();
    
    // 如果还没有加载任何块，不处理滚动
    if (currentDisplayStart < 0) {
        return;
    }
    
    // 计算当前已加载块的范围
    int minBlockIndex = currentDisplayStart / blockSize;
    int maxBlockIndex = currentDisplayEnd / blockSize;
    
    qDebug() << "滚动:" << value << "/" << maximum 
             << "，显示范围:[" << currentDisplayStart << "-" << currentDisplayEnd << "]" 
             << "，块范围:[" << minBlockIndex << "-" << maxBlockIndex << "]";
    
    // 只在向上滚动到顶部时加载历史块
    if (value <= minimum + 10 && minBlockIndex > 0) {
        int prevBlockIndex = minBlockIndex - 1;
        qDebug() << "滚动到顶部，加载块" << prevBlockIndex;
        m_isUpdatingModel = true;
        m_logModel->loadBlock(prevBlockIndex, true);
        m_isUpdatingModel = false;
    }
    */
}

void MultiModuleLogWidget::onModuleListViewScrolled(int value)
{
    // 模块日志使用代理模型，暂时不实现分块加载
    // 可以根据需要实现类似的逻辑
    Q_UNUSED(value)
}
