# OHB80PortMonitor
80port ohb 充氮设备监控上位机

## 项目文档
详细的项目框架文档请参阅：[PROJECT_STRUCTURE.md](./OHB80PortMonitor_V_1_0_0/docs/PROJECT_STRUCTURE.md)

---

## 更新日志

### 2026-04-17 19:30 - Simon
**调度系统完善与日志格式优化**

#### 修改内容
1. **InitCheckTask 初始化检查任务**
   - 创建 `scheduler/tasks/init_check_task.h` 和 `.cpp`
   - 普通任务，监听所有 ModbusTcpMaster 的 InitialCommandIssuer 的 `finished` 信号
   - 汇总所有设备的初始化结果，区分成功和失败设备
   - 发射 `allFinished` 信号，携带成功数、失败数和失败设备ID列表
   - 任务完成后自动释放，不占用调度器资源

2. **NetworkStatusTask 网络状态监控任务**
   - 创建 `scheduler/tasks/network_status_task.h` 和 `.cpp`
   - 长驻任务，监听所有 ModbusTcpMaster 的 ModbusConnecter 的 `statusChanged` 信号
   - 在 `start()` 方法中创建并启动 InitCheckTask，管理其生命周期
   - 启动所有 ModbusTcpMaster 的自动重连功能
   - 根据连接状态实时更新 FoupOfOHBInfo 的告警信息（alarmId=111）
   - 连接成功时清除告警，断开或出错时设置告警
   - 接收 InitCheckTask 的 `allFinished` 信号并转发为 `allInitFinished` 信号

3. **调度器初始化流程优化**
   - `App::initScheduler()` 在 `getSharedData()` 之后调用，确保 ModbusTcpMaster 实例已创建
   - 在 NetworkStatusTask 启动前提交任务，保证能够监控到底层 Master 类的初始化过程
   - 只提交 NetworkStatusTask，InitCheckTask 由 NetworkStatusTask 内部管理
   - 移除 SharedData 中的 ModbusTcpMaster 自动启动调用
   - 将 ModbusTcpMaster 启动控制权交给 NetworkStatusTask

4. **MonitorDataTask 完善**
   - 添加构造函数和析构函数日志，格式与 InitCheckTask 一致
   - 移除冗余的数据更新日志，减少日志输出量
   - 添加 PeriodicCommandSender 为空的详细日志提示

5. **日志格式统一优化**
   - **Scheduler 模块**：统一为 `[Scheduler][类名][方法名]` 格式
     - Scheduler 类：`[Scheduler][方法名]`（避免重复类名）
     - InitCheckTask：`[Scheduler][InitCheckTask][方法名]`
     - NetworkStatusTask：`[Scheduler][NetworkStatusTask][方法名]`
     - MonitorDataTask：`[Scheduler][MonitorDataTask][方法名]`
     - SendCommandTask：`[Scheduler][SendCommandTask][方法名]`
   
   - **UI 模块**：统一为 `[ui][类名][方法名]` 格式
     - GraphConfigParser：`[ui][GraphConfigParser][方法名]`
     - SetLevelGraphBuilder：`[ui][SetLevelGraphBuilder][方法名]`
     - FoupLevelGraphBuilder：`[ui][FoupLevelGraphBuilder][方法名]`
     - GraphAdjacencyList：`[ui][Graph][方法名]`
     - GraphAdjacencyMultilist：`[ui][Graph][方法名]`
   
   - **Data 模块**：统一为 `[data][类名][方法名]` 格式
     - ModbusTcpMasterManager：`[data][ModbusTcpMasterManager][方法名]`
     - ModbusTcpMasterPool：`[data][ModbusTcpMasterPool][方法名]`
     - ModbusTcpMaster：`[data][ModbusTcpMaster][方法名]`
     - ModbusConnecter：`[data][ModbusConnecter][方法名]`
     - ModbusCommandReceiver：`[data][ModbusCommandReceiver][方法名]`
     - ModbusCommandSender：`[data][ModbusCommandSender][方法名]`
     - ModbusConfigParser：`[data][ModbusConfigParser][方法名]`
   
   - **日志路径优化**：
     - 添加 `AppLogger::CraneMapLoggerPath()` 返回 `"ui/cranemap"`
     - 修改 `AppLogger::ModbusMasterLoggerPath()` 返回 `"data/modbustcpmaster/deviceid_%1"`
     - 天车轨道布局相关类统一使用 CraneMapLoggerPath

6. **界面刷新问题修复**
   - 解决 FrameDevice 控件背景颜色状态不刷新的问题
   - 确保设备状态变化时界面能够实时更新显示

7. **QString::arg() 歧义修复**
   - 在 `send_command_task.cpp` 中使用 `QString::fromLatin1()` 显式转换 QByteArray
   - 避免 QString::arg() 重载解析冲突

#### 技术细节
- **任务生命周期管理**：InitCheckTask 由 NetworkStatusTask 创建、启动和释放
- **信号转发机制**：NetworkStatusTask 接收 InitCheckTask 的 allFinished 信号并转发为 allInitFinished
- **线程安全**：所有信号连接使用 Qt::QueuedConnection 确保跨线程安全
- **日志分层**：按模块（Scheduler/ui/data）和类名分层，便于日志过滤和问题定位
- **启动顺序**：Scheduler → SharedData → NetworkStatusTask → InitCheckTask → ModbusTcpMaster

#### 数据流向
```
App::initialize()
  ↓ initScheduler()
Scheduler::start()
  ↓ submitTask(NetworkStatusTask)
NetworkStatusTask::start()
  ↓ create InitCheckTask
  ↓ start all ModbusTcpMaster
  ↓ connect statusChanged signals
InitCheckTask::start()
  ↓ connect InitialCommandIssuer::finished signals
  ↓ wait for all devices init
  ↓ emit allFinished
NetworkStatusTask::onInitCheckFinished()
  ↓ release InitCheckTask
  ↓ emit allInitFinished
```

#### 影响范围
- 新增文件：`scheduler/tasks/init_check_task.h`、`scheduler/tasks/init_check_task.cpp`
- 新增文件：`scheduler/tasks/network_status_task.h`、`scheduler/tasks/network_status_task.cpp`
- 修改文件：`app/app.cpp`（调整 initScheduler 调用顺序）
- 修改文件：`app/shareddata.cpp`（移除 ModbusTcpMaster 自动启动）
- 修改文件：`app/applogger.h`、`app/applogger.cpp`（添加日志路径方法）
- 修改文件：`scheduler/scheduler.cpp`（日志格式优化）
- 修改文件：`scheduler/tasks/monitor_data_task.cpp`（添加构造/析构日志，移除冗余日志）
- 修改文件：`scheduler/tasks/send_command_task.cpp`（修复 QString::arg 歧义）
- 修改文件：`ui/customwidget/overheadcranetrack/graphconfigparser.cpp`（日志格式优化）
- 修改文件：`ui/customwidget/overheadcranetrack/setlevelgraphbuilder.cpp`（日志格式优化）
- 修改文件：`ui/customwidget/overheadcranetrack/fouplevelgraphbuilder.cpp`（日志格式优化）
- 修改文件：`ui/customwidget/overheadcranetrack/graphadjacencylist.h`（日志格式优化）

---

### 2026-04-17 15:48 - Simon
**重大功能：调度层实现与 UI-Scheduler-Data 三层通路打通**

#### 修改内容
1. **Scheduler 调度器核心实现**
   - 创建 `scheduler/scheduler.h` 和 `scheduler.cpp`，实现单例模式的任务调度器
   - 支持长驻任务（persistent）和普通任务两种类型
   - 长驻任务不占用并发槽位，立即启动并持续运行
   - 普通任务进入队列，按并发数限制调度执行
   - 调度器运行在独立线程 `SchedulerThread`，避免阻塞主线程
   - 使用 `QMetaObject::invokeMethod` 的 functor 形式跨线程调用任务方法，避免 moc 名字查找问题

2. **SchedulerTask 任务基类**
   - 创建 `scheduler/scheduler_task.h`，定义任务生命周期接口
   - 任务状态：Pending、Running、Paused、Finished、Failed、Cancelled
   - 任务优先级：Low、Normal、High、Urgent
   - 提供 `start()`、`stop()`、`pause()`、`resume()` 虚方法供子类实现
   - 支持 `isPersistent()` 和 `isRecurring()` 属性标识
   - 信号：`stateChanged`、`progress`、`finished`、`dataResult`

3. **MonitorDataTask 监控数据任务**
   - 创建 `scheduler/tasks/monitor_data_task.h` 和 `.cpp`
   - 长驻任务，监听所有 ModbusTcpMaster 的 PeriodicCommandSender 的 `commandCompleted` 信号
   - 解析 ReadFoupStatus 响应帧（18字节，9个寄存器，大端序）
   - 通过 `SharedData::getFoupByQRCode()` 更新 FoupOfOHBInfo 实时数据
   - 支持跨线程信号连接（Qt::QueuedConnection）

4. **SendCommandTask 业务指令任务**
   - 创建 `scheduler/tasks/send_command_task.h` 和 `.cpp`
   - 普通任务，用于发送业务指令（如充氮、抽真空等）
   - 通过 `ModbusTcpMasterManager` 获取 ModbusCommandSender
   - 克隆指令模板，构建参数覆盖，提交高优先级指令
   - 监听 `commandFinished` 信号，解析响应并发射 `finished` 信号
   - 支持超时、重试、设备繁忙等异常处理

5. **SharedData 增强**
   - 添加 `getFoupByQRCode()` 静态方法，根据 qrCode 快速查找 FoupOfOHBInfo 指针
   - 遍历所有 SetOfOHBInfo 和 FoupOfOHBInfo，匹配 qrCode 并返回

6. **PeriodicCommandSender 信号增强**
   - `commandCompleted` 信号添加 `masterId` 参数，方便调度层直接使用
   - 构造函数中通过 lambda 转发 `commandSucceeded` 信号并附加 masterId

7. **ModbusCommandSender 信号增强**
   - `commandFinished` 信号添加 `masterId` 参数
   - 所有 `emit commandFinished` 调用均添加 `m_masterId` 参数

8. **App 初始化流程完善**
   - 在 `App::initialize()` 中添加 `initScheduler()` 调用
   - `App::initScheduler()` 启动调度器线程并打印日志
   - 确保调度器线程在任务提交前已启动

9. **HomePage 集成调度器**
   - 添加 `initScheduler()` 方法，创建并提交 MonitorDataTask
   - 在构造函数中调用 `initScheduler()`，启动实时数据监控

10. **日志系统统一**
    - `modbuscommandreceiver.cpp` 中所有日志路径获取从 `LoggerFilePathManager::` 改为 `AppLogger::`
    - 统一使用 `AppLogger::getModbusMasterLogPath()` 和 `AppLogger::getModbusFrameMessageLogPath()`

#### 技术细节
- **线程模型**：Scheduler 运行在独立线程，所有 Task 通过 `moveToThread` 移动到 Scheduler 线程执行
- **信号槽连接**：使用 `Qt::QueuedConnection` 确保跨线程安全
- **invokeMethod 优化**：使用 functor 形式 `[task]() { task->start(); }` 替代字符串 `"start"`，避免 moc 元信息查找失败
- **大端序解析**：MonitorDataTask 中使用 `(data[i] << 8) | data[i+1]` 解析 Modbus 大端序数据
- **UUID 追踪**：所有指令使用 `cmd.uuid` 在异步通信中追踪指令生命周期

#### 数据流向
```
UI (HomePage)
  ↓ submitTask(MonitorDataTask)
Scheduler
  ↓ invokeMethod(task->start)
MonitorDataTask
  ↓ connect(PeriodicCommandSender::commandCompleted)
Data Layer (ModbusTcpMaster)
  ↓ emit commandCompleted(cmd, masterId)
MonitorDataTask::onCommandCompleted
  ↓ parseReadFoupStatusResponse
  ↓ SharedData::getFoupByQRCode
  ↓ updateFoupInfo
SharedData (FoupOfOHBInfo)
```

#### 影响范围
- 新增文件：`scheduler/scheduler.h`、`scheduler/scheduler.cpp`
- 新增文件：`scheduler/scheduler_task.h`
- 新增文件：`scheduler/tasks/monitor_data_task.h`、`scheduler/tasks/monitor_data_task.cpp`
- 新增文件：`scheduler/tasks/send_command_task.h`、`scheduler/tasks/send_command_task.cpp`
- 修改文件：`app/app.h`、`app/app.cpp`（添加 initScheduler）
- 修改文件：`app/shareddata.h`、`app/shareddata.cpp`（添加 getFoupByQRCode）
- 修改文件：`ui/homepage.h`、`ui/homepage.cpp`（集成调度器）
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/periodiccommandsender.h`、`.cpp`（信号增强）
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/modbuscommandsender.h`、`.cpp`（信号增强）
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/modbuscommandreceiver.cpp`（日志统一）

---

### 2026-04-17 09:46 - Simon
**重大重构：配置系统优化与 Modbus 初始化改进**

#### 修改内容
1. **配置类重构**
   - 创建 `NetworkConfig` 类，管理 `network.ini` 配置文件（80个OHB的IP和端口）
   - 创建 `QRCodeConfig` 类，管理 `qrcode.ini` 配置文件（80个OHB的二维码映射）
   - 创建 `UserInfoConfig` 类，管理 `userinfo.ini` 配置文件
   - 从 `OHBNetworkInfo` 和 `OHBQRCodeInfo` 结构体中移除 `ohbIndex` 字段
   - 删除 `OHBQRCodeInfo` 结构体，改用 `QString` 直接存储二维码

2. **AppConfig 类增强**
   - 添加 `getNetworkConfigPath()`、`getQRCodeConfigPath()`、`getUserInfoConfigPath()` 方法
   - 添加 `getModbusConfigPath()` 方法，获取 Modbus 配置文件路径
   - 添加 `getNetworkConfig()`、`getQRCodeConfig()`、`getUserInfoConfig()` 方法获取配置类实例
   - 修复循环依赖问题

3. **SharedData 初始化优化**
   - 从配置文件读取 OHB 网络信息和二维码映射
   - 按数字顺序（OHB1-OHB80）读取配置，确保顺序正确
   - 为每个 Foup 创建独立的 ModbusTcpMaster 实例（共320个）
   - 添加 `foupIn` 和 `hasAlarm` 字段初始化
   - 移除 SharedData 中的 ModbusTcpMasterManager 初始化，改为在 App::initialize() 中加载

4. **配置文件更新**
   - `network.ini`：所有 OHB 的 IP 统一为 169.254.173.206，端口从 500-579
   - 配置读取添加详细的调试日志，打印每一条配置项

5. **App 初始化流程优化**
   - 在 `App::initialize()` 中添加 ModbusTcpMasterManager 配置文件加载
   - 使用 `AppConfig::getInstance().getModbusConfigPath()` 获取配置路径
   - 添加 `modbustcpmastermanager.h` 头文件引用

6. **CraneMapWidget 自动刷新**
   - 添加 QTimer 定时器，每隔1秒自动刷新设备数据
   - 在构造函数中初始化并启动定时器

7. **Modbus 日志系统整合**
   - 将 `modbustcpmastermanager` 模块中的所有日志引用从 `loggermanager.h` 改为 `applogger.h`
   - 修改文件包括：`modbustcpmastermanager.cpp`、`modbustcpmasterpool.cpp`、`modbuscommandreceiver.cpp`、`modbusconnecter.cpp`、`periodiccommandsender.cpp`、`initialcommandissuer.cpp`、`modbustcpmaster.cpp`、`modbuscommandsender.cpp`、`modbusconfigparser.cpp`
   - 统一使用 AppLogger 进行日志管理，确保日志路径一致性

#### 技术细节
- 配置读取顺序：使用 `for (int i = 1; i <= 80; ++i)` 确保按 OHB1-OHB80 顺序读取
- Modbus Master 创建：每个 Foup 使用独立的 IP、端口和二维码作为 Master ID
- 日志增强：在 `readNetworkConfig()`、`readQRCodeMapping()`、`readMasterDevices()` 中添加详细日志

#### 影响范围
- 新增文件：`app/networkconfig.h`、`app/networkconfig.cpp`
- 新增文件：`app/qrcodeconfig.h`、`app/qrcodeconfig.cpp`
- 新增文件：`app/userinfoconfig.h`、`app/userinfoconfig.cpp`
- 修改文件：`app/appconfig.h`、`app/appconfig.cpp`
- 修改文件：`app/shareddata.h`、`app/shareddata.cpp`
- 修改文件：`app/app.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/cranemapwidget.h`、`cranemapwidget.cpp`
- 修改文件：`bin/config/network.ini`
- 修改文件：`data/modbustcpmastermanager/` 目录下的 9 个 .cpp 文件（日志系统整合）

---

### 2026-04-16 19:20 - Simon
**功能增强：添加全局共享数据类**

#### 修改内容
1. **添加 SharedData 类**
   - 创建 `shareddata.h` 和 `shareddata.cpp`，实现全局共享内存管理
   - 包含静态成员变量 `setOfOHBInfoList`，存储 20 个 SetOfOHBInfo 对象
   - 提供 `getSetOfOHBInfoByUiId()` 方法，根据 uiId 获取对应的 SetOfOHBInfo 对象
   - 每个 SetOfOHBInfo 包含 4 个 FoupOfOHBInfo 对象

2. **App 类集成 SharedData**
   - 添加 `getSharedData()` 方法，在应用程序启动时初始化 SharedData
   - 在 `App::initialize()` 中调用 `getSharedData()` 确保共享数据初始化

3. **AppConfig 类增强**
   - 添加 `getGraphConfigPath()` 方法，获取天车地图配置文件路径
   - 配置文件路径：`bin/config/graph_config.xml`

4. **HomePage 类完善**
   - 添加四个初始化方法：`initUI()`、`initDeviceMonitorWidget()`、`initCraneMapWidget()`、`initOverheadCranesWidget()`
   - 在构造函数中调用 `initUI()` 初始化界面
   - 使用 `App::getConfig()->getGraphConfigPath()` 获取配置文件路径
   - 添加 `m_deviceMonitor` 成员变量

5. **修复编译错误**
   - 将 `setlevelgraphbuilder.cpp` 中的 `App::getSetOfOHBInfoByUiId` 改为 `SharedData::getSetOfOHBInfoByUiId`
   - 将 `fouplevelgraphbuilder.cpp` 中的 `App::getSetOfOHBInfoByUiId` 改为 `SharedData::getSetOfOHBInfoByUiId`
   - 将 `devicemonitorwidget.cpp` 中的 `App::setOfOHBInfoList` 改为 `SharedData::setOfOHBInfoList`
   - 所有相关文件添加 `#include "app/shareddata.h"` 头文件

#### 影响范围
- 新增文件：`app/shareddata.h`、`app/shareddata.cpp`
- 修改文件：`app/app.h`、`app/app.cpp`、`app/appconfig.h`、`app/appconfig.cpp`
- 修改文件：`ui/homepage.h`、`ui/homepage.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/setlevelgraphbuilder.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/fouplevelgraphbuilder.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/devicemonitorwidget.cpp`

---

### 2026-04-16 18:41 - Simon
**项目结构优化：添加 classes 目录**
- 新增 `classes/` 目录，用于存放数据信息相关的类文件

---

### 2026-04-16 18:00 - Simon
**编写项目框架文档**
- 创建 `PROJECT_STRUCTURE.md` 文档，详细介绍项目目录结构、bin 目录结构、app 模块实现及使用方法

---

### 2026-04-16 17:00 - Simon
**功能优化：日志系统重构**
- 日志目录拆分为调试日志和用户日志
- AppConfig 类增强：新增 `getRootDir()`、`getOSType()`、`getDebugLogDir()`、`getUserLogDir()` 方法
- 配置文件新增 `OSType` 配置项
- App 类更新：静态变量 `logDir` 拆分为 `debugLogDir` 和 `userLogDir`
- 完善代码注释

详细修改内容请参阅项目文档。
