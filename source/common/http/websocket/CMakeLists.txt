project(http_websocket)

add_library(websocket_lib ws_handler_impl.cc)
target_link_libraries(websocket_lib PUBLIC tcp_proxy_lib)

