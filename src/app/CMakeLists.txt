item(shakespeare)

generatePaths(shakespear SRC)

executable(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS shakespear_ui gammaray_launcher appkit_log appkit_app log_widget)

lint(${PROJECT_NAME})

