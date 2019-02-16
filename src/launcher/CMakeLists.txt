item(launcher)

generatePaths(shakespear SRC)

sharedLib(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets Network
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS coriolis_config coriolis_app gammaray_launcher)

lint(${PROJECT_NAME})

