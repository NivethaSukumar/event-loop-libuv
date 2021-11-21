cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME app)

project(${TARGET_NAME} C)

set(SOURCES "src/main.c")
set(LIBUV_LIBRARIES uv)

add_executable(${TARGET_NAME} ${SOURCES})

include_directories("${PROJECT_SOURCE_DIR}")
target_link_libraries(${TARGET_NAME} uv)
