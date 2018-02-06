project(api)

add_library(api_lib api_impl.cc)
target_link_libraries(api_lib PUBLIC 
    os_sys_calls_impl
    filesystem_lib
)

add_library(os_sys_calls_impl os_sys_calls_impl.cc)
