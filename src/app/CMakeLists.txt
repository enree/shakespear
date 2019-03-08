item(shakespeare)

generatePaths(shakespear SRC)

executable(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS shakespear_launcher gammaray_launcher appkit_log appkit_app)

lint(${PROJECT_NAME})

