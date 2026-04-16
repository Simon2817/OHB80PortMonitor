HEADERS += \
    $$PWD/quiwidget.h

SOURCES += \
    $$PWD/quiwidget.cpp

# overheadcranetrack
include ($$PWD/overheadcranetrack/overheadcranetrack.pri)
INCLUDEPATH += $$PWD/overheadcranetrack

# settingwidget
include ($$PWD/settingwidget/settingwidget.pri)
INCLUDEPATH += $$PWD/settingwidget
