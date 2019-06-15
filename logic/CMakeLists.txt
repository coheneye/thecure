cmake_minimum_required(VERSION 3.5.2)

project(logic)

MESSAGE(STATUS "build executable " ${PROJECT_NAME})

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

# file(GLOB SOURCES "*.cpp")
add_subdirectory(../src thecure)

include_directories(../include)

aux_source_directory(./ DIR_SRCS)

# build main
add_executable(${PROJECT_NAME} ${DIR_SRCS})

# link lua.lib
target_link_libraries(${PROJECT_NAME} thecure)
#install(TARGETS thecure DESTINATION /usr/local/lib)