#include(GoogleTest)

add_library(upstream_utility_lib INTERFACE)
target_link_libraries(upstream_utility_lib INTERFACE
    json_loader_lib
    cds_json_lib
    network_utility_lib
    upstream_lib
)

target_link_libraries(upstream_utility_lib INTERFACE
    debug gtestd.lib
)

target_link_libraries(upstream_utility_lib INTERFACE
    optimized gtest.lib
)