project(buffer)

add_library(filesystem_lib filesystem_impl.cc)
target_link_libraries(filesystem_lib PUBLIC os_sys_calls_impl)

add_library(watcher_lib win32/watcher_impl.cc)
