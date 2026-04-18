INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/scheduler_task.h \
    $$PWD/scheduler.h \
    $$PWD/tasks/monitor_data_task.h \
    $$PWD/tasks/send_command_task.h \
    $$PWD/tasks/init_check_task.h \
    $$PWD/tasks/network_status_task.h \
    $$PWD/tasks/set_firmware_config_task.h \
    $$PWD/tasks/firmware_upgrade_task.h

SOURCES += \
    $$PWD/scheduler.cpp \
    $$PWD/tasks/monitor_data_task.cpp \
    $$PWD/tasks/send_command_task.cpp \
    $$PWD/tasks/init_check_task.cpp \
    $$PWD/tasks/network_status_task.cpp \
    $$PWD/tasks/set_firmware_config_task.cpp \
    $$PWD/tasks/firmware_upgrade_task.cpp
