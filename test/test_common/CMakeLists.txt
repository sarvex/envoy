set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_library(environment_lib environment.cc)

target_link_libraries(environment_lib PUBLIC 
    network_utility_lib
    utility_lib
    options_lib
    test_network_utility_lib
    ws2_32.lib
)

target_link_libraries(environment_lib PUBLIC 
    debug absl_based.lib
    debug absl_stringsd.lib
)

target_link_libraries(environment_lib PUBLIC 
    optimized absl_base.lib
    optimized absl_strings.lib
)

add_library(test_network_utility_lib network_utility.cc)
target_link_libraries(test_network_utility_lib PUBLIC 
    address_lib
    common_network_raw_buffer_socket_lib
    utility_lib
    runtime_lib
)

add_library(printers_lib printers.cc)
target_link_libraries(printers_lib PUBLIC 
    buffer_lib
    header_map_lib
    codec_lib
)

add_library(test_common_utility_lib utility.cc)
target_link_libraries(test_common_utility_lib  PUBLIC
    utility_lib
    bootstrap_json_lib
    header_map_lib
    json_loader_lib
    network_utility_lib
    address_lib
    utility_lib_protobuf
    well_known_names_lib
    tls_context_json_lib
    filesystem_lib
    os_sys_calls_impl
    buffer_lib
    libevent_lib
    thread_lib
    cidr_range_lib
    protocol_json_lib

    ws2_32.lib
    event.lib
    event_core.lib
    event_extra.lib
    xxhash.lib
    fmt.lib
    gogoproto.lib
    googleapis.lib
    protoc-gen-validate.lib
    health.lib
)
target_link_libraries(test_common_utility_lib PUBLIC
    debug libprotobufd.lib
    debug libprotocd.lib
)
target_link_libraries(test_common_utility_lib PUBLIC
    debug absl_based.lib
    debug absl_stringsd.lib
    debug libyaml-cppmdd.lib
)
target_link_libraries(test_common_utility_lib PUBLIC
    optimized absl_base.lib
    optimized absl_strings.lib
    optimized libyaml-cppmd.lib
)

target_link_libraries(test_common_utility_lib PRIVATE
    data-plane-api.lib
)