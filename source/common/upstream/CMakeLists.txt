project(upstream)

include_directories(${GoogleApi_Path} / gens)

add_library(cds_subscription_lib cds_subscription.cc)

add_library(cds_api_lib cds_api_impl.cc)
target_link_libraries(cds_api_lib PUBLIC cds_subscription_lib)

add_library(load_stats_reporter_lib load_stats_reporter.cc)

add_library(original_dst_cluster_lib original_dst_cluster.cc)

add_library(subset_lb_lib subset_lb.cc)

add_library(cluster_manager_lib cluster_manager_impl.cc)
target_link_libraries(cluster_manager_lib PUBLIC 
    grpc_mux_impl_lib 
    load_stats_reporter_lib
    original_dst_cluster_lib 
    resolver_impl_lib 
    subset_lb_lib
)

add_library(health_checker_lib health_checker_impl.cc)

add_library(host_utility_lib host_utility.cc)

add_library(load_balancer_lib load_balancer_impl.cc)

add_library(logical_dns_cluster_lib logical_dns_cluster.cc)

add_library(outlier_detection_lib outlier_detection_impl.cc)

add_library(ring_hash_lb_lib ring_hash_lb.cc)

add_library(eds_lib eds.cc)

add_library(sds_subscription_lib sds_subscription.cc)

add_library(upstream_lib upstream_impl.cc)
