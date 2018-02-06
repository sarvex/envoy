project(ssl)

add_library(connection_lib_ssl connection_impl.cc)

add_library(context_config_lib context_config_impl.cc)

add_library(context_lib context_impl.cc context_manager_impl.cc)

add_library(common_ssl_ssl_socket_lib ssl_socket.cc)
