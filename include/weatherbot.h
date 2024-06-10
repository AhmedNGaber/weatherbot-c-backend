#ifndef WEATHERBOT_H
#define WEATHERBOT_H
#include <stdbool.h>

/* define the return codes */
#define VALID_STRING        (1)
#define VALID_GEOCODE       (2)


#define INVALID_MESG        (-1)
#define INVALID_PARAM       (-2)
#define INVALID_GEOCODE     (-3)


#define CACHE_SUCCESS       (0)

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define TOKEN_MAX_SIZE 128

void cache_init(void);
int run_weatherbot(char* token);
int city_info_parser(const char* message, float* lat, float* lng);
bool find_city_info_cached(const char* city_name, float* temperature);
void cache_city_info(const char* city_name, float temperature);

#endif // WEATHERBOT_H