# OHB80PortMonitor
80port ohb 充氮设备监控上位机

## 项目文档
详细的项目框架文档请参阅：[PROJECT_STRUCTURE.md](./OHB80PortMonitor_V_1_0_0/docs/PROJECT_STRUCTURE.md)

---

## 更新日志

### 2026-04-27 16:50 - Simon
**配置文件更新：设备列表扩展与气控阀压力指令**

#### 修改内容

**1. ModbusTcpMasterConfig.xml 新增指令**
- 添加 `WritePneumaticValvePressure` 指令（功能码 06）
- 寄存器地址：0x0002
- 用于设置气控阀压力值
- 请求帧：`01 06 00 02 XX XX CRC`
- 响应帧：`01 06 00 02 XX XX CRC`（镜像返回）
- 值计算：`Value = (256 * hi_byte + lo_byte)`

**2. qrcode.ini 设备列表扩展**
- MasterDevices list 新增设备 ID：1220、1240、1270
- 原设备列表：12001, 12010
- 扩展后设备列表：12001, 12010, 1220, 1240, 1270

#### 影响范围
- 修改文件：`bin/config/ModbusTcpMasterConfig.xml`（添加 WritePneumaticValvePressure 指令）
- 修改文件：`bin/config/qrcode.ini`（扩展 MasterDevices list）

---

### 2026-04-27 11:30 - Simon
**固件升级：Modbus RTU CRC 字节序修复与设备重启等待时间配置**

#### 修改内容

**1. Modbus RTU CRC 字节序修复**
- 修复 `handleVersionResponse` 中 CRC 校验字节序错误
- Modbus RTU 协议标准：CRC 以低字节在前、高字节在后传输
- 原代码 `receivedCrc = (resp[5] << 8) | resp[6]` 字节序反了
- 修正为 `receivedCrc = (resp[6] << 8) | resp[5]`
- 响应帧无论 CRC 校验成功或失败都打印完整帧内容

**2. 数据传输后等待设备重启配置**
- 将 `handleTransferResponse` 中的 `postTransferWaitMs` 局部变量改为成员变量 `m_postTransferWaitMs`
- 添加 `setPostTransferWaitTime(int ms)` 方法，允许外部设置等待时间
- 默认值：2000ms
- 数据传输完成后等待设备重启/加载新固件，避免版本查询超时

**3. 调度层接口扩展**
- `SetFirmwareConfigTask` 添加 `setPostTransferWaitTime(int ms)` 方法
- 添加 `postTransferWaitTimeApplied()` 信号
- 添加成员变量 `m_postTransferWaitTime`
- 在 `start()` 中将配置应用到所有设备的 FirmwareUpgrader

**4. UI 层设置项添加**
- `FirmwareUpdateConfigSettingWidget` 添加 "Post-Transfer Wait Time" 设置项
- 新增控件 `m_postTransferWaitTimeSpinBox` 和 `m_postTransferWaitTimeItem`
- 添加 `initPostTransferWaitTimeItem()` 初始化方法
- 添加 `onPostTransferWaitTimeSetBtnClicked()` 槽函数
- 范围：0-60000ms

**5. FirmwareConfig 配置支持**
- 添加 `postTransferWaitMs()` getter
- 添加 `setPostTransferWaitMs()` setter
- 配置键：`PostTransferWaitTimeMs`
- 默认值：2000ms
- 支持从 `bin/config/firmware.ini` 加载和保存配置

#### 技术细节
- **Modbus RTU CRC 标准**：CRC16 校验码在传输时低字节在前、高字节在后
- **设备重启等待**：固件数据传输完成后，设备需要时间重启并加载新固件，此时发送版本查询会超时
- **配置层级**：UI → Scheduler → Data 层，三层配置传递链路

#### 影响范围
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/firmwareupgrader.h`（添加成员变量和 setter）
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/firmwareupgrader.cpp`（修复 CRC 字节序，使用成员变量）
- 修改文件：`scheduler/tasks/set_firmware_config_task.h`（添加方法和信号）
- 修改文件：`scheduler/tasks/set_firmware_config_task.cpp`（实现方法和应用配置）
- 修改文件：`ui/customwidget/debugsettingwidget/firmwareupdateconfigsettingwidget.h`（添加控件和方法声明）
- 修改文件：`ui/customwidget/debugsettingwidget/firmwareupdateconfigsettingwidget.cpp`（实现初始化和槽函数）
- 修改文件：`config/firmwareconfig.h`（添加 getter/setter）
- 修改文件：`config/firmwareconfig.cpp`（实现 getter/setter，加载/保存配置）

---

### 2026-04-27 09:00 - Simon
**内存泄漏修复与 UI 线程阻塞优化**

#### 问题背景
软件运行后内存持续增长至 5468MB，同时频繁写入运行日志时 UI 线程出现明显卡顿。

#### 根因分析

**内存泄漏 1（严重）— LogicalFileSystem 防抖永不触发**
- `LogicalFileSystem::writeLog()` 每次调用都重置 50ms single-shot 定时器
- 80 设备持续写入（间隔 < 50ms）→ 定时器永远不超时 → `m_pendingLogs`（`QVector<QJsonObject>`）无限增长
- 该 bug 同时影响 `RunningLoggerWidget` 和 `LoggerWidget`

**内存泄漏 2（严重）— CommLogicalFileSystem 事件队列无界膨胀**
- `CommLogicalFileSystem::writeLog()` 每次直接 emit 跨线程信号 `_requestAppendLog`
- Qt 为每次 emit 创建 `QMetaCallEvent`（携带 6 个 QString 拷贝）投递到 worker 线程
- worker 每次处理需 CSV 追加写 + 页表重建 + 页面读取，80 设备 × ~5 命令/秒 ≈ 400 事件/秒
- worker I/O 跟不上 → Qt 事件队列无限膨胀（每小时增长数百 MB）

**内存泄漏 3（中等）— 历史查询缓存容量不符**
- `CommLogFileSystem::m_queryCache{3}` 注释明确写"容量必须为 1"，实际为 3
- 每条缓存存整天 CSV 所有行（`QVector<QStringList>`），单条可达数百 MB

**UI 阻塞 1 — refreshDisplayText 每条记录重绘**
- `RunningLoggerWidget::writeRecord()` 每条都调用 `refreshDisplayText()` → `m_btn->setText()` → 触发 repaint
- 每 50ms 处理 20 条 = 400 次/秒按钮重绘

**UI 阻塞 2 — 归还剩余条目的 O(n²) 循环**
- `onFlushTick()` 先 swap 整个队列（可能 10000+ 条），只消费 20 条后归还剩余
- `QQueue::prepend()` 底层 `QList::prepend()` = O(n) memmove，9980 次循环 = O(n²)

**UI 阻塞 3 — Collector 队列无上限**
- `RunningLoggerCollector::log()` 无上限 enqueue → 连接风暴时队列无限膨胀

#### 修改内容

**1. LogicalFileSystem 防抖修复**
- `writeLog()` 添加 `kMaxPendingBatch = 200` 上限保护
- 达到上限时立即 `flushPendingLogs()`，不再等定时器
- 低频写入仍走 50ms 防抖合并

**2. CommLogicalFileSystem 批量缓冲改造**
- `writeLog()` 改为主线程侧缓冲到 `QVector<QStringList>`
- 50ms 防抖 + 200 条上限保护，批量 flush 到 worker
- `CommLogFileSystem` 新增 `requestAppendBatch(const QVector<QStringList> &)` 槽
- 一次批量 append 只做一次页表重建和页面读取（原先每条一次）
- 析构函数 `BlockingQueuedConnection` flush 残留缓冲

**3. 历史查询缓存容量修正**
- `m_queryCache{3}` → `m_queryCache{1}`

**4. RunningLoggerWidget 批量写入模式**
- 新增 `beginBatchWrite()` / `endBatchWrite()` 接口
- 批量模式下 `writeRecord()` 跳过 `refreshDisplayText()`
- 仅在 `endBatchWrite()` 时统一刷新一次按钮文本
- 400 次/秒 repaint → 20 次/秒

**5. RunningLoggerCollector 重写**
- `onFlushTick()` 只 dequeue 需要的条数，消除 swap + prepend-back 的 O(n²) 开销
- 使用 `beginBatchWrite()` / `endBatchWrite()` 包裹批量写入
- `log()` 添加 `kMaxQueueSize = 2000` 上限，超出丢弃最旧条目

#### 性能对比
| 指标 | 优化前 | 优化后 |
|---|---|---|
| 运行日志内存增长 | 无限（防抖永不触发） | 有界（≤200 条 flush） |
| 通讯日志事件队列 | 无限（逐条 emit） | 有界（≤200 条批量） |
| 历史查询缓存 | ≤3 天数据常驻 | ≤1 天数据常驻 |
| 按钮重绘频率 | 400 次/秒 | 20 次/秒 |
| Collector flush 复杂度 | O(n²) | O(k)，k=每帧消费数 |

#### 影响范围
- 修改文件：`ui/customwidget/loggerwidget/logicalfilesystem.h`（添加 kMaxPendingBatch）
- 修改文件：`ui/customwidget/loggerwidget/logicalfilesystem.cpp`（writeLog 添加上限保护）
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogicalfilesystem.h`（添加 batch 信号/slot/定时器/缓冲区）
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogicalfilesystem.cpp`（writeLog 改为缓冲 + 防抖；析构 flush）
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogfilesystem.h`（添加 requestAppendBatch；m_queryCache 容量 3→1）
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogfilesystem.cpp`（实现 requestAppendBatch）
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.h`（添加 beginBatchWrite/endBatchWrite）
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.cpp`（实现批量写入模式）
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggercollector.h`（添加 kMaxQueueSize）
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggercollector.cpp`（重写 onFlushTick；log 添加队列上限）

---

### 2026-04-24 10:50 - Simon
**Modbus 扩展：FOUP 属性与 Idle Purge 状态查询**

#### 主要完成的任务

**1. XML 配置扩展**
- 添加 7 条新 Modbus 指令定义到 `ModbusTcpMasterConfig.xml`
- ReadFoupStatus：FOUP 属性查询（进气压力、负压、流量、湿度、温度、FOUP 在位状态、充气时间）
- ReadIdlePurgeEnable：Idle Purge 使能查询
- ReadIdlePurgeStatus：Idle Purge 状态查询
- ReadIdlePurgeWorkingTime：Idle Purge 工作计时时间查询
- WriteIdlePurgeEnable：Idle Purge 使能写入
- WriteIdlePurgeTime：Idle Purge 时间写入
- WriteIdlePurgeInterval：Idle Purge 间隔写入

**2. 定时查询指令配置**
- 设置 `PeriodicCommandSender` 间隔为 300ms
- 添加 ReadFoupStatus、ReadIdlePurgeEnable、ReadIdlePurgeStatus、ReadIdlePurgeWorkingTime 到定时查询列表
- 定时查询逻辑：仅在 FOUP in 状态时下发 idle purge 相关查询指令

**3. Data 层：指令解析与响应处理**
- 创建 `CommandResponseParser` 类，实现模块化的 Modbus 响应解析
- 为 ReadFoupStatus、ReadIdlePurgeEnable、ReadIdlePurgeStatus、ReadIdlePurgeWorkingTime 注册解析函数
- 修复非标准设备响应处理：
  - ReadFoupStatus 响应函数码为 0x12（非标准），XML 配置已更新
  - 接收器帧长计算：当响应 FC ≠ 请求 FC 时，使用 XML 配置的 byteCount 而非帧内 BC 字段
  - fillResponseFrame 补充 case 0x12 分支，按读寄存器格式提取 registerValue

**4. Scheduler 层：数据更新与状态管理**
- MonitorDataTask 解析所有 4 条查询指令的响应
- 将解析结果写入全局共享变量 FoupOfOHBInfo
- 实现 FOUP 状态变化逻辑：
  - **FOUP out → in**：设置 startTime 为当前时间，idleState 置为 Idle，idleWorkingTimeSec 归 0
  - **FOUP in → out**：purgeTimeSec 和 startTime 归 0
  - **Idle Purge 禁用**：idleState 置为 Idle，idleWorkingTimeSec 归 0

**5. 全局共享变量扩展（FoupOfOHBInfo）**
- 新增字段：
  - `negativePressure`（double）：负压值
  - `idlePurgeEnabled`（bool）：Idle Purge 使能状态
  - `idleState`（enum）：Idle Purge 状态（Stopped、Preparing、Purging、Idle）
  - `idleWorkingTimeSec`（quint16）：Idle Purge 工作计时时间（秒）
- 重命名字段：
  - `purgeTimeMs` → `purgeTimeSec`（单位改为秒）
  - `idleTimeMs` → `idleWorkingTimeSec`（单位改为秒）

**6. UI 层：监控表格扩展**
- FoupMonitor 表（第一个表格）：
  - 添加 "Vacuum"（负压）列，显示负压值并加负号前缀（例如：-0.05 Bar）
  - 列头 "Humidity" 改为 "Relative Humidity"
- FoupPurgeTimeMonitor 表（第二个表格）：
  - 添加 "Idle Enable"、"Idle State"、"IdleTime" 列
  - 列顺序：Start Time / Duration / Purge Time / Idle Enable / Idle State / IdleTime
  - Duration：purgeTimeSec 转分钟显示（例如：5Min）
  - Idle State：枚举值转中文显示
  - IdleTime：idleWorkingTimeSec 转 hh:mm:ss 格式
- SetMonitor 表（第三个表格）：
  - 同步添加 "Vacuum" 列和 "Relative Humidity" 列头

**7. 代码国际化**
- RunningLoggerWidget 中文字符串改为英文：
  - "暂无日志" → "No logs"
  - "日志查看器" → "Log Viewer"

#### 技术细节
- **非标准设备处理**：本设备对 FC=0x04 请求返回 FC=0x12 响应，需要特殊处理帧长计算和数据提取
- **状态机管理**：FOUP 在位状态变化时自动重置相关计时字段，避免数据不一致
- **定时查询优化**：Idle purge 查询仅在 FOUP in 时执行，减少无效通信

#### 影响范围
- 修改文件：`bin/config/ModbusTcpMasterConfig.xml`
- 新增文件：`data/modbustcpmastermanager/modbuscommand/commandresponseparser.h`、`.cpp`
- 修改文件：`data/modbustcpmastermanager/modbuscommand/modbuscommand.pri`
- 修改文件：`data/modbustcpmastermanager/modbustcpmaster/modbuscommandreceiver.cpp`
- 修改文件：`classes/foupofohbinfo.h`、`.cpp`
- 修改文件：`scheduler/tasks/monitor_data_task.h`、`.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/devicemonitorwidget.cpp`
- 修改文件：`ui/customwidget/overheadcranetrack/framedevice.cpp`
- 修改文件：`scheduler/tasks/network_status_task.cpp`
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.cpp`

---

### 2026-04-24 09:00 - Simon
**关机崩溃修复：ModbusTcpMasterPool 与 Scheduler 线程清理**

#### 问题描述
应用关机时在 `ModbusTcpMasterPool` 析构阶段发生崩溃，日志显示"清理线程"后立即崩溃。根本原因是 **竞态条件**：
- `clear()` 用 `QueuedConnection` 异步投递 `stop() + deleteLater()` 到 22 个工作线程
- 线程清理是串行的：当 thread 0 被 quit/wait/delete 时，其他线程仍在处理 `clear()` 投递的事件
- 二次 `stop()` 触发信号级联（`disconnectDevice` → `setStatus` → `statusChanged` → `onConnectionStatusChanged`），导致跨线程访问已销毁对象

#### 修改内容

**1. ModbusTcpMasterPool 析构函数重写**
- **第一步**：`stopAllMasters()` — 用 `BlockingQueuedConnection` 同步停止所有 Master（保留不变）
- **第二步**：直接 `deleteLater()` 所有 Master — 去掉冗余的二次 `stop()` 和 `QueuedConnection` lambda
- **第三步**：并行 `quit()` 所有线程 — 避免串行 quit 时部分线程仍在处理事件的竞态
- **第四步**：逐一 `wait()` 线程 — 确保干净退出
- **移除**：500ms sleep（同步调用无需额外等待）

**关键改进**：
```
修改前：stopAllMasters() → sleep(500) → clear() → 串行 quit/wait/delete 线程
修改后：stopAllMasters() → deleteLater() → 并行 quit() → 逐一 wait()
```

**2. Scheduler::stop() 跨线程安全修复**
- 问题：`qDeleteAll(m_tasks)` 在主线程直接 `delete` 住在调度器线程上的 Task 对象，违反 Qt 线程安全规则
- 修复：改为 `deleteLater()` + `quit()` + `wait()`，让调度器线程自行销毁 Task 对象
- 注释说明：`deleteLater` 事件会在 `quit` 退出事件循环前被处理

#### 修复后的完整关机流程
```
aboutToQuit → Scheduler::stop()
  ├─ 同步停止所有任务（BlockingQueuedConnection）
  ├─ deleteLater() 所有 Task
  ├─ quit() 调度器线程
  └─ wait() 等待线程退出

a.exec() 返回 → UI 析构（信号已自动断开）

App::cleanup() → ModbusTcpMasterManager::shutdown()
  ├─ stopAllMasters()（BlockingQueuedConnection 同步停止）
  ├─ deleteLater() 所有 Master
  ├─ 并行 quit() 所有工作线程
  └─ 逐一 wait() 确保干净退出
```

#### 技术细节
- **事件循环顺序**：Qt 事件循环按 FIFO 处理，`deleteLater()` 投递的 DeferredDelete 事件在 `quit()` 之前处理
- **线程安全**：`deleteLater()` 自动投递到对象所属线程，无需手动 `invokeMethod`
- **并行 quit 的好处**：避免串行 quit 时某些线程仍在处理事件，导致竞态条件

#### 影响范围
- 修改文件：`data/modbustcpmastermanager/modbustcpmasterpool.cpp`（析构函数）
- 修改文件：`scheduler/scheduler.cpp`（stop 方法）

---

### 2026-04-23 19:30 - Simon
**报警直连运行日志与界面卡顿优化**

#### 修改内容
- **AlarmLoggerWidget → RunningLoggerWidget 直连**
  - `RunningLoggerWidget` 新增 `onAlarmPublished(const AlarmInfo&)` 与 `onAlarmResolved(const AlarmInfo&)` 槽
  - 在 `UIDemo6::initForm()` 中将 `ui->alarmpage->alarmLogger()` 的 `alarmPublished/alarmResolved` 信号直接连接到运行日志控件
  - `onAlarmResolved` 固定按 `Message` 级别写入运行日志

- **AlarmPage/AlarmLoggerWidget 文案与消息增强**
  - 断连告警文案改为动态设备号：`Device {masterId} connection lost`
  - `SoftwareConnectionLost` 解决时文案统一为：`Device {qrCode} connection resolved`
  - `AlarmInfo` 新增 `setMessage(const QString&)`，支持在转发前按场景改写 message
  - 修复默认构造 `AlarmLoggerWidget(QWidget*)` 下未应用上述解决文案的问题

- **性能优化（缓解界面卡顿）**
  - `RunningLoggerWidget` 跑马灯改为缓存拼接字符串（`m_scrollDoubled/m_scrollTotalLen`），避免每 150ms 重复字符串分配
  - `AlarmLoggerWidget::purgeResolvedRows()` 批量删行时禁用表格更新，减少重复布局重算
  - `RunningLoggerCollector::onFlushTick()` 增加空队列快速返回，减少无效锁与 swap 开销

#### 影响范围
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.h`
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.cpp`
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggercollector.cpp`
- 修改文件：`ui/customwidget/alarmloggerwidget/alarminfo.h`
- 修改文件：`ui/customwidget/alarmloggerwidget/alarmloggerwidget.cpp`
- 修改文件：`ui/alarmpage.h`
- 修改文件：`ui/alarmpage.cpp`
- 修改文件：`ui/uidemo6.cpp`

---

### 2026-04-23 18:30 - Simon
**新增 RunningLoggerCollector 运行日志采集器**

#### 修改内容
- **RunningLoggerCollector**：新增跨线程全局日志采集单例
  - 基于 Producer-Consumer 模式，任意线程可调用 `logMessage()` 提交日志
  - 内部使用 `QQueue<LogEntry>` + `QMutex` 缓冲，主线程定时器（50ms）异步提交
  - 每帧最多提交 20 条（`kMaxFlushPerTick`），防止队列积压时 UI 卡顿
  - 使用 `Q_GLOBAL_STATIC` 管理单例生命周期，`QPointer<RunningLoggerWidget>` 自动处理 Widget 销毁

- **RunningLoggerWidget**：优化日志控件初始化
  - 构造函数自动设置日志根目录为 `CustomLogger::RunningLoggerPath()`
  - `initialize()` 时自动注册到 `RunningLoggerCollector::setTarget(this)`
  - 滚动定时器移至构造函数启动，不依赖 `initialize()`
  - `refreshDisplayText()` 中立即更新按钮文字，无需等待定时器 tick

- **CustomLogger**：新增运行日志路径
  - 新增静态方法 `RunningLoggerPath()`，返回 `bin/log/runningLog`

- **UIDemo6**：初始化运行日志控件
  - `initForm()` 中调用 `ui->runningLoggerWidget->initialize()`

- **日志调用点**（均为 Message 级别）
  - `logindialog.cpp`：记录登录成功/失败
  - `changepassworddialog.cpp`：记录密码修改成功/失败
  - `useraccountlabel.cpp`：记录用户退出登录、"Login New Account" 时先退出再登录

#### 影响范围
- 新增文件：`ui/customwidget/runningloggerwidget/runningloggercollector.h`、`ui/customwidget/runningloggerwidget/runningloggercollector.cpp`
- 修改文件：`ui/customwidget/runningloggerwidget/runningloggerwidget.h`、`ui/customwidget/runningloggerwidget/runningloggerwidget.cpp`、`ui/customwidget/runningloggerwidget/runningloggerwidget.pri`、`app/customlogger.h`、`app/customlogger.cpp`、`ui/uidemo6.cpp`、`ui/logindialog.cpp`、`ui/changepassworddialog.cpp`、`ui/customwidget/useraccountlabel/useraccountlabel.cpp`

---

### 2026-04-23 17:00 - Simon
**UserAccountLabel 菜单功能增强**

#### 修改内容
- **UserAccountLabel**：菜单功能增强
  - 未登录状态：显示当前账号级别（Level）、登录账号（Login）
  - 已登录状态：显示当前用户名（Current User）、账号级别（Level）、修改密码（Change Password）、登录新账号（Login New Account）、退出登录（Logout）
  - 菜单项高度增加至 40px，便于触屏点击
  - 菜单位置调整至 label 正下方，中心对齐
  - 所有菜单字符串改为英文

- **ChangePasswordDialog**：新增修改密码对话框
  - 需要输入旧密码验证身份
  - 输入新密码并确认
  - 密码无长度限制，只需非空
  - 所有界面字符串改为英文

- **UserManager**：权限字符串转换方法
  - 新增静态方法 `permissionToString()` 将权限枚举转换为英文字符串
  - 从 UserAccountLabel 移至 UserManager，避免代码重复

#### 影响范围
- 新增文件：`ui/changepassworddialog.h`、`ui/changepassworddialog.cpp`
- 修改文件：`ui/customwidget/useraccountlabel/useraccountlabel.h`、`ui/customwidget/useraccountlabel/useraccountlabel.cpp`、`tool/usermanager/usermanager.h`、`tool/usermanager/usermanager.cpp`、`ui/logindialog.cpp`、`ui/ui.pri`

---

### 2026-04-23 16:20 - Simon
**UI 权限注册集成**

#### 修改内容
- **UIDemo6**：添加 `registerWidgetPermissions()` 方法
  - 通过 `UserManager::registerWidget()` 注册 UI 控件权限
  - `btnDebug` → `UserPermission::Root` (4)
  - `btnCommunicate` → `UserPermission::Debug` (2)
  - `btnSetting` → `UserPermission::Normal` (1)（普通用户可查看）
  - 在 `initForm()` 中调用权限注册方法

#### 影响范围
- 修改文件：`ui/uidemo6.h`、`ui/uidemo6.cpp`

---

### 2026-04-23 16:00 - Simon
**新增运行日志模块和用户管理模块**

#### 运行日志模块
- **RunningLoggerWidget**：实时运行日志显示组件
  - 支持三种消息类型：Message、Warn、Error
  - 待处理警报轮播显示
  - 基于 LoggerWidget 实现，使用 QJson 格式存储
  - 支持警报解决记录

#### 用户管理模块
- **UserManager**：用户权限管理系统
  - 基于单例模式，统一管理用户登录状态
  - 支持五级权限：Guest、Normal、Debug、Engineer、Root
  - 内置 root 账号（密码：cytc666666），硬编码验证不写入配置文件
  - 自动控制 UI 控件显示/隐藏（基于权限级别）
  - 支持用户增删改查（需要 Debug 及以上权限）

- **UserAccountLabel**：用户账号显示组件
  - 未登录：显示默认图标，左键点击弹出登录菜单
  - 已登录：显示蓝色圆形头像 + 用户名首字母
  - 支持登录新账号和登出功能
  - 自动响应登录/登出状态变化

- **LoginDialog**：登录对话框
  - 用户名和密码输入
  - 通过 UserManager 进行登录验证
  - 错误提示显示

---

### 2026-04-22 19:16 - Simon
**通讯日志历史查询性能优化（翻页 1021ms → 4ms）**

#### 问题背景
用户反馈通讯日志（`CommunicateLoggerWidget`）历史 Tab 点击分页按钮非常慢（约 1 秒），即使数据已缓存。

#### 根因分析
通过插入 `QElapsedTimer` 计时日志定位瓶颈：
- 每次翻页 **cache MISS**，worker 线程每次重读 74 万行 CSV（约 1015ms）
- 原缓存键包含 `fileSig`（文件路径 + 大小），当天 CSV 文件在持续 `append` 实时日志，大小不断变化，导致缓存**永远失效**

#### 修改内容
1. **查询缓存从"单项"升级为 LRU（容量 5）**
   - 原 `m_qc*` 系列字段（单项缓存）替换为 `CommLRUCache<CommQueryKey, CommQueryValue> m_queryCache{5}`
   - 切换查询条件（日期/时间段/qrcode/commandId）也能命中历史缓存

2. **缓存键移除文件大小依赖**
   - `CommQueryKey` 删除 `files` 字段（原为 `QVector<QPair<QString,qint64>>`）
   - 今天 CSV 持续 append 不再让翻页缓存失效

3. **显式刷新机制**
   - `CommHistoryQuery` 新增 `bool forceRefresh = false`
   - Search 按钮 → `forceRefresh=true`，worker 主动 `m_queryCache.remove(key)` 重算最新数据
   - 翻页按钮 → `forceRefresh=false`，O(1) 命中缓存做切片

4. **`matchedGlobalIndices` O(N) → O(1)**
   - 原每次翻页都 `resize(viewSize)` 并填充 `[0..viewSize-1]`（8179 条时跨线程传输量大）
   - 改为只在有命中时填 1 个哨兵元素（widget 只用 `isEmpty()` 判断）

5. **Miss 时才读盘**
   - 缓存命中时完全跳过 `filePathsForDate` 和 `QtConcurrent::blockingMapped`

#### 性能对比
| 指标 | 优化前 | 优化后 |
|---|---|---|
| 翻页总耗时 | 1021 ms | **4 ms** |
| 缓存命中 | MISS（每次） | HIT |


---

### 2026-04-22 18:00 - Simon
**AlarmLoggerWidget 警报日志模块完善**

#### 修改内容
1. **UI 字符串本地化**
   - `alarmpage.cpp`、`alarmloggerwidget.ui`、`alarmloggerwidget.cpp` 中所有中文 UI 字符串统一为英文
   - 移除不需要的按钮（findPrevBtn、findNextBtn）及相关逻辑
   - `labelLike`、`likeEdit` 替换为 `QRCode SpinBox` 和 `CommandId ComboBox`
   - 查找成功/失败均通过 `QMessageBox` 提示

2. **已解决告警行清理机制**
   - `AlarmLoggerWidget` 新增 `QList<int> m_resolvedRows` 记录已解决行索引
   - 新增 `QTimer m_purgeTimer`，每 3 秒触发 `purgeResolvedRows()` 按 FIFO 清理
   - 新增 `setResolvedPurgeBatchSize(int)` 接口，默认批次 40 条
   - 清理后自动重建内部行索引映射
   - 日期翻页（`onDayRollover`）时清空 `m_resolvedRows`

3. **表格列宽比例配置**
   - 实时表格与历史表格统一使用 `5:8:5:5:5:8:5` 列宽比例
   - `Message` 列左对齐，其余列居中（`Request`/`Response` 除外）
   - 分页按钮增大尺寸（宽度、字体）方便点击

4. **警报日志根目录配置**
   - `AlarmPage` 初始化时通过 `setRootDir(CustomLogger::AlarmLoggerPath())` 设置根目录为 `bin/log/alarms/`
   - 警报日志按 `年月/日_HHmmss.csv` 结构存储，保留 6 个月

5. **网络状态联动**
   - `AlarmPage::onNetworkStatusChanged` 在断连时自动上报 `SoftwareConnectionLost` 告警
   - 重连成功时自动提交告警解决

6. **注释统一**
   - `alarmloggerwidget.h` / `.cpp` 中的英文注释统一改回中文，保持项目注释风格一致

#### 影响范围
- 修改文件：`ui/alarmpage.cpp`
- 修改文件：`ui/customwidget/alarmloggerwidget/alarmloggerwidget.ui`
- 修改文件：`ui/customwidget/alarmloggerwidget/alarmloggerwidget.h`
- 修改文件：`ui/customwidget/alarmloggerwidget/alarmloggerwidget.cpp`

---

### 2026-04-22 11:43 - Simon
**CommunicatePage 模块完成**

#### 修改内容
1. **UI字符串本地化**
   - 将 `communicateloggerwidget.ui` 中所有UI标签、按钮文本翻译为英文
   - 将 `communicateloggerwidget.cpp` 中列头（kLiveHeaders）和用户可见消息翻译为英文
   - 更新 `communicatepage.cpp` 中 JSON 键以匹配新的英文列头
   - 翻译 `commloadmoreitemwidget.cpp` 中加载更多小部件的文本
   - 翻译 `commlogfilesystem.cpp` 中错误消息
   - 修复 `commlogpagemodel.cpp` 中用户可见格式中的冒号

2. **时间格式优化**
   - 将日志时间格式从 `yyyy-MM-dd HH:mm:ss.zzz` 改为 `yyyy-MM-dd HH:mm:ss`
   - 不再显示毫秒部分

3. **表格列宽比例设置**
   - 实时表格和历史表格列宽比例设置为 1:2:2.5:1:3.5:6
   - qrcode: 80px, Time: 160px, CommandId: 200px, DurationMs: 80px, Request: 280px, Response: 480px

#### 影响范围
- 修改文件：`ui/customwidget/communicateloggerwidget/communicateloggerwidget.ui`
- 修改文件：`ui/customwidget/communicateloggerwidget/communicateloggerwidget.cpp`
- 修改文件：`ui/communicatepage.cpp`
- 修改文件：`ui/customwidget/communicateloggerwidget/commloadmoreitemwidget.cpp`
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogfilesystem.cpp`
- 修改文件：`ui/customwidget/communicateloggerwidget/commlogpagemodel.cpp`

---

### 2026-04-20 14:28 - Simon
**MonitorDataTask 数据解析修正**

#### 修改内容
- 修正 `parseReadFoupStatusResponse` 中时间字段的单位转换
- 充气时间和 idle 时间从秒转换为毫秒（乘以 1000）
- 更新 `getU16BE` lambda 注释，明确单位为秒

#### 技术细节
- Modbus 寄存器返回的时间单位为秒，需转换为毫秒存储
- 字节偏移 10-11：充气时间（秒）→ 毫秒
- 字节偏移 12-13：idle 时间（秒）→ 毫秒

#### 影响范围
- 修改文件：`scheduler/tasks/monitor_data_task.cpp`

---

### 2026-04-20 14:06 - Simon
**图配置解析器增强与天车轨道布局优化**

#### 修改内容
1. **GraphConfigParser 自动设置 SET 节点 uiId**
   - 在 `GraphConfigParser` 中添加 `m_setNodeIndex` 计数器成员变量
   - 在构造函数中初始化计数器为 0
   - 在 `processNode()` 方法中，解析到 SET 节点时按文档顺序自动设置 `SharedData::setOfOHBInfoList` 中对应项的 `uiId`
   - 添加详细日志记录：`"SET节点文档顺序#{}: 设置 setOfOHBInfoList[{}].uiId = {}"`
   - 移除 `SharedData` 构造函数中硬编码的 `uiIds` 数组，改为由 XML 解析器动态设置

2. **graph_config.xml 布局调整**
   - 调整边的偏移值以优化天车轨道布局显示效果

#### 技术细节
- **SET 节点顺序映射**：按 XML 文档中 SET 节点的出现顺序（从上到下），依次映射到 `setOfOHBInfoList[0]`、`[1]`、`[2]`...
- **解析时机**：`GraphConfigParser::parse()` 在 `QtConcurrent::run()` 中异步执行，确保在 `SharedData` 初始化之后
- **线程安全**：`SharedData::setOfOHBInfoList` 在主线程初始化，解析器在工作线程中修改 `uiId`，无并发冲突
- **日志路径**：使用 `AppLogger::CraneMapLoggerPath()` 记录到 `ui/cranemap` 日志文件

#### 数据流向
```
GraphConfigParser::parse()
  ↓ parseEdges()
  ↓ processNode(SET节点)
  ↓ 创建 SetOfOHBNode
  ↓ SharedData::setOfOHBInfoList[m_setNodeIndex].setUiId(nodeId)
  ↓ m_setNodeIndex++
```

#### 影响范围
- 修改文件：`ui/customwidget/overheadcranetrack/graphconfigparser.h`（添加 `m_setNodeIndex` 成员）
- 修改文件：`ui/customwidget/overheadcranetrack/graphconfigparser.cpp`（引入 `shareddata.h`，构造函数初始化计数器，`processNode` 中设置 uiId）
- 修改文件：`bin/config/graph_config.xml`（调整边的偏移值以优化布局显示）

---

### 2026-04-20 11:05 - Simon
**日志系统优化与固件升级功能增强**

#### 修改内容
1. **日志级别统一**
   - 移除 "trace" 日志级别，统一使用 info/warn/error 三种级别
   - 数据帧日志从 trace 改为 info 级别
   - 移除日志样式中的 trace 分支，仅保留 error（红色）和 warn（橙色）文字颜色

2. **日志批量写入优化**
   - 在 LogicalFileSystem 中添加日志缓冲机制
   - 使用 100ms 定时器批量写入日志，减少信号发射频率
   - LogFileSystem 新增 requestAppendBatch 槽，支持批量写入
   - 解决高频日志写入导致的 UI 刷新闪烁问题

3. **固件升级截图功能**
   - 实现 captureTableWidgetScreenshot() 方法，捕获升级结果表格
   - 截图保存路径：`bin/log/firmware_upgrade/capture/`
   - 文件命名格式：`firmware_update_yyyyMMdd_hhmmss_zzz.png`
   - 在所有设备升级完成时自动触发截图，确保最终状态完整

4. **固件升级界面重构**
   - 创建 FirmwareUpdateSettingWidget 容器，用 SettingWidget 封装升级界面
   - 配置界面（FirmwareUpdateConfigSettingWidget）和升级界面分离
   - 通过 Qt 信号槽（binFilePathChanged）解耦通信
   - 标题改为英文：Firmware Config、Firmware Update

5. **目录结构调整**
   - 重命名 `customwidget/debugpage/` → `customwidget/debugsettingwidget/`
   - 更新 customwidget.pri 中的 include 路径
   - 更新 debugpage.cpp 中的头文件引用路径
   - 添加 .gitignore，忽略构建产物和日志文件

#### 技术细节
- **日志缓冲**：QVector<QJsonObject> 缓冲 + QTimer::singleShot(100) 触发批量写入
- **截图时机**：onTaskAllProgress 中 completed >= total 时触发，先 repaint() 再截图
- **信号解耦**：配置界面发射 binFilePathChanged 信号，DebugPage 连接到升级界面

#### 影响范围
- 新增文件：`ui/customwidget/debugsettingwidget/firmwareupdatesettingwidget.h`、`.cpp`
- 新增文件：`.gitignore`
- 修改文件：`ui/customwidget/debugsettingwidget/firmwareupdateconfigsettingwidget.h`、`.cpp`
- 修改文件：`ui/customwidget/debugsettingwidget/firmwareupdatewidget.cpp`（日志级别、截图）
- 修改文件：`ui/customwidget/loggerwidget/logfilesystem.h`、`.cpp`（批量写入）
- 修改文件：`ui/customwidget/loggerwidget/logicalfilesystem.h`、`.cpp`（批量缓冲）
- 修改文件：`ui/customwidget/loggerwidget/logitemdelegate.cpp`（样式调整）
- 修改文件：`ui/debugpage.h`、`ui/debugpage.cpp`（集成两个 SettingWidget）
- 修改文件：`ui/customwidget/customwidget.pri`（路径更新）
- 修改文件：`ui/customwidget/debugsettingwidget/debugsettingwidget.pri`（文件重命名）

---

### 2026-04-18 11:30 - Simon
**固件升级任务迁移**

#### 修改内容
1. **FirmwareUpgradeTask 固件升级任务**
   - 从旧代码 `old/firmware_upgrade_task.{h,cpp}` 迁移到新架构
   - 创建 `scheduler/tasks/firmware_upgrade_task.h` 和 `.cpp`
   - 适配新版 `FirmwareUpgrader` 类（位于 data 层）
   - 支持多设备并发固件升级
   - 使用共享 BinFileReader 实例，避免重复读取文件
   - 遵循项目日志格式规范：`[Scheduler][FirmwareUpgradeTask][方法名]`

2. **主要变更**
   - 设备标识从 `qrcode` 改为 `deviceId`（与 ModbusTcpMaster 的 ID 字段一致）
   - FirmwareUpgrader 信号增加 `masterId` 参数，便于多设备场景识别
   - ModbusTcpMasterManager 访问方式从指针改为引用
   - 添加详细的迁移说明文档：`scheduler/tasks/FIRMWARE_UPGRADE_MIGRATION.md`

3. **构建配置**
   - 将新任务添加到 `scheduler/scheduler.pri`
   - 保留旧代码在 `old/` 目录作为参考

---

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
