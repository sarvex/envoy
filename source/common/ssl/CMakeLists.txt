project(ssl)

add_library(context_config_impl_lib context_config_impl.cc)

add_library(context_config_lib context_config_impl.cc)

add_library(context_lib context_impl.cc context_manager_impl.cc)

add_library(common_ssl_ssl_socket_lib ssl_socket.cc)

add_library(common_tls_certificate_config_lib tls_certificate_config_impl.cc)

add_library(common_utility_lib utility.cc)
