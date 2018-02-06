project(http_filter)

add_library(buffer_filter_lib buffer_filter.cc)

add_library(fault_filter_lib fault_filter.cc)

add_library(ip_tagging_filter_lib ip_tagging_filter.cc)

add_library(http_ratelimit_lib ratelimit.cc)
