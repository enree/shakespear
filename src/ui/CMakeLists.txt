item(shakespear_ui)

add_subdirectory(logging)

sharedLib(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*"
    QT Core Gui Widgets
    COMPONENT ${PROJECT_NAME}
    DEPENDS shakespear_launcher)

lint(${PROJECT_NAME})

