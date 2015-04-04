TEMPLATE = subdirs
SUBDIRS += src plugins flow_main

CONFIG += ordered

include ("global.pri")

android {
    OTHER_FILES += android/AndroidManifest.xml
    DISTFILES += android/src/com/kdab/flowpomodoro/MainActivity.java
}
