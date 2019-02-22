project(shakespear_core_test)

gqtest(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Core
    LIBS shakespear_core)
