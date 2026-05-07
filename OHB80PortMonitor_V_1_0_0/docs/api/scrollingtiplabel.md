# ScrollingTipLabel API 文档

## 模块概述

ScrollingTipLabel 是一个滚动公告栏控件，用于实时显示系统操作日志和警报日志。该控件继承自 QLabel，具有警报优先显示、消息滚动、消息消除等功能，并支持根据消息类型自动切换样式。

**特性：**
- 警报优先显示，警报模式下使用红色背景样式
- 普通日志使用浅灰色背景样式
- 文本长度超过控件宽度时自动滚动显示
- 无消息时自动隐藏控件
- 必须在 UI 线程中使用

---

## 类定义

```cpp
class ScrollingTipLabel : public QLabel
{
    Q_OBJECT
};
```

**继承关系：**
- `QLabel` → `ScrollingTipLabel`

---

## 公共 API

### submitAlarmLog

提交一个警报日志。

```cpp
void submitAlarmLog(const QStringList& operationLog, int alarmRecordId);
```

**参数：**
- `operationLog` (const QStringList&): OperationLogDBCon 记录，格式为 QStringList
- `alarmRecordId` (int): AlarmLogDBCon 的警报记录的主键 ID

**说明：**
- 将警报日志存储到内部映射表 `m_alarmLogs`
- 将警报记录 ID 加入队列 `m_alarmQueue`
- 触发显示更新

---

### submitAlarmResolved

提交一个警报已解决日志。

```cpp
void submitAlarmResolved(int alarmRecordId);
```

**参数：**
- `alarmRecordId` (int): AlarmLogDBCon 的警报记录的主键 ID

**说明：**
- 从警报队列 `m_alarmQueue` 中移除对应的警报记录 ID
- 从映射表 `m_alarmLogs` 中删除对应的警报记录
- 触发显示更新

---

### submitOperationLog

提交一个 OperationLogDBCon 记录。

```cpp
void submitOperationLog(const QStringList& operationLog);
```

**参数：**
- `operationLog` (const QStringList&): OperationLogDBCon 记录，格式为 QStringList

**说明：**
- 替换当前存储的 OperationLog 记录 `m_operationLog`
- 触发显示更新

---

## 使用示例

```cpp
#include "scrollingtiplabel.h"

// 创建控件
auto *tipLabel = new ScrollingTipLabel(this);
tipLabel->setFixedHeight(30);
tipLabel->setFixedWidth(400);

// 提交操作日志
tipLabel->submitOperationLog({"2026-05-07 16:00:00", "INFO", "系统启动成功"});

// 提交警报日志
tipLabel->submitAlarmLog({"2026-05-07 16:05:00", "ALARM", "设备连接断开"}, 1001);

// 提交警报已解决
tipLabel->submitAlarmResolved(1001);

// 再次提交操作日志（替换之前的操作日志）
tipLabel->submitOperationLog({"2026-05-07 16:10:00", "INFO", "设备重新连接"});
```

---

## 注意事项

1. **日志格式**：假设 QStringList 格式为 `[时间, 日志类型, 描述]`，如与实际格式不符需调整 `formatLogToString()` 方法
2. **UI 线程使用**：该控件为 UI 控件，必须在 UI 线程中使用。调度层通过信号传递日志，调度层不需要知道该模块的存在
3. **内存管理**：警报队列和映射表会持续增长，建议定期清理已解决的警报
4. **滚动效果**：当前滚动实现较为简单，如需更平滑的效果可使用 QGraphicsView 或自定义绘制
5. **显示优先级**：警报日志优先级最高，只要警报队列不空，公告栏优先显示警报
6. **自动隐藏**：当没有任何可显示的消息时，控件会自动隐藏；有新消息时自动显示
7. **样式切换**：控件会根据当前显示的消息类型自动切换样式（警报模式/普通模式）
