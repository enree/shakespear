cmake_minimum_required(VERSION 3.8)
project(shakespear)

set(BUILD_SCRIPTS_PATH ${CMAKE_SOURCE_DIR}/build)
include(${BUILD_SCRIPTS_PATH}/make.cmake)
include(${BUILD_SCRIPTS_PATH}/check.cmake)
include(${BUILD_SCRIPTS_PATH}/pack.cmake)

definePaths(${PROJECT_NAME})
defineProjectIncludes(${CMAKE_CURRENT_BINARY_DIR})

addModule(coriolis)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGSL_THROW_ON_CONTRACT_VIOLATION")

# Project main source
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Code checks
cppcheck(shakespear SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src)

