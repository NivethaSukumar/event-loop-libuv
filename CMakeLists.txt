cmake_minimum_required(VERSION 3.5)

project("event_loop_libuv")

set(CMAKE_C_EXTENSIONS On)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory("app")