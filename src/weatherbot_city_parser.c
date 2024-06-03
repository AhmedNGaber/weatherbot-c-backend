#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "weatherbot.h"
#include "weatherbot_rust_wrapper.h"

/**
 * Parses the city information.
 *
 * @param message The input string containing the city information.
 * @param lat The output latitude in case it is valid latitude:longitude to be sent back to the caller.
 * @param lng The output longitude in case it is valid latitude:longitude to be sent back to the caller.
 * @return The parsed city information as a struct.
 */
int city_info_parser(const char* message, float* lat, float* lng) {

    if (message == NULL) {
        return INVALID_PARAM;
    }

    /* in case no valid pointer is sent, just ignore*/
    if (lat == NULL || lng == NULL) {
        return INVALID_PARAM;
    }

    /* check the format of the message , sometimes just numbers are passed*/
    if (atoi(message) || (strstr(message, ":") != NULL)) {
        /* check if the message is in the format of latitude:longitude */
        if (sscanf(message, "%f:%f", lat, lng) != 2) {
            return INVALID_GEOCODE;
        } 

        /* lat and lng now contain float numbers, verify that they are valid range for geometry code */
        if (*lat < -90.0 || *lat > 90.0 || *lng < -180.0 || *lng > 180.0) {
            return INVALID_GEOCODE;
        }
        return VALID_GEOCODE;
    } 
    return VALID_STRING;
}

/**
 * Finds the city information in the cache.
 *
 * @param city_name The city name to search for.
 * @param lat The latitude to search for.
 * @param lng The longitude to search for.
 * @return The temperature of the city if found, otherwise TEMPERATURE_NOT_CACHED.
 */
int find_city_info_cached(const char* city_name, float lat, float lng) {

    //TBD: Use APR lib search in the cached temperatures then return the temperature if found

    return TEMPERATURE_NOT_CACHED;
}

int cache_city_info(const char* city_name, float lat, float lng, float temperature) {

    //TBD: Use APR lib to cache the temperatures to not call open-meteo API every time

    return 0;
}