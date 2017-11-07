cmake_minimum_required(VERSION 2.6)

project(avm2)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include_directories(source)

file(GLOB_RECURSE SOURCES source/*.cpp)

add_definitions(-std=c++14 -Wall -Wextra -Werror -Wpedantic)

add_executable(avm2 ${SOURCES})

target_link_libraries(avm2
)
