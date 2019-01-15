item(shakespear_log)

# This definition will be auto included with addModule function
generateLogger(Shakespear _log  ${PROJECT_GENERATED_INCLUDES}/shakespear)

sharedLib(
    ${PROJECT_NAME}
    SOURCES ${_log}
    DEPENDS coriolis_log
    COMPONENT ${PROJECT_NAME})
