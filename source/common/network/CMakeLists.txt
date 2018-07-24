project(network)

#find_library(FMT_LIB fmt.lib REQUIRED)

add_library(address_lib address_impl.cc)

add_library(cidr_range_lib cidr_range.cc)

add_library(connection_lib connection_impl.cc)

add_library(dns_lib dns_impl.cc)

add_library(filter_manager_lib filter_manager_impl.cc)

add_library(listen_socket_lib listen_socket_impl.cc)

add_library(listener_lib listener_impl.cc)

add_library(common_network_raw_buffer_socket_lib raw_buffer_socket.cc)

add_library(resolver_impl_lib resolver_impl.cc)

add_library(network_utility_lib utility.cc)

add_library(socket_option_factory_lib socket_option_factory.cc)
