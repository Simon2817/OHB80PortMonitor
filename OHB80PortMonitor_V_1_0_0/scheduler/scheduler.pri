INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/scheduler_task.h \
    $$PWD/scheduler.h \
    $$PWD/tasks/monitor_data_task.h \
    $$PWD/tasks/send_command_task.h

SOURCES += \
    $$PWD/scheduler.cpp \
    $$PWD/tasks/monitor_data_task.cpp \
    $$PWD/tasks/send_command_task.cpp
