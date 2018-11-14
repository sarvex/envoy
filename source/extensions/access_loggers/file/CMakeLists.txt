add_library(config_file_access_log_lib OBJECT config.cc)
add_library(extension_file_access_log_lib STATIC file_access_log_impl.cc)
