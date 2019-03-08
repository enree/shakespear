project(shakespear_core_test)

gtest(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Core
    LIBS shakespear_core)
