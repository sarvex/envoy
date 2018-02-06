project(http)

add_subdirectory(http1)
add_subdirectory(http2)
add_subdirectory(websocket)

add_library(async_client_lib async_client_impl.cc)

add_library(codec_client_lib codec_client.cc)

add_library(codes_lib codes.cc)

add_library(conn_manager_lib conn_manager_impl.cc conn_manager_utility.cc)
target_link_libraries(conn_manager_lib PUBLIC websocket_lib)

add_library(date_provider_lib date_provider_impl.cc)

add_library(header_map_lib header_map_impl.cc)

add_library(message_lib message_impl.cc)

add_library(rest_api_fetcher_lib rest_api_fetcher.cc)

add_library(user_agent_lib user_agent.cc)

add_library(http_utility_lib utility.cc)

add_library(filter_utility_lib filter_utility.cc)

add_library(header_utility_lib header_utility.cc)
