FORMS += \
    $$PWD/alarmpage.ui \
    $$PWD/chartpage.ui \
    $$PWD/configpage.ui \
    $$PWD/debugpage.ui \
    $$PWD/homepage.ui \
    $$PWD/uidemo6.ui

HEADERS += \
    $$PWD/alarmpage.h \
    $$PWD/chartpage.h \
    $$PWD/configpage.h \
    $$PWD/debugpage.h \
    $$PWD/homepage.h \
    $$PWD/uidemo6.h

SOURCES += \
    $$PWD/alarmpage.cpp \
    $$PWD/chartpage.cpp \
    $$PWD/configpage.cpp \
    $$PWD/debugpage.cpp \
    $$PWD/homepage.cpp \
    $$PWD/uidemo6.cpp

#customwidget
include ($$PWD/customwidget/customwidget.pri)
INCLUDEPATH += $$PWD/customwidget
