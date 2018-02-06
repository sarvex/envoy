project(http_http2)

add_library(http2_codec_lib codec_impl.cc)

add_library(http2_conn_pool_lib conn_pool.cc)
