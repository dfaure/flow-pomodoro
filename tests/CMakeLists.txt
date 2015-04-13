
set(tests_SRC
    main.cpp
    modelsignalspy.cpp
    quick/testui.cpp
    signalspy.cpp
    testarchivedtasksmodel.cpp
    testcheckabletagmodel.cpp
    testduedate.cpp
    testtaskfiltermodel.cpp
    teststorage.cpp
    testbase.cpp
    teststagedtasksmodel.cpp
    testtag.cpp
    testtask.cpp
    testtagmodel.cpp
)

add_definitions(-DNO_WEBDAV)
add_executable(tests ${tests_SRC})
target_link_libraries(tests flowPrivate)
qt5_use_modules(tests Gui Quick DBus Widgets Test)
