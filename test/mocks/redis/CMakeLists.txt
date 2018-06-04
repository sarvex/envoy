add_library(redis_mocks mocks.cc)

target_link_libraries(redis_mocks
    assert_lib
    codec_lib
)