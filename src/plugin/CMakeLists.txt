item(shakespear_plugin)

set(CONFIG_PATH ${PROBE_PLUGIN_INSTALL_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

sources(_src)

generatePaths(paths SRC)

find_package(Qt5Qml REQUIRED)
find_package(Qt5Network REQUIRED)

# probe part of the plug-in
gammaray_add_plugin(${PROJECT_NAME} JSON metadata/shakespearplugin.json
    SOURCES metadata/ShakespearMetadata.cpp ${_src} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/paths)

target_link_libraries(${PROJECT_NAME}
    gammaray_core shakespear_core appkit_log appkit_translations
    Qt5::Widgets Qt5::Qml Qt5::Network)
linkBoost(${PROJECT_NAME} locale)

# Create logger config
configure_file("_config/shakespear.log.ini.in"
    "${GENERATED_CONFIG}/${PROJECT_NAME}/shakespear.log.ini" @ONLY)

installProjectConfiguration(${PROJECT_NAME} ${CONFIG_PATH} ${PROJECT_NAME})
