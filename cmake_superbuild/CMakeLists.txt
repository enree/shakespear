# Cmake "superbuild" for entire Shakespear project.
# It uses number of external projects to build all dependency in proper order

cmake_minimum_required(VERSION 3.12)
project(shakespear-superbuild C CXX)

include(ExternalProject)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/make.cmake)

definePaths(shakespear)

macro(defineGitSource TARGET DEFAULT NAME)
    if (NOT DEFINED ${TARGET})
        set(${TARGET} ${DEFAULT})
    endif()
    message(STATUS "${NAME}: ${${TARGET}}")
endmacro()

#Add GammaRay project
defineGitSource(GAMMARAY "https://github.com/KDAB/GammaRay.git" "GammaRay project")
set(GAMMARAY_INSTALL_PATH ${INSTALL_PREFIX}/gammaray)

ExternalProject_Add(gammaray
  PREFIX gammaray
  GIT_REPOSITORY    ${GAMMARAY}
  GIT_TAG           master
  STEP_TARGETS build install
  UPDATE_DISCONNECTED true

  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GAMMARAY_INSTALL_PATH}
)

#Add Shakespear project
ExternalProject_Add(shakespear
  PREFIX shakespear
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.."
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shakespear"
  STEP_TARGETS build install
  UPDATE_DISCONNECTED true

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DINSTALL_PREFIX=${INSTALL_PREFIX}
    -DGAMMARAY_INSTALL_PATH=${GAMMARAY_INSTALL_PATH}

  DEPENDS gammaray
)
