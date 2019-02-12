item(shakespear_plugin)

# probe part of the plug-in
gammaray_add_plugin(${PROJECT_NAME} JSON shapeinjector.json SOURCES ShapeInjector.cpp ShapeInjector.h)
target_link_libraries(${PROJECT_NAME} gammaray_core Qt5::Widgets)

lint(${PROJECT_NAME})
