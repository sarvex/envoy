cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_TOOLCHAIN_FILE ${ENVOY_DEPENDENCIES_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(Envoy)

find_package(Protobuf REQUIRED)


add_definitions(-DNOMINMAX)
add_definitions(-DXXH_STATIC_LINKING_ONLY)
add_definitions(-DCARES_STATICLIB)
add_definitions(-DDISABLE_PROTO_VALIDATE)


include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/vcpkg/installed/x64-windows-static/include)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/vcpkg/installed/x64-windows/include)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/vcpkg/buildtrees/grpc/src/grpc-1.8.3)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/source)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/data-plane-api)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/googleapis)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/protoc-gen-validate)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/gogoproto)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/opencensus-proto)
include_directories(${ENVOY_DEPENDENCIES_ROOT}/gens/prometheus)

LINK_DIRECTORIES(${ENVOY_DEPENDENCIES_ROOT}/gens/lib)
LINK_DIRECTORIES(${ENVOY_DEPENDENCIES_ROOT}/vcpkg/installed/x64-windows-static/debug/lib)
LINK_DIRECTORIES(${ENVOY_DEPENDENCIES_ROOT}/vcpkg/installed/x64-windows/debug/lib)

if (MSVC)
  foreach (flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD") 
  endforeach(flag_var)
endif(MSVC)

add_subdirectory(source)
