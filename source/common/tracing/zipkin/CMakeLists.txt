project(zipkin)

add_library(zipkin_lib
    span_buffer.cc
    span_context.cc
    tracer.cc
    util.cc
    zipkin_core_types.cc
    zipkin_tracer_impl.cc
)
