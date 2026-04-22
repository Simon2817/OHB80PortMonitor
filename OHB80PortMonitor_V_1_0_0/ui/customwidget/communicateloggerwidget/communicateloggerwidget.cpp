#include "communicateloggerwidget.h"
#include "ui_communicateloggerwidget.h"
#include "commlogpagetablemodel.h"
#include "commlogtableitemdelegate.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include "historycalendardialog.h"

// =====================================================================
// 固定列头（qrcode 为第 0 列，用于行索引）
// =====================================================================
const QStringList CommunicateLoggerWidget::kLiveHeaders = {
    QStringLiteral("qrcode"),
    QStringLiteral("Time"),
    QStringLiteral("CommandId"),
    QStringLiteral("DurationMs"),
    QStringLiteral("Request"),
    QStringLiteral("Response")
};

// =====================================================================
// CommunicateLoggerWidget
// =====================================================================

CommunicateLoggerWidget::CommunicateLoggerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CommunicateLoggerWidget)
{
    ui->setupUi(this);

    m_lfs       = new CommLogicalFileSystem(this);
    m_liveModel = new CommLogPageTableModel(this);
    m_histModel = new CommLogPageTableModel(this);

    setupLiveTab();
    setupHistoryTab();
    setupConnections();
}

CommunicateLoggerWidget::~CommunicateLoggerWidget()
{
    delete ui;
}

void CommunicateLoggerWidget::setupConnections()
{
    // ---- CommLogicalFileSystem 信号 ----
    connect(m_lfs, &CommLogicalFileSystem::pageReady,
            this,  &CommunicateLoggerWidget::onPageReady);
    connect(m_lfs, &CommLogicalFileSystem::loadFailed,
            this,  &CommunicateLoggerWidget::onLoadFailed);
    connect(m_lfs, &CommLogicalFileSystem::logAppended,
            this,  &CommunicateLoggerWidget::onLogAppended);
    connect(m_lfs, &CommLogicalFileSystem::navigationUpdated,
            this,  &CommunicateLoggerWidget::onNavigationUpdated);
    connect(m_lfs, &CommLogicalFileSystem::historyReady,
            this,  &CommunicateLoggerWidget::onHistoryReady);
    connect(m_lfs, &CommLogicalFileSystem::availableDatesReady,
            this,  &CommunicateLoggerWidget::onAvailableDatesReady);

    // ---- 历史查询按钮 ----
    connect(ui->searchBtn,     &QPushButton::clicked, this, &CommunicateLoggerWidget::onSearchClicked);
    connect(ui->findPrevBtn,   &QPushButton::clicked, this, &CommunicateLoggerWidget::onFindPrev);
    connect(ui->findNextBtn,   &QPushButton::clicked, this, &CommunicateLoggerWidget::onFindNext);
    connect(ui->selectDateBtn, &QPushButton::clicked, this, &CommunicateLoggerWidget::onSelectDateClicked);
}

void CommunicateLoggerWidget::setupLiveTab()
{
    // 模型表头（实时 + 历史共用同一套 CSV 列）
    m_liveModel->setCsvHeaders(kLiveHeaders);
    m_histModel->setCsvHeaders(kLiveHeaders);

    // 默认初始化 80 行（qrcode 1~80）
    QStringList defaultQrcodes;
    defaultQrcodes.reserve(kDefaultRowCount);
    for (int i = 1; i <= kDefaultRowCount; ++i)
        defaultQrcodes << QString::number(i);
    initQrcodeList(defaultQrcodes);

    // 实时表格视图
    ui->tableView->setModel(m_liveModel);
    m_liveDelegate = new CommLogTableItemDelegate(ui->tableView);
    ui->tableView->setItemDelegate(m_liveDelegate);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setVisible(false);

    // 设置列宽
    setupTableColumnWidths(ui->tableView);
}

void CommunicateLoggerWidget::setupHistoryTab()
{
    setSelectedDate(QDate::currentDate());

    ui->timeFromHour->setValue(0);
    ui->timeFromMinute->setValue(0);
    ui->timeFromSecond->setValue(0);
    ui->timeToHour->setValue(23);
    ui->timeToMinute->setValue(59);
    ui->timeToSecond->setValue(59);

    ui->histtableView->setModel(m_histModel);
    m_histDelegate = new CommLogTableItemDelegate(ui->histtableView);
    ui->histtableView->setItemDelegate(m_histDelegate);
    ui->histtableView->horizontalHeader()->setStretchLastSection(true);
    ui->histtableView->verticalHeader()->setVisible(false);

    // 设置列宽
    setupTableColumnWidths(ui->histtableView);

    m_calendarDlg = new HistoryCalendarDialog(this);
    connect(m_calendarDlg, &HistoryCalendarDialog::dateSelected,
            this, [this](const QDate &date) { setSelectedDate(date); });
}

void CommunicateLoggerWidget::setupTableColumnWidths(QTableView *tableView)
{
    // 设置列宽比例：1:2:2.5:1:3.5:6 (qrcode:Time:CommandId:DurationMs:Request:Response)
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    tableView->setColumnWidth(0, 80);
    tableView->setColumnWidth(1, 160);
    tableView->setColumnWidth(2, 200);
    tableView->setColumnWidth(3, 80);
    tableView->setColumnWidth(4, 280);
    tableView->setColumnWidth(5, 480);
}

// -------- 配置 --------

void CommunicateLoggerWidget::setRootPath(const QString &path)
{
    m_rootPath = path;

    // 同步到所有依赖日志根目录的子组件
    if (m_lfs) m_lfs->setRootPath(path);
    // 未来若新增需要 rootPath 的子控件，在此统一分发
}
void CommunicateLoggerWidget::setPageSize(int size)             { m_lfs->setPageSize(size); }
void CommunicateLoggerWidget::setMaxFileBytes(qint64 bytes)     { m_lfs->setMaxFileBytes(bytes); }

void CommunicateLoggerWidget::setItemStyler(
    std::function<void(const QStringList &record, CommItemStyle &style)> fn)
{
    m_stylerFn = fn;
    m_liveDelegate->setStyler(fn);
    if (m_histDelegate) m_histDelegate->setStyler(fn);
    ui->tableView->viewport()->update();
}

QDate CommunicateLoggerWidget::selectedDate() const
{
    return QDate::fromString(ui->dateEdit->text(), "yyyy-MM-dd");
}

void CommunicateLoggerWidget::setSelectedDate(const QDate &d)
{
    ui->dateEdit->setText(d.toString("yyyy-MM-dd"));
}

// -------- 实时表格控制 --------

void CommunicateLoggerWidget::initQrcodeList(const QStringList &qrcodes)
{
    m_qrcodeRow.clear();
    m_liveRecords.clear();
    m_liveRecords.reserve(qrcodes.size());

    for (int i = 0; i < qrcodes.size(); ++i) {
        m_qrcodeRow[qrcodes[i]] = i;
        QStringList row;
        row.reserve(kLiveHeaders.size());
        row << qrcodes[i]; // 第 0 列：qrcode
        for (int col = 1; col < kLiveHeaders.size(); ++col)
            row << QString();
        m_liveRecords.append(row);
    }
    m_liveModel->setRecords(m_liveRecords);
}

// -------- 写入日志 --------

void CommunicateLoggerWidget::writeLog(const QJsonObject &record)
{
    // 持久化（异步，通过 LFS 写入磁盘）
    m_lfs->writeLog(record);

    // 按 qrcode 更新实时表格对应行
    QString qrcode = record.value(QStringLiteral("qrcode")).toString();
    auto it = m_qrcodeRow.constFind(qrcode);
    if (it == m_qrcodeRow.constEnd()) return;

    int row = it.value();
    for (int col = 0; col < kLiveHeaders.size(); ++col)
        m_liveRecords[row][col] = record.value(kLiveHeaders[col]).toString();

    m_liveModel->setRecords(m_liveRecords);
}

// -------- 初始化 --------

void CommunicateLoggerWidget::initialize()
{
    // 将固定列头同步到 LFS（决定 CSV 写入格式）
    m_lfs->setHeaders(kLiveHeaders);
    // 启动 LFS（用于历史查询）；onPageReady 不会更新实时视图
    m_lfs->initialize();
}

// -------- LFS 回调 --------

void CommunicateLoggerWidget::onPageReady(const CommPage &page, bool isPrev)
{
    // 实时视图由 writeLog() 直接维护，不从磁盘恢复
    Q_UNUSED(page)
    Q_UNUSED(isPrev)
}

void CommunicateLoggerWidget::onNavigationUpdated(bool hasPrev, bool hasNext)
{
    Q_UNUSED(hasPrev)
    Q_UNUSED(hasNext)
}

void CommunicateLoggerWidget::onLogAppended(bool success, bool pageChanged)
{
    Q_UNUSED(pageChanged)
    emit logWritten(success);
}

void CommunicateLoggerWidget::onLoadFailed(const QString &reason)
{
    Q_UNUSED(reason)
}

// =====================================================================
// 历史查询 Tab
// =====================================================================

void CommunicateLoggerWidget::onSearchClicked()
{
    CommHistoryQuery q;
    q.date      = selectedDate();
    q.pageSize  = 50;
    q.pageIndex = 0;

    if (ui->timeCheck->isChecked()) {
        q.timeColumnIndex = kLiveHeaders.indexOf(QStringLiteral("Time"));
        q.timeFrom = QString("%1:%2:%3")
                      .arg(ui->timeFromHour->value(),   2, 10, QLatin1Char('0'))
                      .arg(ui->timeFromMinute->value(), 2, 10, QLatin1Char('0'))
                      .arg(ui->timeFromSecond->value(), 2, 10, QLatin1Char('0'));
        q.timeTo   = QString("%1:%2:%3")
                      .arg(ui->timeToHour->value(),   2, 10, QLatin1Char('0'))
                      .arg(ui->timeToMinute->value(), 2, 10, QLatin1Char('0'))
                      .arg(ui->timeToSecond->value(), 2, 10, QLatin1Char('0'));
    }

    q.likePattern = ui->likeEdit->text().trimmed();

    m_lastQuery = q;
    m_histIsNewSearch = true;
    m_histCurrentMatchPos = -1;
    m_histMatchIndices.clear();
    m_histPendingScrollRow = -1;
    ui->searchBtn->setEnabled(false);
    ui->searchBtn->setText(tr("Searching..."));
    m_lfs->queryHistory(q);
}

int CommunicateLoggerWidget::getFilteredRow(int originalRow,
                                             const QVector<bool> &highlighted) const
{
    if (m_lastQuery.likePattern.isEmpty() || highlighted.isEmpty())
        return originalRow;
    int filteredRow = 0;
    for (int i = 0; i < originalRow && i < highlighted.size(); ++i) {
        if (highlighted[i]) ++filteredRow;
    }
    return filteredRow;
}

void CommunicateLoggerWidget::onHistoryReady(const CommHistoryResult &result)
{
    m_lastResult = result;
    ui->searchBtn->setEnabled(true);
    ui->searchBtn->setText(tr("Search"));

    bool doFilter = !m_lastQuery.likePattern.isEmpty();

    if (m_histIsNewSearch) {
        bool noRecords = (result.totalRecords == 0);
        bool likeEmpty = (doFilter && result.matchedGlobalIndices.isEmpty());
        if (noRecords || likeEmpty) {
            ui->findPrevBtn->setEnabled(false);
            ui->findNextBtn->setEnabled(false);
            QMessageBox::information(this, tr("Search Result"),
                noRecords ? tr("No records match the query")
                          : tr("No records match the keyword"));
            if (noRecords) return;
        }
    }

    if (doFilter && !result.highlighted.isEmpty()) {
        QVector<QStringList> filteredRecords;
        for (int i = 0; i < result.records.size(); ++i) {
            if (i < result.highlighted.size() && result.highlighted[i])
                filteredRecords.append(result.records[i]);
        }
        m_histModel->setRecords(filteredRecords);
        m_histModel->setHighlightMask({});
    } else {
        m_histModel->setRecords(result.records);
        m_histModel->setHighlightMask(result.highlighted);
    }

    if (m_histIsNewSearch) {
        m_histMatchIndices = result.matchedGlobalIndices;
        if (!m_histMatchIndices.isEmpty()) {
            m_histCurrentMatchPos = 0;
            int globalIdx  = m_histMatchIndices[0];
            int targetPage = globalIdx / m_lastQuery.pageSize;
            int targetRow  = globalIdx % m_lastQuery.pageSize;
            if (targetPage == result.currentPage) {
                int filteredRow = getFilteredRow(targetRow, result.highlighted);
                QModelIndex idx = m_histModel->index(filteredRow, 0);
                ui->histtableView->scrollTo(idx, QAbstractItemView::PositionAtCenter);
                ui->histtableView->setCurrentIndex(idx);
            } else {
                m_histPendingScrollRow = targetRow;
                onHistoryPageClicked(targetPage);
                return;
            }
        }
    }

    if (m_histPendingScrollRow >= 0 && m_histPendingScrollRow < result.records.size()) {
        int filteredRow = getFilteredRow(m_histPendingScrollRow, result.highlighted);
        if (filteredRow < m_histModel->rowCount()) {
            QModelIndex idx = m_histModel->index(filteredRow, 0);
            ui->histtableView->scrollTo(idx, QAbstractItemView::PositionAtCenter);
            ui->histtableView->setCurrentIndex(idx);
        }
        m_histPendingScrollRow = -1;
    }

    updateFindButtons();
    rebuildHistoryPageBar();
}

void CommunicateLoggerWidget::onHistoryPageClicked(int pageIndex)
{
    m_lastQuery.pageIndex = pageIndex;
    m_histIsNewSearch = false;
    ui->searchBtn->setEnabled(false);
    ui->searchBtn->setText(tr("Searching..."));
    m_lfs->queryHistory(m_lastQuery);
}

static const int PAGE_BTN_W = 36;
static const char *PAGE_BTN_NORMAL_SS  = "";
static const char *PAGE_BTN_CURRENT_SS =
    "QPushButton { background:#3080E8; color:white; font-weight:bold;"
    " border:1px solid #2060C0; border-radius:3px; }";

void CommunicateLoggerWidget::rebuildHistoryPageBar()
{
    QHBoxLayout *bar = ui->pageBarLayout;
    while (bar->count() > 0) {
        QLayoutItem *item = bar->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (m_lastResult.totalPages <= 1) return;

    int cur   = m_lastResult.currentPage;
    int total = m_lastResult.totalPages;

    auto makeBtn = [&](const QString &text, bool enabled, int page) -> QPushButton* {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedWidth(PAGE_BTN_W);
        btn->setEnabled(enabled);
        btn->setStyleSheet(page == cur ? PAGE_BTN_CURRENT_SS : PAGE_BTN_NORMAL_SS);
        if (enabled) {
            connect(btn, &QPushButton::clicked, this, [this, page]() {
                onHistoryPageClicked(page);
            });
        }
        return btn;
    };

    auto makeDots = [&](int leftPage, int rightPage) -> QPushButton* {
        QPushButton *dots = new QPushButton("...");
        dots->setFixedWidth(PAGE_BTN_W);
        dots->setCursor(Qt::PointingHandCursor);
        connect(dots, &QPushButton::clicked, this, [this, leftPage, rightPage]() {
            bool ok = false;
            int page = QInputDialog::getInt(this, tr("Jump to Page"),
                           tr("Enter page number (%1-%2):").arg(leftPage + 1).arg(rightPage + 1),
                           m_lastResult.currentPage + 1,
                           leftPage + 1, rightPage + 1, 1, &ok);
            if (ok) onHistoryPageClicked(page - 1);
        });
        return dots;
    };

    bar->addWidget(makeBtn(tr("<"), cur > 0, cur - 1));

    int start = qMax(0, cur - 3);
    int end   = qMin(total - 1, cur + 3);

    if (start > 0) {
        bar->addWidget(makeBtn("1", true, 0));
        if (start > 1) bar->addWidget(makeDots(1, start - 1));
    }
    for (int i = start; i <= end; ++i)
        bar->addWidget(makeBtn(QString::number(i + 1), (i != cur), i));
    if (end < total - 1) {
        if (end < total - 2) bar->addWidget(makeDots(end + 1, total - 2));
        bar->addWidget(makeBtn(QString::number(total), true, total - 1));
    }

    bar->addWidget(makeBtn(tr(">"), cur < total - 1, cur + 1));

    QLabel *info = new QLabel(tr("Page %1/%2, total %3 records")
                              .arg(cur + 1).arg(total).arg(m_lastResult.totalRecords));
    bar->addWidget(info);
    bar->addStretch();
}

void CommunicateLoggerWidget::onSelectDateClicked()
{
    ui->selectDateBtn->setEnabled(false);
    ui->selectDateBtn->setText(tr("Loading..."));
    m_lfs->requestAvailableDates();
}

void CommunicateLoggerWidget::onAvailableDatesReady(const QSet<QDate> &dates)
{
    ui->selectDateBtn->setEnabled(true);
    ui->selectDateBtn->setText(tr("Select Date"));
    m_calendarDlg->setAvailableDates(dates);
    m_calendarDlg->setSelectedDate(selectedDate());
    m_calendarDlg->exec();
}

void CommunicateLoggerWidget::onFindPrev()
{
    if (m_histMatchIndices.isEmpty()) return;
    if (m_histCurrentMatchPos > 0) --m_histCurrentMatchPos;
    else m_histCurrentMatchPos = m_histMatchIndices.size() - 1;
    navigateToGlobalIndex(m_histMatchIndices[m_histCurrentMatchPos]);
    updateFindButtons();
}

void CommunicateLoggerWidget::onFindNext()
{
    if (m_histMatchIndices.isEmpty()) return;
    if (m_histCurrentMatchPos < m_histMatchIndices.size() - 1) ++m_histCurrentMatchPos;
    else m_histCurrentMatchPos = 0;
    navigateToGlobalIndex(m_histMatchIndices[m_histCurrentMatchPos]);
    updateFindButtons();
}

void CommunicateLoggerWidget::navigateToGlobalIndex(int globalIdx)
{
    int targetPage = globalIdx / m_lastQuery.pageSize;
    int targetRow  = globalIdx % m_lastQuery.pageSize;

    if (targetPage == m_lastResult.currentPage) {
        int filteredRow = getFilteredRow(targetRow, m_lastResult.highlighted);
        QModelIndex idx = m_histModel->index(filteredRow, 0);
        ui->histtableView->scrollTo(idx, QAbstractItemView::PositionAtCenter);
        ui->histtableView->setCurrentIndex(idx);
    } else {
        m_histPendingScrollRow = targetRow;
        onHistoryPageClicked(targetPage);
    }
}

void CommunicateLoggerWidget::updateFindButtons()
{
    bool hasMatches = !m_histMatchIndices.isEmpty();
    ui->findPrevBtn->setEnabled(hasMatches);
    ui->findNextBtn->setEnabled(hasMatches);
}
