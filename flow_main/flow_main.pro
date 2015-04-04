include ("../global.pri")

TEMPLATE = app
TARGET = flow

SOURCES = main.cpp
LIBS += -lflowPrivate
QT += quick

LIBS += -lflowPrivate -L $$shadowed($$PWD/../)

static {
    DEFINES += FLOW_STATIC_BUILD
    LIBS += -lshellscript -lhosts
    LIBPATH += ../plugins/
}

DESTDIR = ../
