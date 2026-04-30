INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/scheduler_task.h \
    $$PWD/scheduler.h \
    $$PWD/tasks/monitor_data_task.h \
    $$PWD/tasks/send_command_task.h \
    $$PWD/tasks/init_check_task.h \
    $$PWD/tasks/network_status_task.h \
    $$PWD/tasks/set_firmware_config_task.h \
    $$PWD/tasks/firmware_upgrade_task.h \
    $$PWD/tasks/set_idle_purge_task.h \
    $$PWD/tasks/set_pneumatic_valve_pressure_task.h \
    $$PWD/tasks/sh85_self_check_task.h \
    $$PWD/tasks/set_humidity_offset_task.h \
    $$PWD/tasks/set_purge_flow_task.h \
    $$PWD/tasks/set_vefc_gas_type_task.h \
    $$PWD/tasks/set_ui_refresh_time_task.h \
    $$PWD/tasks/read_vefc_flow_unit_medium_status_task.h

SOURCES += \
    $$PWD/scheduler.cpp \
    $$PWD/tasks/monitor_data_task.cpp \
    $$PWD/tasks/send_command_task.cpp \
    $$PWD/tasks/init_check_task.cpp \
    $$PWD/tasks/network_status_task.cpp \
    $$PWD/tasks/set_firmware_config_task.cpp \
    $$PWD/tasks/firmware_upgrade_task.cpp \
    $$PWD/tasks/set_idle_purge_task.cpp \
    $$PWD/tasks/set_pneumatic_valve_pressure_task.cpp \
    $$PWD/tasks/sh85_self_check_task.cpp \
    $$PWD/tasks/set_humidity_offset_task.cpp \
    $$PWD/tasks/set_purge_flow_task.cpp \
    $$PWD/tasks/set_vefc_gas_type_task.cpp \
    $$PWD/tasks/set_ui_refresh_time_task.cpp \
    $$PWD/tasks/read_vefc_flow_unit_medium_status_task.cpp
