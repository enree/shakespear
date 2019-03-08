item(shakespear_core)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    QT Core
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true
    DEPENDS appkit_core)

lint(${PROJECT_NAME})

tests(core)
