#include <stdio.h>
#include <assert.h>
#include "weatherbot.h"

void test_run_weatherbot() {
    // Test case 1: Invalid Telegram token
    char token[] = "INVALID_TOKEN";
    int result;
    result = run_weatherbot(token);
    assert(result != 0);
}

void test_city_info_parser() {
    float lat, lng;
    int result;

    // Test case 1: Valid latitude:longitude format
    result = city_info_parser("26.25404:29.26754", &lat, &lng);
    assert(result == VALID_GEOCODE);
    assert(lat < 90.0);
    assert(lat > -90.0);
    assert(lng < 180.0);
    assert(lng > -180.0);


    // Test case 2: Invalid latitude:longitude format
    result = city_info_parser("26.2540429.26759", &lat, &lng);
    assert(result == INVALID_GEOCODE);

    // Test case 3: Valid string format
    result = city_info_parser("Egypt", &lat, &lng);
    assert(result == VALID_STRING);

    // Test case 3: Invalid string starts with numbers
    result = city_info_parser("100100Egypt100100", &lat, &lng);
    assert(result == INVALID_GEOCODE);

    // Test case 4: Invalid message (NULL)
    result = city_info_parser(NULL, &lat, &lng);
    assert(result == INVALID_PARAM);

    // Test case 5: Invalid lat or lng (NULL)
    result = city_info_parser("26.25404:29.26754", NULL, &lng);
    assert(result == INVALID_PARAM);
    result = city_info_parser("26.25404:29.26754", &lat, NULL);
    assert(result == INVALID_PARAM);
}

void test_find_city_info_cached() {
    // TBD: Add test case when caching functionality is implemented
}

void test_cache_city_info() {
    // TBD: Add test case when caching functionality is implemented
}

int main() {
    test_run_weatherbot();
    test_city_info_parser();
    test_find_city_info_cached();
    test_cache_city_info();

    DEBUG_PRINT("All tests passed!\n");

    return 0;
}
