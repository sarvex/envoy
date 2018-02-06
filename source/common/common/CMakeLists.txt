project(buffer)

add_library(base64_lib base64.cc)

add_library(hex_lib hex.cc)

add_library(logger_lib logger.cc)

add_library(thread_lib thread.cc)

add_library(utility_lib utility.cc)

add_library(version_lib version.cc)

add_library(to_lower_table_lib to_lower_table.cc)
