project(buffer)

add_library(dispatcher_lib
    dispatcher_impl.cc
    event_impl_base.cc
    file_event_impl.cc
    signal_impl.cc
    timer_impl.cc)

add_library(libevent_lib libevent.cc)

add_library(dispatched_thread_lib dispatched_thread.cc)
