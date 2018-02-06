project(http_http1)

add_library(http1_codec_lib codec_impl.cc)

add_library(http1_conn_pool_lib conn_pool.cc)
