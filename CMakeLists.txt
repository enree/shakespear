cmake_minimum_required(VERSION 3.8)
project(shakespear)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/make.cmake)

set(IS_ROOT true)
definePaths(${PROJECT_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/modules/gqtest/gqtest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/modules/appkit/appkit.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules/gammaray.cmake)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGSL_THROW_ON_CONTRACT_VIOLATION")

defineProjectIncludes(${CMAKE_CURRENT_BINARY_DIR})

# Project main source
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Project examples
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

