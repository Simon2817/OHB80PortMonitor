#pragma once
#include <QColor>
#include <QFont>

struct CommItemStyle {
    QColor foreground;
    QColor background;
    QFont  font;
    bool   applyForeground = false;
    bool   applyBackground = false;
    bool   applyFont       = false;

    void setForeground(const QColor &c) { foreground = c; applyForeground = true; }
    void setBackground(const QColor &c) { background = c; applyBackground = true; }
    void setFont(const QFont &f)        { font = f;       applyFont       = true; }
};
