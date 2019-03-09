cmake_minimum_required(VERSION 3.9)

include(ExternalProject)

set(GAMMARAY_SRC "${CMAKE_CURRENT_BINARY_DIR}/gammaray-src")
set(GAMMARAY_BIN "${CMAKE_CURRENT_BINARY_DIR}/gammaray-build")

if (NOT DEFINED GAMMARAY_GIT)
    set(GAMMARAY_GIT "https://github.com/KDAB/GammaRay.git")
endif()

set(GAMMARAY_INSTALL_PATH ${INSTALL_PREFIX}/gammaray)

ExternalProject_Add(gammaray_external
  GIT_REPOSITORY    ${GAMMARAY_GIT}
  GIT_TAG           master
  SOURCE_DIR        "${GAMMARAY_SRC}"
  BINARY_DIR        "${GAMMARAY_BIN}"
  STEP_TARGETS build install
  UPDATE_DISCONNECTED true

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${GAMMARAY_INSTALL_PATH}
)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${GAMMARAY_INSTALL_PATH}/lib64/cmake")

include(FeatureSummary)

find_package(GammaRay REQUIRED NO_MODULE)
set_package_properties(GammaRay PROPERTIES
  TYPE REQUIRED
  URL "http://www.kdab.com/gammaray"
  DESCRIPTION "Qt introspection tool"
  PURPOSE "Needed to build Shakespear project."
)

set(PROBE_PLUGIN_INSTALL_DIR "${GAMMARAY_INSTALL_PATH}/${GAMMARAY_PROBE_PLUGIN_INSTALL_DIR}")
