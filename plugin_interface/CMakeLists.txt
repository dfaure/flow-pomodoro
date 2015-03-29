set(SRCS plugininterface.cpp)

add_definitions(-DBUILDING_FLOW_PLUGIN_INTERFACE)

include_directories("${PROJECT_SOURCE_DIR}/src/")

add_library(flowplugininterface SHARED ${SRCS})
set_target_properties(flowplugininterface PROPERTIES SOVERSION 1.2.0)

qt5_use_modules(flowplugininterface Core Gui Quick)
install(TARGETS flowplugininterface DESTINATION lib)
