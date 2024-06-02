cmake_minimum_required(VERSION 3.10)

project(WeatherBot VERSION 0.1)

include(FindPkgConfig)
#find_package(apr-1 REQUIRED)
#SET(DEPENDENTS "apr-1")
pkg_check_modules(APR1 REQUIRED "apr-1")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${APR1_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${APR1_LIBRARIES})
