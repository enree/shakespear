item(shakespear_plugin)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    QT Core
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS coriolis_core)

lint(${PROJECT_NAME})
