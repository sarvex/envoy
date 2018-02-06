project(buffer)

add_library(watermark_buffer_lib watermark_buffer.cc)
target_link_libraries(watermark_buffer_lib PUBLIC 
    buffer_lib
)

add_library(buffer_lib buffer_impl.cc)
target_link_libraries(buffer_lib PUBLIC 
    libevent_lib
)

add_library(zero_copy_input_stream_lib zero_copy_input_stream_impl.cc)
