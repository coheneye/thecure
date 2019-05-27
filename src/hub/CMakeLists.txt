# "hub" library

aux_source_directory(. DIR_HUB)

message(STATUS "dir_hub=" ${DIR_HUB})

include_directories(../utils ../third_party)

add_library(hub ${DIR_HUB})

target_link_libraries(hub utils third_party) # pthread used by async spdlog
