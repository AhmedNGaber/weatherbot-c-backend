cmake_minimum_required(VERSION 3.10)

project(WeatherBot VERSION 0.1)

include(FindPkgConfig)
pkg_check_modules(APR1 REQUIRED "apr-1")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES} telebot)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${APR1_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${APR1_LIBRARIES})

#TBD: Check if the path can be more configurable
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../weatherbot-rust-c-wrapper/target/debug/libweatherbot_rust_c_wrapper.so)

set(TELEBOTLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/telebot)

if(EXISTS ${TELEBOTLIB_PATH} AND IS_DIRECTORY ${TELEBOTLIB_PATH})
    add_subdirectory(${TELEBOTLIB_PATH})
else()
    message(FATAL_ERROR "The directory ${TELEBOTLIB_PATH} does not exist!\n"
        "Clone the telebot library from https://github.com/smartnode/telebot "
        "and place it in the root directory of this project "
        "or set the TELEBOTLIB_PATH variable to the correct path.\n"
        "Then run 'cmake' again and 'make telebot'"
        "Failed to find telebot library!")
endif()

include_directories(${TELEBOTLIB_PATH}/include)
target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/telebot/libtelebot.so)
