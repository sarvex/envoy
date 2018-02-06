project(ratelimit)

find_package(protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR}/source)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ratelimit.proto)

add_library(ratelimit_lib_proto ${PROTO_HDRS} ${PROTO_SRCS})
target_include_directories(ratelimit_lib_proto PUBLIC ${CMAKE_BINARY_DIR})

add_library(ratelimit_lib ratelimit_impl.cc)
target_link_libraries(ratelimit_lib PUBLIC ratelimit_lib_proto)
