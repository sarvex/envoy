project(stats)

add_library(stats_lib  stats_impl.cc)

add_library(thread_local_store_lib thread_local_store.cc)
