item(log_widget)

sharedLib(
    ${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Gui Widgets
    DEPENDS appkit_log
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})
