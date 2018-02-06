project(config_http)

add_library(config_http_buffer_lib OBJECT buffer.cc)

#add_library(config_http_cors_lib OBJECT cors.cc)

#add_library(config_http_dynamo_lib dynamo.cc)

add_library(config_http_fault_lib fault.cc)
target_link_libraries(config_http_fault_lib PUBLIC ratelimit_lib_proto)

#add_library(config_http_file_access_log_lib OBJECT file_access_log.cc)
#target_link_libraries(config_http_file_access_log_lib PUBLIC )

add_library(config_http_grpc_http1_bridge_lib grpc_http1_bridge.cc)

#add_library(
#config_http_grpc_json_transcoder_lib
#grpc_json_transcoder.cc
#)

add_library(config_http_grpc_web_lib grpc_web.cc)

add_library(config_http_ip_tagging_lib ip_tagging.cc)

#add_library(
#config_http_lightstep_lib
#lightstep_http_tracer.cc
#)

add_library(config_http_ratelimit_lib ratelimit.cc)

add_library(config_http_router_lib OBJECT router.cc)

add_library(config_http_zipkin_lib zipkin_http_tracer.cc)
