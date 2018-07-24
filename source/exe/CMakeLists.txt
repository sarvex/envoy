project(exe)

#find_library(XXHASH_LIB xxhash.lib)
#find_package( yaml-cpp REQUIRED)

add_library(envoy_main_common_lib main_common.cc
)
target_link_libraries(envoy_main_common_lib PUBLIC
    libevent_lib
    network_utility_lib
    stats_lib 
    thread_local_store_lib
    drain_manager_lib
    options_lib
    server_lib
    config_http_fault_lib
    config_http_grpc_http1_bridge_lib
    config_http_grpc_web_lib    
    config_http_ip_tagging_lib
    config_http_ratelimit_lib
    network_client_ssl_auth_lib
    network_echo_lib  
    network_http_connection_manager
    network_mongo_proxy_lib
    network_ratelimit_lib
    network_redis_proxy_lib
    health_check_lib
    )

#add_library(
#sigaction_lib
#signal_action.cc
#)

add_executable(envoy
    main.cc
    $<TARGET_OBJECTS:config_http_buffer_lib>
    $<TARGET_OBJECTS:config_http_router_lib>
    $<TARGET_OBJECTS:config_network_raw_buffer_lib>
    $<TARGET_OBJECTS:config_network_tcp_proxy_lib>
) 
    
target_link_libraries(
    envoy
    envoy_main_common_lib
    #config_http_zipkin_lib
    xxhash.lib
    base64_lib
    hex_lib
    logger_lib
    thread_lib
    utility_lib
    version_lib
    to_lower_table_lib
    api_lib
    buffer_lib
    zero_copy_input_stream_lib
    dispatcher_lib
    dispatched_thread_lib
    memory_stats_lib
    statsd_lib
    guarddog_lib
    profiler_lib
    codec_lib
    logger_lib
    thread_local_lib
    server_lib_config_validation
    utility_lib
    configuration_lib
    connection_handler_lib
    init_manager_lib
    lds_api_lib
    listener_manager_lib
    watchdog_lib
    worker_lib
	proto_descriptors_lib
    
    buffer_filter_lib
    config_lib_router
    config_utility_lib_router
    rds_lib
    retry_state_lib
    router_lib
    router_ratelimit_lib
    shadow_writer_lib
    async_client_lib
    codec_client_lib
    codes_lib
    conn_manager_lib
    date_provider_lib
    header_map_lib
    message_lib
    rest_api_fetcher_lib
    user_agent_lib
    http_utility_lib
    filter_utility_lib
	header_utility_lib
    admin_lib
	config_tracker_lib
    common_lib
    http_tracer_lib
    #zipkin_lib
    access_log_formatter_lib
    http2_codec_lib
    http2_conn_pool_lib
    codec_client_lib
    http1_codec_lib
    http1_conn_pool_lib
    ratelimit_lib
    cds_api_lib
    cluster_manager_lib
    health_checker_lib
    host_utility_lib
    load_balancer_lib
    logical_dns_cluster_lib
    outlier_detection_lib
    ring_hash_lb_lib
    eds_lib
    sds_subscription_lib
    upstream_lib
    protocol_json_lib
    tls_context_json_lib
    api_lib_config_validation
    async_client_lib_config_validation
    cluster_manager_lib_config_validation
    dispatcher_lib_config_validation
    dns_lib_config_validation
    server_lib_config_validation
    watcher_lib
    filesystem_lib
    config_schemas_lib
    json_loader_lib
    metadata_lib
    config_utility_lib
    shadow_writer_lib
    router_ratelimit_lib
    router_lib
    retry_state_lib
    rds_lib
    address_lib
    runtime_lib
    uuid_util_lib
    context_config_impl_lib
    context_config_lib
    context_lib
    watermark_buffer_lib
    buffer_lib
    zero_copy_input_stream_lib
    cidr_range_lib
    connection_lib
    dns_lib
    filter_manager_lib
    listen_socket_lib
    filter_json_lib
    well_known_names_lib
    utility_lib_protobuf
    singleton_manager_lib
    listener_lib
    access_log_lib
    common_html_utility 
    common_request_info_utility
    common_grpc_async_client_manager_lib
    common_ssl_ssl_socket_lib
    common_network_raw_buffer_socket_lib
	token_bucket_lib
	logger_delegates_lib
    fault_filter_lib
    grpc_web_filter_lib
    http1_bridge_filter_lib
    http_ratelimit_lib
    ip_tagging_filter_lib
	socket_option_factory_lib
    #ratelimit_lib_filter
	ext_file_access_log_lib
    health.lib                
    cares.lib
    event.lib
    event_core.lib
    event_extra.lib
    nghttp2.lib
    Ws2_32.lib
    http_parser.lib
    googleapis.lib
    Rpcrt4.lib
    protoc-gen-validate.lib
    gogoproto.lib
	circllhist.lib
)

target_link_libraries(envoy
	debug fmtd.lib
	optimized fmt.lib
)

target_link_libraries(envoy
    debug libprotobufd.lib
    debug libprotocd.lib

    optimized libprotobuf.lib
    optimized libprotoc.lib
)

target_link_libraries(envoy 
    debug absl_based.lib
    debug absl_stringsd.lib
    debug absl_debuggingd.lib
    debug absl_numericd.lib
	debug absl_synchronizationd.lib
	debug absl_timed.lib
)

target_link_libraries(envoy 
    optimized absl_base.lib
    optimized absl_debugging.lib
    optimized absl_numeric.lib
    optimized absl_strings.lib
	optimized absl_synchronization.lib
	optimized absl_types.lib
	optimized absl_time.lib
)

target_link_libraries(envoy 
    crypto.lib
    decrepit.lib
    ssl.lib
)

target_link_libraries(envoy 
    debug libyaml-cppmdd.lib
    optimized libyaml-cppmd.lib
)


target_link_libraries(envoy 
    data-plane-api.lib
)
