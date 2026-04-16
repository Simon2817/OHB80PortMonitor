# OHB80PortMonitor
80port ohb 充氮设备监控上位机

## 项目文档
详细的项目框架文档请参阅：[PROJECT_STRUCTURE.md](./OHB80PortMonitor_V_1_0_0/docs/PROJECT_STRUCTURE.md)

---

## 更新日志

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
