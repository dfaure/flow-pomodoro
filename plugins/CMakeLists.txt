include_directories("${PROJECT_SOURCE_DIR}/src/")

macro(add_distractions_plugin _name _has_resources)
    add_flow_plugin(${_name} distractions ${_has_resources})
endmacro()

macro(add_flow_plugin _name _type _has_resources)
    add_definitions(-DQT_PLUGIN)
    add_definitions(-DQT_SHARED)

    if (${_has_resources})
        qt5_add_resources(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${_name}plugin.qrc)
        add_library(${_name} SHARED ${_name}.cpp ${RESOURCES})
    else()
        add_library(${_name} SHARED ${_name}.cpp)
    endif()
    qt5_use_modules(${_name} Gui Quick DBus)
    install(TARGETS ${_name} DESTINATION lib${LIB_SUFFIX}/flow-pomodoro/plugins/${_type}/)
endmacro()

add_subdirectory(distractions)
