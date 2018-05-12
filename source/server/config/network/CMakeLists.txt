project(server_config_network)

add_library(config_network_client_ssl_auth_lib client_ssl_auth.cc)

add_library(config_network_echo_lib echo.cc)

add_library(config_network_http_connection_manager http_connection_manager.cc)

add_library(config_network_mongo_proxy_lib mongo_proxy.cc)

add_library(config_network_ratelimit_lib ratelimit.cc)

add_library(config_network_raw_buffer_lib OBJECT raw_buffer_socket.cc)

add_library(config_network_redis_proxy_lib redis_proxy.cc)

add_library(config_network_tcp_proxy_lib OBJECT tcp_proxy.cc)
