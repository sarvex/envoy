project(access_log)

add_library(access_log_formatter_lib access_log_formatter.cc)

add_library(access_log_lib access_log_impl.cc)

add_library(access_log_manager_lib access_log_manager_impl.cc)
