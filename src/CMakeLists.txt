cmake_minimum_required(VERSION 3.5.2)

project(thecure)

MESSAGE(STATUS "build project thecure" ${DIR_SRCS})

set(CMAKE_BUILD_TYPE Debug)

include_directories(../include/)

add_compile_options(-std=c++11)

# set(CMAKE_CXX_COMPILER      "clang++" )         # 显示指定使用的C++编译器

# set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11
# set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
# set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告

# set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )             # 调试包不优化
# set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化
# file(GLOB SOURCES "*.cpp")

# aux_source_directory(. DIR_SRCS)
file(GLOB_RECURSE SRC_FILES ./*.cc) 

# build main
add_library(${PROJECT_NAME} ${SRC_FILES})

# link lua.lib
target_link_libraries(${PROJECT_NAME} libluajit-5.1.so dl uv)
#install(TARGETS thecure DESTINATION /usr/local/lib)