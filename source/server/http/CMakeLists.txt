project(server_http)

add_library(admin_lib admin.cc)

add_library(config_tracker_lib config_tracker_impl.cc)
