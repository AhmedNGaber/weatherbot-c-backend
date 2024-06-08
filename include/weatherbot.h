#ifndef WEATHERBOT_H
#define WEATHERBOT_H

/* define the return codes */
#define VALID_STRING        (1)
#define VALID_GEOCODE       (2)


#define INVALID_MESG        (-1)
#define INVALID_PARAM       (-2)
#define INVALID_GEOCODE     (-3)


#define CACHE_SUCCESS       (0)
#define CACHE_FAILURE       (-10)
#define CACHE_NOT_FOUND     (-11)
/* As no location has temperature -200 :D,
 * return it to inform that location info is not cached
 */
#define TEMPERATURE_NOT_CACHED (-200.0f)


#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define TOKEN_MAX_SIZE 128

int run_weatherbot(char* token);
int city_info_parser(const char* message, float* lat, float* lng);
float find_city_info_cached(const char* city_name, float lat, float lng);
int cache_city_info(const char* city_name, float lat, float lng, float temperature);
void* init_cache_invalidate_cache_thread(void *);

#endif // WEATHERBOT_H