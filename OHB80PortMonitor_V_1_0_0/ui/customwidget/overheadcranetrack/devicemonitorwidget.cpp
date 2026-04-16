#include "devicemonitorwidget.h"
#include "cranemapwidget.h"
#include "app/shareddata.h"

Graph::DeviceMonitorWidget::DeviceMonitorWidget(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_refreshTimer(new QTimer(this))
    , m_selectedDeviceType(FrameDevice::DeviceType::Foup)
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(2);
    setLayout(m_layout);

    initTables();

    connect(m_refreshTimer, &QTimer::timeout, this, &DeviceMonitorWidget::refreshData);
}

Graph::DeviceMonitorWidget::~DeviceMonitorWidget()
{
    m_refreshTimer->stop();
}

void Graph::DeviceMonitorWidget::bindCraneMapWidget(CraneMapWidget* widget)
{
    if (!widget) return;
    connect(widget, &CraneMapWidget::deviceSelected,
            this, &DeviceMonitorWidget::onDeviceSelected);
}

void Graph::DeviceMonitorWidget::setRefreshInterval(int ms)
{
    if (ms > 0) {
        m_refreshTimer->setInterval(ms);
    }
}

void Graph::DeviceMonitorWidget::startRefresh()
{
    if (!m_refreshTimer->isActive()) {
        m_refreshTimer->start(m_refreshTimer->interval() > 0 ? m_refreshTimer->interval() : 1000);
    }
}

void Graph::DeviceMonitorWidget::stopRefresh()
{
    m_refreshTimer->stop();
}

void Graph::DeviceMonitorWidget::onDeviceSelected(FrameDevice::DeviceType deviceType,
                                                   QSharedPointer<SetOfOHBInfo> setInfo,
                                                   QSharedPointer<FoupOfOHBInfo> foupInfo)
{
    m_selectedDeviceType = deviceType;
    m_monitoredFoupInfos.clear();

    if (deviceType == FrameDevice::DeviceType::Foup && foupInfo) {
        // 从全局变量中查找对应的 FoupOfOHBInfo 指针
        for (auto& globalSet : SharedData::setOfOHBInfoList) {
            QVector<FoupOfOHBInfo>& foups = globalSet.getFoups();
            for (auto& foup : foups) {
                if (foup.qrCode == foupInfo->qrCode) {
                    m_monitoredFoupInfos.append(&foup);
                    break;
                }
            }
            if (!m_monitoredFoupInfos.isEmpty()) break;
        }
    } else if (deviceType == FrameDevice::DeviceType::Set && setInfo) {
        // 从全局变量中查找对应的 Set，获取其所有 Foup 指针
        for (auto& globalSet : SharedData::setOfOHBInfoList) {
            if (globalSet.getUiId() == setInfo->getUiId()) {
                QVector<FoupOfOHBInfo>& foups = globalSet.getFoups();
                for (auto& foup : foups) {
                    m_monitoredFoupInfos.append(&foup);
                }
                break;
            }
        }
    }

    // 立即刷新一次
    refreshData();
}

void Graph::DeviceMonitorWidget::refreshData()
{
    m_tableManager.hideAll();

    if (m_monitoredFoupInfos.isEmpty()) {
        m_tableManager.syncVisibility();
        return;
    }

    if (m_selectedDeviceType == FrameDevice::DeviceType::Foup) {
        refreshFoupMonitor();
        m_tableManager.setVisible("FoupMonitor", true);
        m_tableManager.setVisible("FoupPurgeTimeMonitor", true);
    } else if (m_selectedDeviceType == FrameDevice::DeviceType::Set) {
        refreshSetMonitor();
        m_tableManager.setVisible("SetMonitor", true);
    }

    m_tableManager.syncVisibility();
}

void Graph::DeviceMonitorWidget::initTables()
{
    // 第一个表格：FoupMonitor
    QTableWidget* foupTable = m_tableManager.addTable(
        "FoupMonitor",
        {"QRCode", "InletPressure", "InletFlow", "Relative Humidity"},
        this);
    m_layout->addWidget(foupTable);
    m_tableManager.setAlignment("FoupMonitor", Qt::AlignCenter);

    // 第二个表格：FoupPurgeTimeMonitor
    QTableWidget* purgeTable = m_tableManager.addTable(
        "FoupPurgeTimeMonitor",
        {"Start Time", "Duration", "Purge Time", "Idle Time"},
        this);
    m_layout->addWidget(purgeTable);
    m_tableManager.setAlignment("FoupPurgeTimeMonitor", Qt::AlignCenter);

    // 第三个表格：SetMonitor
    QTableWidget* setTable = m_tableManager.addTable(
        "SetMonitor",
        {"QRCode", "InletPressure", "InletFlow", "Relative Humidity"},
        this);
    m_layout->addWidget(setTable);
    m_tableManager.setAlignment("SetMonitor", Qt::AlignCenter);
}

void Graph::DeviceMonitorWidget::refreshFoupMonitor()
{
    if (m_monitoredFoupInfos.isEmpty()) return;

    const FoupOfOHBInfo* foup = m_monitoredFoupInfos.first();

    // FoupMonitor 表
    QStringList rowData;
    rowData << foup->qrCode
            << QString::number(foup->inletPressure, 'f', 2) + " Mpa"
            << QString::number(foup->inletFlow, 'f', 2) + " L/Min"
            << QString::number(foup->RH, 'f', 2) + " %";
    m_tableManager.updateFirstRow("FoupMonitor", rowData);

    // FoupPurgeTimeMonitor 表
    quint32 durationMs = foup->purgeTimeMs + foup->idleTimeMs;
    double durationMin = durationMs / 60000.0;  // 毫秒转分钟

    // 将毫秒转换为 hh:mm:ss 格式
    auto formatTime = [](quint32 ms) -> QString {
        int totalSeconds = ms / 1000;
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;
        return QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    };

    QStringList purgeRowData;
    purgeRowData << foup->startTime.toString("HH:mm:ss.zzz")
                 << QString::number(durationMin, 'f', 1) + " Min"
                 << formatTime(foup->purgeTimeMs)
                 << formatTime(foup->idleTimeMs);
    m_tableManager.updateFirstRow("FoupPurgeTimeMonitor", purgeRowData);
}

void Graph::DeviceMonitorWidget::refreshSetMonitor()
{
    if (m_monitoredFoupInfos.isEmpty()) return;

    QVector<QStringList> rows;
    rows.reserve(m_monitoredFoupInfos.size());
    for (const FoupOfOHBInfo* foup : m_monitoredFoupInfos) {
        QStringList rowData;
        rowData << foup->qrCode
                << QString::number(foup->inletPressure, 'f', 2) + " Mpa"
                << QString::number(foup->inletFlow, 'f', 2) + " L/Min"
                << QString::number(foup->RH, 'f', 2) + " %";
        rows.append(rowData);
    }
    m_tableManager.updateRows("SetMonitor", rows);
}
