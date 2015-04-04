set(flow_SRC
    main.cpp
)

add_executable(flow ${flow_SRC})
target_link_libraries(flow flowPrivate)
qt5_use_modules(flow Gui Quick DBus Widgets)
install(TARGETS flow DESTINATION bin)
