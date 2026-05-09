# 任务日志对应关系文档

本文档记录各个 Scheduler Task 与运行日志（OperationDispatchTask）、警报日志（AlarmDispatchTask）的对应关系。

---

## ReadVEFCFlowUnitAndMediumStatusTask

### 功能
读取 VEFC 流量单元和介质状态。

### 运行日志（OperationDispatchTask）

#### 启动日志
- **位置**: `start()` 方法
- **条件**: 任务正常启动，设备列表不为空且指令池包含所需指令
- **级别**: `Message`
- **内容**: `ReadVEFCStatus task started: X devices`
- **参数**: X - 设备数量

#### 早期失败日志
- **位置**: `start()` 方法
- **条件**: 设备列表为空
- **级别**: `Warn`
- **内容**: `ReadVEFCStatus task failed: device identification failed`

- **位置**: `start()` 方法
- **条件**: 指令池中找不到所需指令
- **级别**: `Warn`
- **内容**: `ReadVEFCStatus task failed: feature not implemented`

#### 超时日志
- **位置**: `onTimeout()` 方法
- **条件**: 任务超时，有设备未响应
- **级别**: `Error`
- **内容**: `ReadVEFCStatus task timeout: X devices did not respond (device1, device2, ...)`
- **参数**: X - 未响应设备数量，device1, device2, ... - 具体设备列表

#### 完成日志
- **位置**: `forceFinish()` 方法
- **条件**: 任务完成，所有设备成功
- **级别**: `Message`
- **内容**: `ReadVEFCStatus task completed: X/Y devices succeeded`
- **参数**: X - 成功设备数量，Y - 总设备数量

- **位置**: `forceFinish()` 方法
- **条件**: 任务完成，有设备失败
- **级别**: `Error`
- **内容**: `ReadVEFCStatus task failed: X/Y devices succeeded. Failed devices: device1 (reason); device2 (reason); ...`
- **参数**: 
  - X - 成功设备数量
  - Y - 总设备数量
  - device1, device2, ... - 失败设备列表
  - reason - 失败原因：
    - `communication failed` - 通信失败（unitRaw=0, mediumRaw=0）
    - `unit abnormal` - 单元状态异常
    - `medium abnormal` - 介质状态异常
    - `unit abnormal, medium abnormal` - 单元和介质均异常

### 警报日志（AlarmDispatchTask）
无

---
