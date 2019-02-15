item(shakespear_plugin)

set(CMAKE_AUTOMOC ON)

collectSources(SOURCES _src)
find_package(Qt5Qml REQUIRED)

# probe part of the plug-in
gammaray_add_plugin(${PROJECT_NAME} JSON shapeinjector.json SOURCES ShapeInjector.cpp ${_src})
target_link_libraries(${PROJECT_NAME} gammaray_core shakespear_core Qt5::Widgets Qt5::Qml)

