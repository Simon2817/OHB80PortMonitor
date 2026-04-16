# OHB80PortMonitor 项目框架文档

## 项目概述
OHB80PortMonitor 是一个基于 Qt 的 80 端口 OHB 充氮设备监控上位机软件。

## 项目目录结构

### 根目录结构
```
OHB80PortMonitor_V_1_0_0/
├── app/                    # 应用程序核心模块
├── bin/                    # 可执行文件和运行时文件
├── build/                  # 构建输出目录
├── classes/                # 业务类模块
├── data/                   # 数据文件目录
├── resource/               # 资源文件（图片、图标等）
├── scheduler/              # 调度器模块
├── tool/                   # 工具类模块
├── ui/                     # UI 界面模块
├── main.cpp                # 程序入口
├── head.h                  # 公共头文件
├── app.rc                  # Windows 资源文件
└── OHB80PortMonitor_V_1_0_0.pro  # Qt 项目文件
```

### 各目录作用说明

#### 1. **app/** - 应用程序核心模块
应用程序的核心管理模块，负责应用程序的初始化、配置管理、日志管理等基础功能。

#### 2. **bin/** - 可执行文件和运行时目录
存放编译后的可执行文件及运行时所需的配置、日志等文件。

#### 3. **build/** - 构建输出目录
Qt 项目的编译输出目录，包含中间文件和最终的可执行文件。

#### 4. **classes/** - 业务类模块
存放业务逻辑相关的类文件。

#### 5. **data/** - 数据文件目录
存放应用程序运行时产生的数据文件。

#### 6. **resource/** - 资源文件目录
存放图片、图标、样式表等资源文件。

#### 7. **scheduler/** - 调度器模块
任务调度相关的功能模块。

#### 8. **tool/** - 工具类模块
通用工具类，如日志管理器（LoggerManager）、辅助函数等。

#### 9. **ui/** - UI 界面模块
用户界面相关的类和窗体文件。

---

## bin 目录结构详解

```
bin/
├── config/                 # 配置文件目录
│   └── app.ini            # 应用程序配置文件
├── data/                   # 数据文件目录（预留）
├── log/                    # 日志文件目录
│   ├── debug/             # 调试日志（系统日志）
│   └── user/              # 用户日志（用户操作日志）
├── x32/                    # 32位可执行文件目录
│   ├── *.exe              # 32位可执行文件
│   ├── *.dll              # 32位依赖库
│   └── log/               # 32位程序的调试日志
└── x64/                    # 64位可执行文件目录
    ├── *.exe              # 64位可执行文件
    ├── *.dll              # 64位依赖库
    └── log/               # 64位程序的调试日志
```

### bin 目录说明

#### **config/** - 配置文件目录
存放应用程序的配置文件 `app.ini`，包含：
- 应用名称（AppName）
- 应用版本（AppVersion）
- 操作系统类型（OSType: x32/x64）

#### **log/** - 日志文件目录
- **debug/** - 调试日志目录，存放系统级别的调试日志（通过 LoggerManager 管理）
- **user/** - 用户日志目录，存放用户操作相关的日志

#### **x32/** 和 **x64/** - 可执行文件目录
分别存放 32 位和 64 位版本的可执行文件及其依赖库。每个目录下都有独立的 `log/` 子目录用于存放该版本程序的调试日志。

---

## app 模块详解

### 模块概述
`app/` 模块是应用程序的核心管理模块，负责应用程序的生命周期管理、配置管理、日志管理等基础功能。

### 文件结构
```
app/
├── app.h                   # App 类头文件
├── app.cpp                 # App 类实现文件
├── appconfig.h             # AppConfig 类头文件
├── appconfig.cpp           # AppConfig 类实现文件
├── applogger.h             # AppLogger 类头文件
├── applogger.cpp           # AppLogger 类实现文件
└── app.pri                 # Qt 子项目文件
```

### 核心类说明

#### 1. **App 类** - 应用程序管理类

**职责：**
- 应用程序的初始化和清理
- 管理应用程序的全局状态
- 提供统一的配置和日志访问接口

**主要方法：**

```cpp
class App
{
public:
    // 静态初始化和清理方法
    static bool initialize();           // 初始化应用程序
    static void cleanup();              // 清理应用程序资源

    // 获取配置和日志管理器
    static AppConfig* getConfig();      // 获取配置管理器实例
    static AppLogger* getLogger();      // 获取日志管理器实例
    
    // 应用信息
    static QString getAppName();        // 获取应用名称
    static QString getAppVersion();     // 获取应用版本
    static QString getDisplayName();    // 获取显示名称（名称 V版本号）
    
    // 路径信息（直接访问静态变量）
    static QString executableDir;       // 可执行文件目录
    static QString configDir;           // 配置文件目录
    static QString debugLogDir;         // 调试日志目录
    static QString userLogDir;          // 用户日志目录

private:
    static bool initializeLogging();    // 初始化日志系统
    static bool initializeDirectories(); // 初始化目录结构
    static void onAboutToQuit();        // 应用程序退出时的清理
};
```

**初始化流程：**

1. **路径初始化**
   - 获取可执行文件目录（`bin/x32` 或 `bin/x64`）
   - 计算根目录（bin 目录）
   - 设置配置目录、日志目录等

2. **单实例检查**
   - 确保应用程序只运行一个实例

3. **目录创建**
   - 创建必要的目录（config、log/debug、log/user）

4. **日志系统初始化**
   - 初始化 AppLogger
   - 注册崩溃信号处理器

5. **连接退出信号**
   - 连接 QApplication 的 aboutToQuit 信号，确保资源正确释放

---

#### 2. **AppConfig 类** - 配置管理类

**职责：**
- 读取和管理应用程序配置
- 提供路径信息访问接口
- 管理配置文件的读写

**主要方法：**

```cpp
class AppConfig
{
public:
    static AppConfig& getInstance();    // 获取单例实例
    
    // 应用信息
    QString getAppName() const;         // 获取应用名称
    QString getAppVersion() const;      // 获取应用版本
    QString getOSType() const;          // 获取操作系统类型（x32/x64）
    
    // 路径信息
    QString getRootDir() const;         // 获取软件根目录（bin目录）
    QString getExecutableDir() const;   // 获取可执行文件目录
    QString getConfigDir() const;       // 获取配置文件目录
    QString getDebugLogDir() const;     // 获取调试日志目录
    QString getUserLogDir() const;      // 获取用户日志目录
    
    // 配置文件操作
    void reload();                      // 重新加载配置
    bool setValue(const QString& key, const QVariant& value);  // 设置配置值
    QVariant getValue(const QString& key, const QVariant& defaultValue) const;  // 获取配置值

private:
    AppConfig();                        // 私有构造函数（单例模式）
    void initializePaths();             // 初始化路径
    
    QSettings* m_settings;              // Qt 配置文件管理器
    QString m_rootDir;                  // 根目录
    QString m_executableDir;            // 可执行文件目录
    QString m_configDir;                // 配置目录
    QString m_debugLogDir;              // 调试日志目录
    QString m_userLogDir;               // 用户日志目录
};
```

**配置文件格式（app.ini）：**

```ini
[Application]
AppName = OHB80PortMonitor
AppVersion = 1.0.0
OSType = x64
```

**路径规则：**
- `m_executableDir`: 可执行文件所在目录（`bin/x32` 或 `bin/x64`）
- `m_rootDir`: 可执行文件目录的上一级（`bin` 目录）
- `m_configDir`: `{rootDir}/config`
- `m_debugLogDir`: `{executableDir}/log`（与可执行文件同目录）
- `m_userLogDir`: `{rootDir}/log/user`（在根目录下）

---

#### 3. **AppLogger 类** - 日志管理类

**职责：**
- 初始化日志系统（基于 LoggerManager）
- 管理日志文件的生成和回滚
- 处理程序崩溃时的日志记录

**主要方法：**

```cpp
class AppLogger
{
public:
    AppLogger();
    ~AppLogger();
    
    // 初始化日志系统
    bool initialize(const QString& logDir);
    
    // 关闭日志系统
    void shutdown();
    
    // 注册崩溃信号处理器
    void registerCrashHandler();
    
    // 崩溃信号处理器（静态方法，用于信号处理）
    static void crashHandler(int sig);
    
    // 获取日志目录
    QString getLogDir() const;

private:
    bool m_initialized;                 // 是否已初始化
    QString m_logDir;                   // 日志目录
};
```

**日志系统特性：**
- 基于 spdlog 的异步日志系统
- 支持日志文件自动回滚（单文件 10MB，保留 5 个文件）
- 支持日志保留天数设置（默认 7 天）
- 自动按日期分目录存储
- 支持 trace、debug、info、warn、error、critical 多个级别
- 自动分离 warn 和 error 日志
- 崩溃时自动记录崩溃信息

---

## 在 main.cpp 中的使用

### 完整使用流程

```cpp
#include "app.h"
#include "quiwidget.h"
#include "uidemo6.h"

int main(int argc, char *argv[])
{
    // 1. 启用高 DPI 支持
    #if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    // 2. 创建 QApplication 实例
    QApplication a(argc, argv);

    // 3. 初始化 App 类（关键步骤）
    if (!App::initialize()) {
        qCritical() << "Failed to initialize application";
        return -1;
    }
    
    // 4. 设置应用程序字体
    a.setFont(QFont("Microsoft Yahei", 9));

    // 5. 初始化 UI 组件
    QUIWidget::setCode();
    QUIWidget::setTranslator(":/image/qt_zh_CN.qm");
    QUIWidget::setTranslator(":/image/widgets.qm");

    // 6. 创建主窗口
    QUIWidget qui;
    qui.setStyle(QUIWidget::Style_LightBlue);
    UIDemo6 *creator = new UIDemo6;
    creator->setDoubleClickMaximize(true);

    // 7. 设置主窗体
    qui.setMainWidget(creator);
    qui.setTitleBarVisible(false);
    qui.setMovable(true);

    // 8. 连接信号和槽
    QObject::connect(creator, SIGNAL(requestMinimize()), 
                     &qui, SLOT(on_btnMenu_Min_clicked()));
    QObject::connect(creator, SIGNAL(requestMaximizeOrRestore()), 
                     &qui, SLOT(on_btnMenu_Max_clicked()));
    QObject::connect(creator, SIGNAL(requestClose()), 
                     &qui, SLOT(on_btnMenu_Close_clicked()));

    // 9. 设置窗口属性
    qui.setTitle("QUI皮肤生成器 (作者: 飞扬青云  QQ: 517216493)");
    qui.setAlignment(Qt::AlignCenter);
    qui.setSizeGripEnabled(true);
    qui.setVisible(QUIWidget::BtnMenu, true);

    // 10. 显示主窗口
    qui.show();

    // 11. 进入事件循环
    return a.exec();
    
    // 12. 清理资源（注意：此行代码不会执行，因为 a.exec() 会阻塞）
    App::cleanup();
}
```

### 使用说明

#### **App::initialize() 的作用**
在 `main()` 函数中调用 `App::initialize()` 是整个应用程序启动的关键步骤，它会：

1. **初始化路径系统**
   - 自动检测可执行文件位置
   - 设置配置目录、日志目录等

2. **单实例检查**
   - 防止程序重复运行

3. **创建必要的目录**
   - 自动创建 config、log 等目录

4. **初始化日志系统**
   - 启动日志管理器
   - 注册崩溃处理器

5. **连接退出信号**
   - 确保程序退出时正确清理资源

#### **访问配置和日志**

在应用程序的任何地方，都可以通过以下方式访问配置和日志：

```cpp
// 获取配置信息
QString appName = App::getAppName();
QString appVersion = App::getAppVersion();
QString configDir = App::configDir;

// 获取配置管理器
AppConfig* config = App::getConfig();
QString osType = config->getOSType();

// 获取日志管理器
AppLogger* logger = App::getLogger();

// 使用 LoggerManager 记录日志
LoggerManager::instance().log("system", Level::INFO, "应用程序启动");
LoggerManager::instance().log("user", Level::INFO, "用户执行了某操作");
```

#### **资源清理**

应用程序退出时，`App::cleanup()` 会自动被调用（通过 `aboutToQuit` 信号），执行以下清理操作：

1. 关闭日志系统
2. 释放单实例共享内存
3. 保存配置文件
4. 释放其他资源

---

## 设计模式

### 单例模式
- **AppConfig**: 使用单例模式，确保全局只有一个配置管理器实例
- **LoggerManager**: 使用单例模式，确保全局只有一个日志管理器实例

### 静态工具类
- **App**: 使用静态方法和静态变量，提供全局访问点

### 资源管理
- 使用 RAII（Resource Acquisition Is Initialization）原则
- 在构造函数中初始化资源，在析构函数中释放资源
- 使用智能指针管理动态内存

---

## 依赖关系

```
main.cpp
    └── App::initialize()
            ├── AppConfig::getInstance()
            │       ├── initializePaths()
            │       └── QSettings (读取 app.ini)
            │
            └── AppLogger
                    └── LoggerManager (spdlog)
```

---

## 总结

`app/` 模块是整个应用程序的基础框架，提供了：
- ✅ 统一的初始化和清理机制
- ✅ 配置文件管理
- ✅ 日志系统管理
- ✅ 路径管理
- ✅ 单实例控制
- ✅ 崩溃处理

通过在 `main()` 函数开始时调用 `App::initialize()`，可以确保应用程序在一个稳定、可控的环境中运行。
