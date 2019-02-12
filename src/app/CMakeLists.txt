item(shakespeare)

generatePaths(shakespear SRC)

executable(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS launcher shakespear_log gammaray_launcher)

lint(${PROJECT_NAME})

