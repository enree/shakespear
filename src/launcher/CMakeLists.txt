item(shakespear_launcher)

generatePaths(shakespear SRC)

sharedLib(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets Network
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS appkit_log appkit_core appkit_config appkit_app gammaray_launcher)

lint(${PROJECT_NAME})

