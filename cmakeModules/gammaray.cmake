cmake_minimum_required(VERSION 3.9)

include(ExternalProject)

set(GAMMARAY_SRC "${CMAKE_CURRENT_BINARY_DIR}/gammaray-src")
set(GAMMARAY_BIN "${CMAKE_CURRENT_BINARY_DIR}/gammaray-build")

if (NOT DEFINED GAMMARAY_GIT)
    set(GAMMARAY_GIT "https://github.com/KDAB/GammaRay.git")
endif()

ExternalProject_Add(gammaray_external
  GIT_REPOSITORY    ${GAMMARAY_GIT}
  GIT_TAG           master
  SOURCE_DIR        "${GAMMARAY_SRC}"
  BINARY_DIR        "${GAMMARAY_BIN}"
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -S <SOURCE_DIR> -B <BINARY_DIR>
  BUILD_COMMAND ${CMAKE_COMMAND} -E echo "Starting GammaRay build"
  COMMAND       ${CMAKE_MAKE_PROGRAM} -j8
  COMMAND       ${CMAKE_COMMAND} -E echo "GammaRay build complete"

  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
  UPDATE_DISCONNECTED true
)

set_target_properties(gammaray_external
    PROPERTIES
    GAMMARAY_INSTALL_DIR "${GAMMARAY_SRC}/googletest/include"
    GAMMARAY_PLUGINS_DIR "${GAMMARAY_BIN}/lib")

include(FeatureSummary)

find_package(GammaRay REQUIRED NO_MODULE)
set_package_properties(GammaRay PROPERTIES
  TYPE REQUIRED
  URL "http://www.kdab.com/gammaray"
  DESCRIPTION "Qt introspection tool"
  PURPOSE "Needed to build Shakespear project."
)

