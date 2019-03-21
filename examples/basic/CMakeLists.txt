project(basic)

executable(${PROJECT_NAME}
    GLOBBING "*"
    COMPONENT ${PROJECT_NAME}
    QT Core Widgets)

