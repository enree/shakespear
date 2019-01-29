cmake_minimum_required(VERSION 3.8)
project(shakespear)

set(BUILD_SCRIPTS_PATH ${CMAKE_SOURCE_DIR}/build)
include(${BUILD_SCRIPTS_PATH}/make.cmake)
include(${BUILD_SCRIPTS_PATH}/check.cmake)
include(${BUILD_SCRIPTS_PATH}/pack.cmake)

definePaths(${PROJECT_NAME})
defineProjectIncludes(${CMAKE_CURRENT_BINARY_DIR})

set(_log_syslog OFF)

addModule(coriolis)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGSL_THROW_ON_CONTRACT_VIOLATION")

#TEMPORARY
set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/home/yuri/gammaray/lib64/cmake")
set(PROBE_PLUGIN_INSTALL_DIR "/home/yuri/gammaray/lib64/gammaray/2.11/qt5_12-x86_64/")

include(FeatureSummary)

find_package(GammaRay REQUIRED NO_MODULE)
set_package_properties(GammaRay PROPERTIES
  TYPE REQUIRED
  URL "http://www.kdab.com/gammaray"
  DESCRIPTION "Qt introspection tool"
  PURPOSE "Needed to build Shakespear project."
)


# Project main source
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Code checks
cppcheck(shakespear SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)

