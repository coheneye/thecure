# "third_party" library

aux_source_directory(. DIR_THIRD_PARTY)

message(STATUS "third_party=" ${DIR_THIRD_PARTY})

include_directories(../utils)

add_library(third_party ${DIR_THIRD_PARTY})

target_link_libraries(third_party pthread utils) # pthread used by async spdlog
