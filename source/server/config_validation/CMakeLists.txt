project(server_config_validation)

add_library(admin_lib_config_validation admin.cc)

add_library(api_lib_config_validation api.cc)

add_library(async_client_lib_config_validation async_client.cc)

add_library(cluster_manager_lib_config_validation cluster_manager.cc)

add_library(dispatcher_lib_config_validation dispatcher.cc)

add_library(dns_lib_config_validation dns.cc)

add_library(server_lib_config_validation server.cc)
target_link_libraries(server_lib_config_validation PUBLIC admin_lib_config_validation)
