include ("../../global.pri")

INCLUDEPATH += ../../../src/
DESTDIR = ../

target.path = /lib/flow-pomodoro/plugins/storage/
INSTALLS += target

LIBS += -lflowPrivate -L $$shadowed($$PWD/../../)
