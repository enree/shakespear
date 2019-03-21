project(shakespear)

# Add translation
set (TRANSLATION_DOMAIN "shakespear")
generateTranslationMacros(SHAKESPEAR
    ${PROJECT_GENERATED_INCLUDES}/${PROJECT_NAME} TR_DOMAIN ${TRANSLATION_DOMAIN})

add_subdirectory(core)
add_subdirectory(launcher)
add_subdirectory(plugin)
add_subdirectory(ui)
add_subdirectory(app)

compileTranslations(${TRANSLATION_DOMAIN})

# Create agent config
configure_file("_config/aut.conf.in"
    "${GENERATED_CONFIG}/${PROJECT_NAME}/aut.conf" @ONLY)
# Create logger config
configure_file("_config/shakespear.log.ini.in"
    "${GENERATED_CONFIG}/${PROJECT_NAME}/shakespear.log.ini" @ONLY)


installProjectConfiguration(${PROJECT_NAME} ${CONFIG_PATH} ${PROJECT_NAME})
