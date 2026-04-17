#include "modbustcpmastermanager.h"
#include "modbusconfigparser.h"
#include "modbustcpmasterpool.h"
#include "commandpool.h"
#include "modbustcpmaster.h"
#include <QMetaObject>
#include <QDebug>
#include "loggermanager.h"
#include "app/applogger.h"

ModbusTcpMasterManager& ModbusTcpMasterManager::instance()
{
    static ModbusTcpMasterManager manager;
    return manager;
}

ModbusTcpMasterManager::ModbusTcpMasterManager(QObject* parent)
    : QObject(parent)
    , m_configParser(new ModbusConfigParser())
    , m_masterPool(new ModbusTcpMasterPool(this))
    , m_commandPool(new CommandPool())
{
    qDebug() << "[ModbusTcpMasterManager] 已创建管理器";
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 已创建管理器").toStdString());
}

ModbusTcpMasterManager::~ModbusTcpMasterManager()
{
    qDebug() << "[ModbusTcpMasterManager] 析构管理器";
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 析构管理器").toStdString());

    delete m_configParser;
    delete m_commandPool;
    // m_masterPool 的父对象是 this，会自动删除
}

bool ModbusTcpMasterManager::loadConfig(const QString& xmlFilePath)
{
    qDebug() << "[ModbusTcpMasterManager] 开始加载配置文件:" << xmlFilePath;
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 开始加载配置文件：%1").arg(xmlFilePath).toStdString());

    if (!m_configParser->parse(xmlFilePath)) {
        qWarning() << "[ModbusTcpMasterManager] 配置文件解析失败:" << m_configParser->errorMessage();
        return false;
    }

    // 更新指令池
    *m_commandPool = m_configParser->pool();

    // 设置配置解析器到 MasterPool
    m_masterPool->setConfigParser(m_configParser);

    qDebug() << "[ModbusTcpMasterManager] 配置文件加载成功";
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 配置文件加载成功").toStdString());
    return true;
}

void ModbusTcpMasterManager::setThreadCount(int threadNum)
{
    m_masterPool->setThreadCount(threadNum);
}

void ModbusTcpMasterManager::setThreadCount(ModbusTcpMasterPool::ThreadCountMode mode)
{
    m_masterPool->setThreadCount(mode);
}

ModbusTcpMaster* ModbusTcpMasterManager::addMaster(const QString& ip, quint16 port, const QString& id)
{
    qDebug() << "[ModbusTcpMasterManager] 创建 Master (ID:" << id << ", IP:" << ip << ", Port:" << port << ")";
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 创建 Master (ID:%1, IP:%2, Port:%3)").arg(id).arg(ip).arg(port).toStdString());
    return m_masterPool->addMaster(ip, port, id);
}

ModbusTcpMaster* ModbusTcpMasterManager::getMaster(const QString& id) const
{
    return m_masterPool->getMaster(id);
}

bool ModbusTcpMasterManager::startMaster(const QString& id, ModbusConnecter::ConnectionMode mode)
{
    ModbusTcpMaster* master = m_masterPool->getMaster(id);
    if (!master) {
        qWarning() << "[ModbusTcpMasterManager] 未找到要启动的 Master, ID:" << id;
        return false;
    }

    QMetaObject::invokeMethod(master, [master, mode]() {
        master->start(mode);
    }, Qt::QueuedConnection);

    qDebug() << "[ModbusTcpMasterManager] 已投递启动请求, ID:" << id;
    LoggerManager::instance().log(AppLogger::getDataLayerLogPath().toStdString(), Level::INFO, QString("[ModbusTcpMasterManager] 已投递启动请求, ID:%1").arg(id).toStdString());
    return true;
}

int ModbusTcpMasterManager::threadCount() const
{
    return m_masterPool->threadCount();
}

int ModbusTcpMasterManager::masterCount() const
{
    return m_masterPool->masterCount();
}

QStringList ModbusTcpMasterManager::masterIds() const
{
    return m_masterPool->masterIds();
}

ModbusConfigParser* ModbusTcpMasterManager::configParser() const
{
    return m_configParser;
}

ModbusTcpMasterPool* ModbusTcpMasterManager::masterPool() const
{
    return m_masterPool;
}

CommandPool* ModbusTcpMasterManager::commandPool() const
{
    return m_commandPool;
}
