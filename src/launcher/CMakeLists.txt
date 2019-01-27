item(shakespear_launcher)

generatePaths(shakespear SRC)

executable(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS coriolis_config coriolis_app)

lint(${PROJECT_NAME})

