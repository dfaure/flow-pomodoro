TEMPLATE = lib
TARGET = flowplugininterface

HEADERS += plugininterface.h
SOURCES += plugininterface.cpp

INCLUDEPATH += $$PWD/../src/
DEFINES += BUILDING_FLOW_PLUGIN_INTERFACE
DESTDIR = ../

QT += quick

!mingw:VERSION = 1.2
