project(basic)

executable(${PROJECT_NAME}
    GLOBBING "*"
    QT Core Widgets
    NOINSTALL true)

