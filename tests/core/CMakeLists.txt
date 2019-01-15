project(shakespear_core_test)

test(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Core
    LIBS shakespear_core)
