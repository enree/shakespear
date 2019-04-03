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

# Add c-ares (for grpc)
defineGitSource(CARES "https://github.com/c-ares/c-ares.git" "c-ares library")
ExternalProject_Add(c-ares
  PREFIX c-ares
  GIT_REPOSITORY ${CARES}
  GIT_TAG master
  CMAKE_ARGS
        -DCARES_SHARED:BOOL=OFF
        -DCARES_STATIC:BOOL=ON
        -DCARES_STATIC_PIC:BOOL=ON
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares
)

# Add protobuf (for grpc)
defineGitSource(PROTOBUF "https://github.com/protocolbuffers/protobuf.git" "Protocol Buffers")
ExternalProject_Add(protobuf
  PREFIX protobuf
  GIT_REPOSITORY ${PROTOBUF}
  GIT_TAG master
  SOURCE_SUBDIR cmake
  CMAKE_ARGS
        -Dprotobuf_BUILD_TESTS:BOOL=OFF
        -Dprotobuf_WITH_ZLIB:BOOL=OFF
        -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/protobuf
)

# Add zlib (for grpc)
defineGitSource(ZLIB "https://github.com/madler/zlib.git" "ZLIB DATA COMPRESSION LIBRARY")
ExternalProject_Add(zlib
  PREFIX zlib
  GIT_REPOSITORY ${ZLIB}
  GIT_TAG master
  CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/zlib
)

# Add grpc (for grpc)
# the location where protobuf-config.cmake will be installed varies by platform
if (WIN32)
  set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake")
else()
  set(_FINDPACKAGE_PROTOBUF_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/protobuf/lib64/cmake/protobuf")
endif()

# if OPENSSL_ROOT_DIR is set, propagate that hint path to the external projects with OpenSSL dependency.
set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "")
if (OPENSSL_ROOT_DIR)
  set(_CMAKE_ARGS_OPENSSL_ROOT_DIR "-DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}")
endif()

# Builds gRPC based on locally checked-out sources and set arguments so that all the dependencies
# are correctly located.
defineGitSource(GRPC "https://github.com/grpc/grpc.git" "gRPC - An RPC library")
ExternalProject_Add(grpc
  PREFIX grpc
  GIT_REPOSITORY ${GRPC}
  GIT_TAG master
  GIT_SUBMODULES "third_party/zlib"
  CMAKE_ARGS
        -DgRPC_INSTALL:BOOL=ON
        -DgRPC_BUILD_TESTS:BOOL=OFF
        -DgRPC_PROTOBUF_PROVIDER:STRING=package
        -DgRPC_PROTOBUF_PACKAGE_TYPE:STRING=CONFIG
        -DProtobuf_DIR:PATH=${_FINDPACKAGE_PROTOBUF_CONFIG_DIR}
        -DgRPC_ZLIB_PROVIDER:STRING=package
        -DZLIB_ROOT:STRING=${CMAKE_CURRENT_BINARY_DIR}/zlib
        -DgRPC_CARES_PROVIDER:STRING=package
        -Dc-ares_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}/c-ares/lib/cmake/c-ares
        -DgRPC_SSL_PROVIDER:STRING=package
        ${_CMAKE_ARGS_OPENSSL_ROOT_DIR}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/grpc
  DEPENDS c-ares protobuf zlib
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
)
