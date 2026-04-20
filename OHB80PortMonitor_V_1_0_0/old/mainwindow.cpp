#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loggerwidget.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QDir>

// 表头：level、时间、qrcode、警报id、是否解决、解决时间、具体信息
const QStringList MainWindow::kHeaders = {
    QStringLiteral("level"),
    QStringLiteral("时间"),
    QStringLiteral("qrcode"),
    QStringLiteral("警报id"),
    QStringLiteral("是否解决"),
    QStringLiteral("解决时间"),
    QStringLiteral("具体信息")
};

const QStringList MainWindow::kLevels = {
    QStringLiteral("info"),
    QStringLiteral("warn"),
    QStringLiteral("error")
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 配置 LoggerWidget
    QString logRoot = QCoreApplication::applicationDirPath() + QDir::separator() + "logs";
    ui->loggerWidget->setRootPath(logRoot);
    ui->loggerWidget->setHeaders(kHeaders);
    // [level][时间][qrcode][警报id][是否解决][解决时间]：具体信息（空字段自动跳过）
    ui->loggerWidget->setFormat(
        QStringLiteral("[{}][{}][{}][{}][{}][{}]:{}"),
        kHeaders);

    // 按 level 字段（索引 0）设置每行样式
    ui->loggerWidget->setItemStyler([](const QStringList &record, ItemStyle &style) {
        const QString &level = record.value(0);
        if (level == QStringLiteral("error")) {
            style.setForeground(QColor(255, 80,  80));   // 红色文字
            style.setBackground(QColor(60,  10,  10));   // 深红背景
        } else if (level == QStringLiteral("warn")) {
            style.setForeground(QColor(255, 165,  0));   // 橙色文字
            style.setBackground(QColor(50,  35,   0));   // 深橙背景
        }
    });

    ui->loggerWidget->setPageSize(200);
    ui->loggerWidget->initialize();

    connect(ui->loggerWidget, &LoggerWidget::logWritten,
            this, &MainWindow::onLogWritten);

    // 定时器
    m_writeTimer = new QTimer(this);
    connect(m_writeTimer, &QTimer::timeout, this, &MainWindow::onWriteTimerTimeout);

    // 按鈕
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartStopClicked()
{
    if (!m_running) {
        int interval = ui->intervalSpinBox->value();
        m_writeTimer->start(interval);
        m_running = true;
        ui->startStopButton->setText(tr("停止写入"));
        ui->intervalSpinBox->setEnabled(false);
        ui->statusLabel->setText(tr("状态：写入中（间隔 %1 ms）").arg(interval));
    } else {
        m_writeTimer->stop();
        m_running = false;
        ui->startStopButton->setText(tr("开始写入"));
        ui->intervalSpinBox->setEnabled(true);
        ui->statusLabel->setText(tr("状态：已停止"));
    }
}

void MainWindow::onWriteTimerTimeout()
{
    ui->loggerWidget->writeLog(generateTestRecord());
}

void MainWindow::onLogWritten(bool success)
{
    statusBar()->showMessage(
        success
            ? tr("第 %1 条日志写入成功").arg(m_recordCounter)
            : tr("日志写入失败！"),
        2000);
}

QJsonObject MainWindow::generateTestRecord()
{
    ++m_recordCounter;

    // 级别
    int levelIdx = ui->levelComboBox->currentIndex();
    QString level = (levelIdx < 3)
        ? kLevels[levelIdx]
        : kLevels[QRandomGenerator::global()->bounded(3)];

    // 随机决定是否已解决
    bool resolved   = QRandomGenerator::global()->bounded(2) == 1;
    QString resolveTime = resolved
        ? QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
        : QString();

    QJsonObject obj;
    obj[QStringLiteral("level")]    = level;
    obj[QStringLiteral("时间")]    = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    obj[QStringLiteral("qrcode")]   = QStringLiteral("QR-%1").arg(m_recordCounter, 6, 10, QChar('0'));
    obj[QStringLiteral("警报id")]  = QStringLiteral("ALT-%1").arg(m_recordCounter, 4, 10, QChar('0'));
    obj[QStringLiteral("是否解决")] = resolved ? QStringLiteral("是") : QStringLiteral("否");
    obj[QStringLiteral("解决时间")] = resolveTime;
    obj[QStringLiteral("具体信息")] = QStringLiteral("[%1] 测试日志第 %2 条")
        .arg(level.toUpper()).arg(m_recordCounter);
    return obj;
}
