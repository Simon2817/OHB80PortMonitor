#include "alarmloggerwidget.h"
#include "ui_alarmloggerwidget.h"
#include "alarmlogicsystem.h"
#include "alarminfo.h"
#include "historycalendardialog/historycalendardialog.h"
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QFont>
#include <QHeaderView>
#include <QCheckBox>
#include <QInputDialog>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QTableWidgetItem>

AlarmLoggerWidget::AlarmLoggerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmLoggerWidget)
    , m_headerConfig()
{
    ui->setupUi(this);
    m_table = ui->tableAlarm;

    m_logic = new AlarmLogicSystem(m_headerConfig, this);

    connect(m_logic, &AlarmLogicSystem::alarmPublished,
            this, [this](const AlarmInfo &info) {
                writeRecord(info);
                emit alarmPublished(info);
            });

    connect(m_logic, &AlarmLogicSystem::alarmResolved,
            this, [this](const AlarmInfo &info) {
                resolveRecord(info.alarmId());
                emit alarmResolved(info);
            });

    initTable();
    initHistoryTab();
    scheduleMidnightReset();
}

AlarmLoggerWidget::AlarmLoggerWidget(const AlarmHeaderConfig &config, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlarmLoggerWidget)
    , m_headerConfig(config)
{
    ui->setupUi(this);
    m_table = ui->tableAlarm;

    m_logic = new AlarmLogicSystem(m_headerConfig, this);

    connect(m_logic, &AlarmLogicSystem::alarmPublished,
            this, [this](const AlarmInfo &info) {
                writeRecord(info);
                emit alarmPublished(info);
            });

    connect(m_logic, &AlarmLogicSystem::alarmResolved,
            this, [this](const AlarmInfo &info) {
                resolveRecord(info.alarmId());
                emit alarmResolved(info);
            });

    initTable();
    initHistoryTab();
    scheduleMidnightReset();
}

AlarmLoggerWidget::~AlarmLoggerWidget()
{
    delete ui;
}

void AlarmLoggerWidget::submitAlarm(AlarmLevel level, const QString &qrCode,
                                    qint64 alarmId, const QString &message)
{
    m_logic->submitAlarm(level, qrCode, alarmId, message);
}

void AlarmLoggerWidget::submitResolve(qint64 alarmId)
{
    m_logic->submitResolve(alarmId);
}

void AlarmLoggerWidget::setRootDir(const QString &dir)
{
    m_logic->setRootDir(dir);
}

QString AlarmLoggerWidget::rootDir() const
{
    return m_logic->rootDir();
}

bool AlarmLoggerWidget::isActive(qint64 alarmId) const
{
    return m_logic->isActive(alarmId);
}

AlarmLogicSystem *AlarmLoggerWidget::logicSystem() const
{
    return m_logic;
}

void AlarmLoggerWidget::scheduleMidnightReset()
{
    const QDateTime midnight(
        QDate::currentDate().addDays(1), QTime(0, 0, 0));
    const qint64 ms = QDateTime::currentDateTime().msecsTo(midnight);

    if (!m_midnightTimer) {
        m_midnightTimer = new QTimer(this);
        m_midnightTimer->setSingleShot(true);
        connect(m_midnightTimer, &QTimer::timeout,
                this, &AlarmLoggerWidget::onDayRollover);
    }
    m_midnightTimer->start(static_cast<int>(ms));
}

void AlarmLoggerWidget::onDayRollover()
{
    m_table->setRowCount(0);
    m_idRowMap.clear();
    m_logic->clearActive();
    m_logic->purgeOldLogsAsync();  // 异步清理超过 6 个月的历史目录

    scheduleMidnightReset();
}

int AlarmLoggerWidget::headerToColumn(const QString &header)
{
    return m_headerConfig.headerToColumn(header);
}

void AlarmLoggerWidget::initTable()
{
    m_table->verticalHeader()->setVisible(false);

    auto *hh = m_table->horizontalHeader();
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[0]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[1]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[2]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[3]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[4]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[5]), QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(headerToColumn(m_headerConfig.headers[6]), QHeaderView::Stretch);
}

void AlarmLoggerWidget::writeRecord(const AlarmInfo &info)
{
    const int row = m_table->rowCount();
    m_table->insertRow(row);

    // ---- 
    auto *levelItem = new QTableWidgetItem(alarmLevelToString(info.level()).toUpper());
    levelItem->setTextAlignment(Qt::AlignCenter);
    QFont boldFont = levelItem->font();
    boldFont.setBold(true);
    levelItem->setFont(boldFont);
    if (info.level() == AlarmLevel::Error)
        levelItem->setForeground(QColor(220, 50, 50));
    else
        levelItem->setForeground(QColor(210, 140, 0));
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[0]), levelItem);

    // ---- 
    auto *timeItem = new QTableWidgetItem(info.sendTime());
    timeItem->setTextAlignment(Qt::AlignCenter);
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[1]), timeItem);

    // ---- 
    auto *qrItem = new QTableWidgetItem(info.qrCode());
    qrItem->setTextAlignment(Qt::AlignCenter);
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[2]), qrItem);

    // ---- 
    auto *idItem = new QTableWidgetItem(alarmIdToString(info.alarmId()));
    idItem->setData(Qt::UserRole, info.alarmId());
    idItem->setTextAlignment(Qt::AlignCenter);
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[3]), idItem);

    // ---- 
    setResolvedCell(row, false);

    // ---- 
    auto *rtItem = new QTableWidgetItem();
    rtItem->setTextAlignment(Qt::AlignCenter);
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[5]), rtItem);

    // ---- 
    m_table->setItem(row, headerToColumn(m_headerConfig.headers[6]), new QTableWidgetItem(info.message()));

    m_idRowMap[info.alarmId()].append(row);
    m_table->scrollToBottom();
}

void AlarmLoggerWidget::resolveRecord(qint64 alarmId)
{
    auto it = m_idRowMap.find(alarmId);
    if (it == m_idRowMap.end())
        return;

    const QString resolveTime =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    for (int row : it.value()) {
        const auto *resolvedItem = m_table->item(row, headerToColumn(m_headerConfig.headers[4]));
        if (!resolvedItem || resolvedItem->text() != QStringLiteral("✗"))
            continue;
        setResolvedCell(row, true);
        m_table->item(row, headerToColumn(m_headerConfig.headers[5]))->setText(resolveTime);
    }
}

void AlarmLoggerWidget::setResolvedCell(int row, bool resolved)
{
    const int col = headerToColumn(m_headerConfig.headers[4]);
    auto *item = m_table->item(row, col);
    if (!item) {
        item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        m_table->setItem(row, col, item);
    }
    if (resolved) {
        item->setText(QStringLiteral("✓"));
        item->setBackground(QColor(34, 139, 34));
        item->setForeground(Qt::white);
    } else {
        item->setText(QStringLiteral("✗"));
        item->setBackground(QColor(200, 30, 30));
        item->setForeground(Qt::white);
    }
}

// ================================================================
// 历史记录 Tab
// ================================================================

void AlarmLoggerWidget::initHistoryTab()
{
    connect(ui->histTimeRangeCheck, &QCheckBox::toggled, this, [this](bool on) {
        ui->histStartHour->setEnabled(on);
        ui->histStartMinute->setEnabled(on);
        ui->histStartSecond->setEnabled(on);
        ui->histEndHour->setEnabled(on);
        ui->histEndMinute->setEnabled(on);
        ui->histEndSecond->setEnabled(on);
    });

    connect(ui->histSelectDateBtn, &QPushButton::clicked,
            this, &AlarmLoggerWidget::onSelectDateClicked);
    connect(ui->histSearchBtn,     &QPushButton::clicked,
            this, &AlarmLoggerWidget::onSearchClicked);
    connect(ui->histPrevMatchBtn,  &QPushButton::clicked,
            this, &AlarmLoggerWidget::onPrevMatch);
    connect(ui->histNextMatchBtn,  &QPushButton::clicked,
            this, &AlarmLoggerWidget::onNextMatch);
    connect(ui->histPrevPageBtn,   &QPushButton::clicked,
            this, &AlarmLoggerWidget::onPrevPage);
    connect(ui->histNextPageBtn,   &QPushButton::clicked,
            this, &AlarmLoggerWidget::onNextPage);

    // 异步读取完成 → 充填记录
    connect(m_logic, &AlarmLogicSystem::readDateFinished,
            this, [this](const QList<QStringList> &rows) {
                m_histAllRecords.clear();
                if (rows.size() > 1)
                    m_histAllRecords = rows.mid(1);
                ui->histSelectDateBtn->setEnabled(true);
                applyTimeFilter();
            });

    // 异步扫描可用日期完成 → 弹出日历对话框
    connect(m_logic, &AlarmLogicSystem::availableDatesReady,
            this, [this](const QSet<QDate> &dates) {
                HistoryCalendarDialog *dlg = new HistoryCalendarDialog(this);
                dlg->setAvailableDates(dates);
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                connect(dlg, &HistoryCalendarDialog::dateSelected,
                        this, &AlarmLoggerWidget::onHistoryDateSelected);
                dlg->exec();
            });

    // 将 histPageBarLayout 包进一个 QWidget，方便整行显隐
    m_histPageBar = new QWidget;
    auto *vl = qobject_cast<QVBoxLayout *>(ui->tabHistory->layout());
    if (vl) {
        for (int i = 0; i < vl->count(); ++i) {
            if (vl->itemAt(i)->layout() == ui->histPageBarLayout) {
                vl->removeItem(vl->itemAt(i));
                m_histPageBar->setLayout(ui->histPageBarLayout);
                vl->insertWidget(i, m_histPageBar);
                break;
            }
        }
    }
    m_histPageBar->hide();

    const int colCount = m_headerConfig.headers.size();
    ui->histTable->setColumnCount(colCount);
    ui->histTable->setHorizontalHeaderLabels(m_headerConfig.headers);
    ui->histTable->verticalHeader()->setVisible(false);
    auto *hh = ui->histTable->horizontalHeader();
    for (int i = 0; i < colCount - 1; ++i)
        hh->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(colCount - 1, QHeaderView::Stretch);
}

void AlarmLoggerWidget::onSelectDateClicked()
{
    ui->histSelectDateBtn->setEnabled(false);
    m_logic->availableDatesAsync();  // 异步扫描，结果在 availableDatesReady 信号里处理
}

void AlarmLoggerWidget::onHistoryDateSelected(const QDate &date)
{
    if (!date.isValid()) return;

    const QString rootDir  = m_logic->rootDir();
    const QString monthPath = QDir(rootDir).filePath(date.toString(QStringLiteral("yyyyMM")));
    const QString dayPrefix = date.toString(QStringLiteral("dd")) + QStringLiteral("_");
    const QDir    monthDir(monthPath);
    QString filePath;
    if (monthDir.exists()) {
        const QStringList files = monthDir.entryList(
            QStringList() << dayPrefix + QStringLiteral("*.csv"), QDir::Files, QDir::Name);
        if (!files.isEmpty())
            filePath = monthDir.filePath(files.first());
    }

    if (filePath.isEmpty()) {
        ui->histFileEdit->setText(
            tr("未找到 %1 的日志文件").arg(date.toString(QStringLiteral("yyyy-MM-dd"))));
        m_histAllRecords.clear();
        m_histFiltered.clear();
        ui->histTable->setRowCount(0);
        updatePagination();
        return;
    }

    ui->histFileEdit->setText(date.toString(QStringLiteral("yyyy-MM-dd")));
    ui->histTable->setRowCount(0);  // 清空旧表，等待异步结果
    ui->histSelectDateBtn->setEnabled(false);
    m_logic->readDateAsync(date);   // 异步读取，结果在 readDateFinished 信号里处理
}

void AlarmLoggerWidget::applyTimeFilter()
{
    m_histFiltered.clear();

    if (!ui->histTimeRangeCheck->isChecked()) {
        m_histFiltered = m_histAllRecords;
    } else {
        const QTime startTime(ui->histStartHour->value(),
                              ui->histStartMinute->value(),
                              ui->histStartSecond->value());
        const QTime endTime(ui->histEndHour->value(),
                            ui->histEndMinute->value(),
                            ui->histEndSecond->value());

        const int col = m_headerConfig.headerToColumn(QStringLiteral("SendTime"));
        for (const QStringList &rec : m_histAllRecords) {
            if (col < 0 || col >= rec.size()) {
                m_histFiltered.append(rec);
                continue;
            }
            const QTime t = QDateTime::fromString(
                rec.at(col), QStringLiteral("yyyy-MM-dd HH:mm:ss")).time();
            if (t >= startTime && t <= endTime)
                m_histFiltered.append(rec);
        }
    }

    m_histCurrentPage  = 0;
    m_histMatchIndices.clear();
    m_histCurrentMatch = -1;
    ui->histMatchLabel->setText(QStringLiteral("0 / 0"));
    ui->histPrevMatchBtn->setEnabled(false);
    ui->histNextMatchBtn->setEnabled(false);

    renderHistoryPage(0);
}

void AlarmLoggerWidget::renderHistoryPage(int page)
{
    m_histCurrentPage = page;
    ui->histTable->setRowCount(0);

    const int start    = page * kHistPageSize;
    const int end      = qMin(start + kHistPageSize, m_histFiltered.size());
    const int colCount = ui->histTable->columnCount();

    const int colLevel    = headerToColumn(m_headerConfig.headers[0]);
    const int colResolved = headerToColumn(m_headerConfig.headers[4]);

    for (int i = start; i < end; ++i) {
        const QStringList &rec = m_histFiltered.at(i);
        const int row = ui->histTable->rowCount();
        ui->histTable->insertRow(row);
        for (int col = 0; col < colCount; ++col) {
            const QString text = col < rec.size() ? rec.at(col) : QString();
            auto *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);

            if (col == colLevel) {
                QFont f = item->font();
                f.setBold(true);
                item->setFont(f);
                if (text.compare(QStringLiteral("Error"), Qt::CaseInsensitive) == 0)
                    item->setForeground(QColor(220, 50, 50));
                else
                    item->setForeground(QColor(210, 140, 0));
            } else if (col == colResolved) {
                const bool yes = (text.compare(QStringLiteral("Yes"), Qt::CaseInsensitive) == 0);
                item->setText(yes ? QStringLiteral("✓") : QStringLiteral("✗"));
                item->setBackground(yes ? QColor(34, 139, 34) : QColor(200, 30, 30));
                item->setForeground(Qt::white);
            }

            ui->histTable->setItem(row, col, item);
        }
    }
    updatePagination();
}

void AlarmLoggerWidget::updatePagination()
{
    const int total   = m_histFiltered.size();
    const int pages   = (total + kHistPageSize - 1) / kHistPageSize;
    const int current = m_histCurrentPage + 1;  // 1-indexed

    const bool multiPage = (pages > 1);
    if (m_histPageBar) m_histPageBar->setVisible(multiPage);
    ui->histPrevPageBtn->setEnabled(m_histCurrentPage > 0);
    ui->histNextPageBtn->setEnabled(m_histCurrentPage < pages - 1);

    auto *layout = ui->histPageBtnsLayout;
    if (!layout) return;

    while (layout->count()) {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }

    if (pages == 0) return;

    // Build page sequence: 1 ... (cur-1) cur (cur+1) ... N
    QList<int> seq;
    seq << 1;
    const int lo = qMax(2, current - 1);
    const int hi = qMin(pages - 1, current + 1);
    if (lo > 2) seq << -1;
    for (int p = lo; p <= hi; ++p) seq << p;
    if (hi < pages - 1) seq << -1;
    if (pages > 1) seq << pages;

    for (int p : seq) {
        if (p == -1) {
            auto *btn = new QPushButton(QStringLiteral("..."));
            btn->setMaximumWidth(35);
            btn->setFlat(true);
            connect(btn, &QPushButton::clicked, this, [this, pages]() {
                bool ok;
                const int pg = QInputDialog::getInt(
                    this, tr("跳转页面"), tr("输入页码:"),
                    m_histCurrentPage + 1, 1, pages, 1, &ok);
                if (ok) goToPage(pg - 1);
            });
            layout->addWidget(btn);
        } else {
            auto *btn = new QPushButton(QString::number(p));
            btn->setMaximumWidth(35);
            btn->setCheckable(true);
            btn->setChecked(p == current);
            btn->setEnabled(p != current);
            if (p != current) {
                connect(btn, &QPushButton::clicked, this, [this, p]() {
                    goToPage(p - 1);
                });
            }
            layout->addWidget(btn);
        }
    }
}

void AlarmLoggerWidget::onPrevPage()  { goToPage(m_histCurrentPage - 1); }
void AlarmLoggerWidget::onNextPage()  { goToPage(m_histCurrentPage + 1); }

void AlarmLoggerWidget::goToPage(int page)
{
    const int pages = (m_histFiltered.size() + kHistPageSize - 1) / kHistPageSize;
    if (page < 0 || page >= pages) return;
    renderHistoryPage(page);
}

void AlarmLoggerWidget::onSearchClicked()
{
    applyTimeFilter();  // 刷新过滤结果

    const QString kw = ui->histSearchEdit->text().trimmed();
    m_histMatchIndices.clear();
    m_histCurrentMatch = -1;

    if (kw.isEmpty()) {
        ui->histMatchLabel->setText(QStringLiteral("0 / 0"));
        ui->histPrevMatchBtn->setEnabled(false);
        ui->histNextMatchBtn->setEnabled(false);
        return;
    }

    for (int i = 0; i < m_histFiltered.size(); ++i) {
        for (const QString &field : m_histFiltered.at(i)) {
            if (field.contains(kw, Qt::CaseInsensitive)) {
                m_histMatchIndices.append(i);
                break;
            }
        }
    }

    const bool has = !m_histMatchIndices.isEmpty();
    ui->histPrevMatchBtn->setEnabled(has);
    ui->histNextMatchBtn->setEnabled(has);
    ui->histMatchLabel->setText(QString(QStringLiteral("0 / %1")).arg(m_histMatchIndices.size()));

    if (has) jumpToMatch(0);
}

void AlarmLoggerWidget::onPrevMatch()
{
    if (m_histMatchIndices.isEmpty()) return;
    const int idx = (m_histCurrentMatch <= 0)
                    ? m_histMatchIndices.size() - 1
                    : m_histCurrentMatch - 1;
    jumpToMatch(idx);
}

void AlarmLoggerWidget::onNextMatch()
{
    if (m_histMatchIndices.isEmpty()) return;
    jumpToMatch((m_histCurrentMatch + 1) % m_histMatchIndices.size());
}

void AlarmLoggerWidget::jumpToMatch(int matchIdx)
{
    if (matchIdx < 0 || matchIdx >= m_histMatchIndices.size()) return;

    m_histCurrentMatch        = matchIdx;
    const int filteredIdx     = m_histMatchIndices.at(matchIdx);
    const int matchPage       = filteredIdx / kHistPageSize;
    const int rowInPage       = filteredIdx % kHistPageSize;

    if (matchPage != m_histCurrentPage)
        renderHistoryPage(matchPage);

    for (int col = 0; col < ui->histTable->columnCount(); ++col) {
        if (auto *item = ui->histTable->item(rowInPage, col))
            item->setBackground(QColor(173, 216, 230));
    }
    ui->histTable->scrollToItem(ui->histTable->item(rowInPage, 0));
    ui->histMatchLabel->setText(
        QString(QStringLiteral("%1 / %2"))
            .arg(matchIdx + 1).arg(m_histMatchIndices.size()));
}

QSet<QDate> AlarmLoggerWidget::collectAvailableDates() const
{
    return m_logic->availableDates();  // 保留同步版本备用
}
