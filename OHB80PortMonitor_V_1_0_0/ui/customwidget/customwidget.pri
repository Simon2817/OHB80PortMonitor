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

# debugsettingwidget
include ($$PWD/debugsettingwidget/debugsettingwidget.pri)
INCLUDEPATH += $$PWD/debugsettingwidget

# loggerwidget
include ($$PWD/loggerwidget/loggerwidget.pri)
INCLUDEPATH += $$PWD/loggerwidget

# alarmloggerwidget
include ($$PWD/alarmloggerwidget/alarmloggerwidget.pri)
INCLUDEPATH += $$PWD/alarmloggerwidget

# historycalendardialog
include($$PWD/historycalendardialog/historycalendardialog.pri)
INCLUDEPATH += $$PWD/historycalendardialog

# communicateloggerwidget
include($$PWD/communicateloggerwidget/communicateloggerwidget.pri)
INCLUDEPATH += $$PWD/communicateloggerwidget

FORMS +=
