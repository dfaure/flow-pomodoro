TEMPLATE = lib
TARGET = flowplugininterface

HEADERS += distractionsplugin.h \
           plugininterface.h

SOURCES += distractionsplugin.cpp \
           plugininterface.cpp

INCLUDEPATH += $$PWD/../src/
DEFINES += BUILDING_FLOW_PLUGIN_INTERFACE
DESTDIR = ../

QT += quick

!mingw:VERSION = 1.2
