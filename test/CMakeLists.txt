SET(TEST_NAME weatherbot_test)


file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS *.c)

add_executable(${TEST_NAME} ${TEST_SRC} ../src/weatherbot_city_parser.c ../src/weatherbot_telebot.c)
target_link_libraries(${TEST_NAME} ${APR1_LIBRARIES})
