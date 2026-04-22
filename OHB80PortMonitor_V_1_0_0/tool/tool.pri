# loggermanange
include ($$PWD/loggermananger/loggermananger.pri)
INCLUDEPATH += $$PWD/loggermananger

# qthelper
include ($$PWD/qthelper/qthelper.pri)
INCLUDEPATH += $$PWD/qthelper

# binfilereader
include ($$PWD/binfilereader/binfilereader.pri)
INCLUDEPATH += $$PWD/binfilereader

# communicationrecorder - 通讯记录采集器（节流器），根据 Foup 状态对 Modbus 通讯指令上报频率进行节流（工作中 1s，空闲 3s），减轻 UI 日志写入压力
include ($$PWD/communicationrecorder/communicationrecorder.pri)
INCLUDEPATH += $$PWD/communicationrecorder
