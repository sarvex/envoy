project(grpc)

add_library(common_grpc_async_client_lib async_client_impl.cc)

add_library(common_grpc_async_client_manager_lib async_client_manager_impl.cc)
target_link_libraries(common_grpc_async_client_manager_lib PUBLIC
    common_grpc_async_client_lib
)

add_library(codec_lib codec.cc)
target_link_libraries(codec_lib PUBLIC
    base64_lib
)

add_library(common_lib common.cc)

add_library(common_grpc_status_lib status.cc)

#add_library(
#json_transcoder_filter_lib
#json_transcoder_filter.cc
#)

#add_library(rpc_channel_lib rpc_channel_impl.cc)

#add_library(
#transcoder_input_stream_lib
#transcoder_input_stream_impl.cc
#)
