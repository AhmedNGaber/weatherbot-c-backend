#ifndef WEATHERBOT_H
#define WEATHERBOT_H

/* define the return codes */
#define INVALID_MESG        (-1)
#define INVALID_PARAM       (-2)
#define INVALID_GEOCODE     (-3)

#define VALID_STRING        (1)
#define VALID_GEOCODE       (2)

/* As no location has temperature -200 :D,
 * return it to inform that location info is not cached
 */
#define TEMPERATURE_NOT_CACHED (-200)


#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define TOKEN_MAX_SIZE 128

int run_weatherbot(char* token);
int city_info_parser(const char* message, float* lat, float* lng);
int find_city_info_cached(const char* city_name, float lat, float lng);
int cache_city_info(const char* city_name, float lat, float lng, float temperature);

#endif // WEATHERBOT_H