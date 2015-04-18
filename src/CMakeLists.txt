
set(flowPrivate_SRC
    assertingproxymodel.cpp
    checkabletagmodel.cpp
    circularprogressindicator.cpp
    checkbox.cpp
    controller.cpp
    distractionsplugin.cpp
    duedate.cpp
    extendedtagsmodel.cpp
    flowjsonbackendinstance.cpp
    flowjsonplugin.cpp
    genericlistmodel.h
    jsonstorage.cpp
    kernel.cpp
    loadmanager.cpp
    pluginbase.cpp
    pluginmodel.cpp
    quickview.cpp
    nonemptytagfilterproxy.cpp
    runtimeconfiguration.cpp
    settings.cpp
    sortedtagsmodel.cpp
    sortedtaskcontextmenumodel.cpp
    storage.cpp
    storagebackendinstance.cpp
    storagebackendinstancemodel.cpp
    storageplugin.cpp
    syncable.cpp
    tag.cpp
    tagref.cpp
    task.cpp
    taskcontextmenumodel.cpp
    taskfilterproxymodel.cpp
    tooltipcontroller.cpp
    utils.cpp
)

qt5_add_resources(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)

add_definitions(-DBUILDING_FLOW_LIBRARY)
add_definitions(-DNO_WEBDAV)

add_library(flowPrivate SHARED ${flowPrivate_SRC} ${RESOURCES})
set_target_properties(flowPrivate PROPERTIES SOVERSION 1.2.0)

qt5_use_modules(flowPrivate Gui Quick DBus Widgets)

install(TARGETS flowPrivate DESTINATION lib)
