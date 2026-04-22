# 调度器使用示例

## 概述

调度器及常驻任务已集中到 `SharedData` 中管理，提供统一的初始化和访问接口。

## 初始化

调度器在 `App::initScheduler()` 中自动初始化：

```cpp
// app.cpp
void App::initScheduler()
{
    // 调用 SharedData 的调度器初始化方法
    SharedData::initScheduler();
}
```

`SharedData::initScheduler()` 会：
1. 启动调度器
2. 创建并提交 `NetworkStatusTask`（网络状态监控任务）
3. 创建并提交 `MonitorDataTask`（监控数据任务）

## 获取常驻任务实例

### 方法 1：获取网络状态监控任务

```cpp
#include "app/shareddata.h"

// 获取网络状态监控任务
NetworkStatusTask* networkTask = SharedData::getNetworkStatusTask();

if (networkTask) {
    // 可以连接任务的信号
    connect(networkTask, &NetworkStatusTask::allInitFinished,
            this, &MyWidget::onAllDevicesInitialized);
}
```

### 方法 2：获取监控数据任务

```cpp
#include "app/shareddata.h"

// 获取监控数据任务
MonitorDataTask* monitorTask = SharedData::getMonitorDataTask();

if (monitorTask) {
    // 可以访问任务的公共方法或连接信号
    // 例如：获取任务状态
    auto state = monitorTask->state();
}
```

## 使用场景示例

### 场景 1：在 UI 页面中监听设备初始化完成

```cpp
// homepage.cpp
void HomePage::initScheduler()
{
    // 获取网络状态任务
    NetworkStatusTask* networkTask = SharedData::getNetworkStatusTask();
    
    if (networkTask) {
        // 连接初始化完成信号
        connect(networkTask, &NetworkStatusTask::allInitFinished,
                this, [this](bool allSuccess, int successCount, int failCount, const QStringList& failedIds) {
            qDebug() << "[HomePage] 设备初始化完成:"
                     << "成功" << successCount
                     << "失败" << failCount;
            
            if (!allSuccess) {
                // 显示失败设备列表
                showFailedDevices(failedIds);
            }
        });
    }
}
```

### 场景 2：在调试页面中显示任务状态

```cpp
// debugpage.cpp
void DebugPage::updateTaskStatus()
{
    NetworkStatusTask* networkTask = SharedData::getNetworkStatusTask();
    MonitorDataTask* monitorTask = SharedData::getMonitorDataTask();
    
    if (networkTask) {
        ui->labelNetworkTaskStatus->setText(
            QString("网络监控任务: %1").arg(taskStateToString(networkTask->state()))
        );
    }
    
    if (monitorTask) {
        ui->labelMonitorTaskStatus->setText(
            QString("数据监控任务: %1").arg(taskStateToString(monitorTask->state()))
        );
    }
}
```

### 场景 3：在通信页面中访问网络状态

```cpp
// communicatepage.cpp
void CommunicatePage::checkDeviceStatus()
{
    NetworkStatusTask* networkTask = SharedData::getNetworkStatusTask();
    
    if (networkTask) {
        // 可以通过任务访问设备连接状态
        // 注意：需要在 NetworkStatusTask 中添加相应的公共接口
        // 例如：networkTask->getDeviceConnectionStatus(deviceId);
    }
}
```

## 注意事项

1. **生命周期管理**：
   - 任务实例由 `SharedData` 管理，不要手动 delete
   - 任务会随着调度器的停止而停止

2. **线程安全**：
   - 任务运行在调度器的工作线程中
   - 如果需要访问任务数据，注意线程安全
   - 使用信号槽机制进行跨线程通信

3. **空指针检查**：
   - 在调度器初始化之前，`getNetworkStatusTask()` 和 `getMonitorDataTask()` 返回 `nullptr`
   - 使用前务必检查返回值

4. **初始化顺序**：
   - `App::initialize()` → `App::getSharedData()` → `App::initScheduler()`
   - 确保在访问任务之前调度器已初始化

## API 参考

### SharedData 静态方法

```cpp
class SharedData
{
public:
    // 初始化调度器并启动常驻任务
    static void initScheduler();
    
    // 获取网络状态监控任务
    static NetworkStatusTask* getNetworkStatusTask();
    
    // 获取监控数据任务
    static MonitorDataTask* getMonitorDataTask();
};
```

### NetworkStatusTask 可用信号

```cpp
signals:
    // 所有设备初始化完成
    void allInitFinished(bool allSuccess, int successCount, int failCount, 
                        const QStringList& failedMasterIds);
```

### MonitorDataTask 可用方法

```cpp
// 继承自 SchedulerTask
SchedulerTask::State state() const;  // 获取任务状态
```
