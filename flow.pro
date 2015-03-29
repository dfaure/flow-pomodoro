TEMPLATE = subdirs
SUBDIRS += plugin_interface plugins src

CONFIG += ordered

include ("global.pri")

android {
    OTHER_FILES += android/AndroidManifest.xml
    DISTFILES += android/src/com/kdab/flowpomodoro/MainActivity.java
}
