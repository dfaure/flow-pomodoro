include ("../../global.pri")

INCLUDEPATH += ../../../src/
DESTDIR = ../

target.path = /lib/flow-pomodoro/plugins/distractions/
INSTALLS += target
LIBS += -lflowPrivate -L $$shadowed($$PWD/../../)
