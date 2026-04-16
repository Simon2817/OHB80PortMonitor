#include "qthelper.h"

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QMessageBox>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QString>
#include <QHash>
#include <QWidget>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>

QtHelper::QtHelper() {}

void QtHelper::setLabelIcon(QLabel *label, const QString &fontFile, int iconCode, int fontSize)
{
    if (!label) {
        return;
    }

    // 缓存已加载的字体，避免重复调用 addApplicationFont
    static QHash<QString, QString> fontCache;

    QString fontName;
    if (fontCache.contains(fontFile)) {
        fontName = fontCache.value(fontFile);
    } else {
        int fontId = QFontDatabase::addApplicationFont(fontFile);
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (families.isEmpty()) {
            return;
        }
        fontName = families.at(0);
        fontCache.insert(fontFile, fontName);
    }

    QFont iconFont = QFont(fontName);
    // 如果 fontSize > 0，则使用指定大小；否则继承父控件字体大小
    if (fontSize > 0) {
        iconFont.setPixelSize(fontSize);
    }

    label->setFont(iconFont);
    label->setText(QChar(iconCode));
}

QRect QtHelper::getCurrentScreenAvailableGeometry(QWidget *widget)
{
    if (!widget) {
        // 如果widget为空，返回主屏幕的可用区域
        return QApplication::desktop()->availableGeometry();
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    // Qt 5.10及以上版本，使用QScreen API
    QScreen *screen = QApplication::screenAt(widget->geometry().center());
    if (screen) {
        return screen->availableGeometry();
    }
#else
    // Qt 5.10以下版本，使用QDesktopWidget
    QDesktopWidget *desktop = QApplication::desktop();
    int screenNumber = desktop->screenNumber(widget);
    return desktop->availableGeometry(screenNumber);
#endif

    // 如果无法获取屏幕信息，返回主屏幕的可用区域
    return QApplication::desktop()->availableGeometry();
}

static QSharedMemory* s_instanceMemory = nullptr;

bool QtHelper::checkSingleInstance(const QString& appId)
{
    if (s_instanceMemory) {
        return true;
    }
    
    QSystemSemaphore semaphore(QString("%1_semaphore").arg(appId), 1);
    semaphore.acquire();
    
    QSharedMemory* mem = new QSharedMemory(QString("%1_shared").arg(appId));
    
    if (mem->attach()) {
        delete mem;
        semaphore.release();
        QMessageBox::warning(
            nullptr,
            "程序已在运行",
            "程序已经在运行中，此实例将自动退出。",
            QMessageBox::Ok
        );
        qDebug() << "[SingleInstance] 检测到已有实例在运行，应用ID:" << appId;
        return false;
    }
    
    if (!mem->create(1)) {
        delete mem;
        semaphore.release();
        qWarning() << "[SingleInstance] 创建共享内存失败，应用ID:" << appId;
        return false;
    }
    
    s_instanceMemory = mem;
    semaphore.release();
    qDebug() << "[SingleInstance] 第一个实例启动，应用ID:" << appId;
    return true;
}

void QtHelper::releaseInstance()
{
    if (s_instanceMemory) {
        s_instanceMemory->detach();
        delete s_instanceMemory;
        s_instanceMemory = nullptr;
        qDebug() << "[SingleInstance] 共享内存已释放";
    }
}
