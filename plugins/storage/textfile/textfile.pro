include ("../../global.pri")
include ("../storage.pri")

QT       += qml quick
CONFIG   += plugin
TARGET = textfile
TEMPLATE = lib

HEADERS += textfile.h \
           textfilebackendinstance.h

SOURCES += textfile.cpp \
           textfilebackendinstance.cpp

RESOURCES += textfileplugin.qrc
