project(protobuf)

add_library(utility_lib_protobuf utility.cc)
target_link_libraries(utility_lib_protobuf PUBLIC json_loader_lib)
