project(buffer)

add_library(address_json_lib address_json.cc)

add_library(cds_json_lib cds_json.cc)
target_link_libraries(cds_json_lib PUBLIC address_json_lib)

add_library(lds_json_lib lds_json.cc)
target_link_libraries(lds_json_lib PUBLIC address_json_lib)

add_library(bootstrap_json_lib bootstrap_json.cc)
target_link_libraries(bootstrap_json_lib PUBLIC cds_json_lib lds_json_lib)

add_library(datasource_lib datasource.cc)

add_library(metadata_lib metadata.cc)

add_library(config_utility_lib utility.cc)

add_library(tls_context_json_lib tls_context_json.cc)

add_library(protocol_json_lib protocol_json.cc)

add_library(base_json_lib base_json.cc)

add_library(rds_json_lib rds_json.cc)
target_link_libraries(rds_json_lib PUBLIC base_json_lib)

add_library(filter_json_lib filter_json.cc)
target_link_libraries(filter_json_lib PUBLIC rds_json_lib)

add_library(grpc_mux_impl_lib grpc_mux_impl.cc)

add_library(well_known_names_lib well_known_names.cc)
