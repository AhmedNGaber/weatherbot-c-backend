cmake_minimum_required(VERSION 2.6)

project(WeatherBot)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
