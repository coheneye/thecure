cmake_minimum_required(VERSION 3.0.2)

project(thecure)

set(CMAKE_BUILD_TYPE Release)

include_directories(./third_party/)

add_compile_options(-std=c++11)

# file(GLOB SOURCES "*.cpp")

aux_source_directory(${PROJECT_SOURCE_DIR} DIR_SRCS)

MESSAGE(STATUS "output" ${PROJECT_SOURCE_DIR})

add_executable(thecure ${DIR_SRCS})

# link lua.lib
# target_link_libraries(thecure liblua.a dl)
target_link_libraries(thecure libluajit-5.1.a dl)
# add_library(thecure SHARED ${DIR_SRCS})
#install(TARGETS thecure DESTINATION /usr/local/lib)