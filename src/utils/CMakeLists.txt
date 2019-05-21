# all others depend on this library
aux_source_directory(. DIR_UTILS)

message(STATUS "utils=" ${DIR_UTILS})

add_library(utils ${DIR_UTILS})

target_link_libraries(utils uv)