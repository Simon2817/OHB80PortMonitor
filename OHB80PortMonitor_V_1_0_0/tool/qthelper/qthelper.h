#ifndef QTHELPER_H
#define QTHELPER_H

#include <QByteArray>
#include <QVector>

class QLabel;
class QString;
class QWidget;
class QRect;

class QtHelper
{
public:
    QtHelper();

    // 设置字符图标
    static void setLabelIcon(QLabel *label, const QString &fontFile, int iconCode, int fontSize);

    // 获取窗口当前所在屏幕的可用区域
    static QRect getCurrentScreenAvailableGeometry(QWidget *widget);

    // 单实例检查：确保软件只能被打开一次
    static bool checkSingleInstance(const QString& appId);
    static void releaseInstance();

    /**
     * KMP 字节序列搜索（O(n+m)）
     * 在 buffer 中搜索第一个 pattern 出现位置
     * @return 起始下标，未找到返回 -1
     */
    static int kmpSearch(const QByteArray &buffer, const QByteArray &pattern);

};

#endif // QTHELPER_H
