project(router)

#add_library(req_header_formatter_lib req_header_formatter.cc)

add_library(config_utility_lib_router config_utility.cc)

add_library(common_router_header_parser_lib header_parser.cc)

add_library(common_router_header_formatter_lib header_formatter.cc)

add_library(config_lib_router config_impl.cc)
target_link_libraries(config_lib_router PUBLIC
    config_utility_lib_router
    common_router_header_formatter_lib
    common_router_header_parser_lib
)

add_library(rds_subscription_lib rds_subscription.cc)
target_link_libraries(rds_subscription_lib PUBLIC rds_json_lib)

add_library(rds_lib rds_impl.cc)
target_link_libraries(rds_lib PUBLIC rds_subscription_lib)

add_library(retry_state_lib retry_state_impl.cc)

add_library(router_lib router.cc)

add_library(router_ratelimit_lib router_ratelimit.cc)

add_library(shadow_writer_lib shadow_writer_impl.cc)
