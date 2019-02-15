project(shakespear)

# Add translation
generateTranslationMacros(
    ${PROJECT_GENERATED_INCLUDES}/${PROJECT_NAME}
    TARGET ${TRANSLATION_DOMAIN})

add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(launcher)
add_subdirectory(plugin)
add_subdirectory(app)

compileTranslations()

createLoggerConfig(${PROJECT_NAME})

# Create agent config
configure_file("config/aut.conf.in"
    "${GENERATED_CONFIG}/${PROJECT_NAME}/aut.conf" @ONLY)

installProjectConfiguration(${PROJECT_NAME} ${CONFIG_PATH} ${COMPONENT_NAME})
