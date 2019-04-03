cmake_minimum_required(VERSION 3.9)

include(ExternalProject)

if (NOT DEFINED GAMMARAY_INSTALL_PATH)
    message(WARNING "No GAMMARAY_INSTALL_PATH defined")
else()
    message("GAMMARAY_INSTALL_PATH=${GAMMARAY_INSTALL_PATH}")
    set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${GAMMARAY_INSTALL_PATH}/lib64/cmake")
endif()


include(FeatureSummary)

find_package(GammaRay REQUIRED NO_MODULE)
set_package_properties(GammaRay PROPERTIES
  TYPE REQUIRED
  URL "http://www.kdab.com/gammaray"
  DESCRIPTION "Qt introspection tool"
  PURPOSE "Needed to build Shakespear project."
)

if (DEFINED GAMMARAY_INSTALL_PATH)
    set(PROBE_PLUGIN_INSTALL_DIR "${GAMMARAY_INSTALL_PATH}/${GAMMARAY_PROBE_PLUGIN_INSTALL_DIR}")
else()
    set(PROBE_PLUGIN_INSTALL_DIR ${GAMMARAY_PROBE_PLUGIN_INSTALL_DIR})
endif()
