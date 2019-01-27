project(shakespear)

# Add translation
generateTranslationMacros(
    ${PROJECT_GENERATED_INCLUDES}/${PROJECT_NAME}
    TARGET ${TRANSLATION_DOMAIN})

add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(launcher)
add_subdirectory(plugin)

compileTranslations()
