#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <apr_pools.h>
#include <apr_strings.h>

#include "weatherbot.h"

#define GEOCODE_TOLERANCE   (0.0001f)


typedef struct {
    char *city_name;
    float lat;
    float lng;
    float temperature;
} city_info_t;

typedef struct apr_cache_entry_t {
struct apr_cache_entry_t * next;
city_info_t * city_info;
} apr_cache_entry_t;



/* Global variables */
static apr_pool_t *cache_pool = NULL;
static bool cache_initialized = false;
static apr_cache_entry_t *cache_head = NULL;

static bool geocode_compare(float lat1, float lng1, float lat2, float lng2) {
    /* Compare between the float values with tolerance, 
     * by checking the differnce between the two numbers
     * and to get the abslote of this comparing it with a predefined tolerance */
    printf("lat1: %f, lng1: %f, lat2: %f, lng2: %f\n", lat1, lng1, lat2, lng2);
    return ((fabs(lat1 - lat2) < GEOCODE_TOLERANCE) && (fabs(lng1 - lng2) < GEOCODE_TOLERANCE));
}

static void cache_init()
{
    /* Initialize the cache here */
    if (cache_initialized)
        return;

    apr_initialize();
    apr_pool_create(&cache_pool, NULL);
    cache_initialized = true;
}

/* The passed pointer should already be allocated before using cache_insert */
static int cache_insert(city_info_t *city_info) {
    /* Insert a value into the cache here */
    /* You can use the apr_pool_t to allocate memory */
    apr_cache_entry_t *entry = apr_palloc(cache_pool, sizeof(apr_cache_entry_t));
    entry->city_info = city_info;
    entry->next = cache_head;
    cache_head = entry;

    return CACHE_SUCCESS;

}

/* Clean up the cache and free any allocated memory here */
static void cache_cleanup() {
    if (!cache_initialized)
        return;
    /* destroy the entire cache pool */
    apr_pool_destroy(cache_pool);
}

/**
 * Finds the city information in the cache.
 *
 * @param city_name The city name to search for it, case insensitive.
 *                  If NULL, lat and lng should be used.
 * @param lat The latitude to search for.
 * @param lng The longitude to search for.
 * @return The temperature of the city if found, otherwise TEMPERATURE_NOT_CACHED.
 */
float find_city_info_cached(const char* city_name, float lat, float lng) {

    /* Find the city information in the cache startting from cache_head */
    apr_cache_entry_t *entry = cache_head;
    while (entry != NULL) {

        if ((entry->city_info->city_name != NULL) && (apr_strnatcasecmp(entry->city_info->city_name, city_name) == 0))
            return entry->city_info->temperature;
        else if ((entry->city_info->city_name == NULL) && (geocode_compare(entry->city_info->lat, lat, entry->city_info->lng, lng)))
            return entry->city_info->temperature;

        entry = entry->next;
    }
    return TEMPERATURE_NOT_CACHED;
}

/**
 * caches the city information to save time in requesting the temperature again.
 *
 * @param city_name The city name.
 * @param lat The latitude.
 * @param lng The longitude.
 * @param temperature of the city/location.
 * @return 0 if the city information is cached successfully.
 */
int cache_city_info(const char* city_name, float lat, float lng, float temperature) {
    
    city_info_t *city_info = apr_palloc(cache_pool, sizeof(city_info_t));
    if (city_info == NULL)
        return CACHE_FAILURE;

/* if the city name is valid, ignore lat and lng and store the city info with only name */
    city_info->city_name = apr_pstrdup(cache_pool, city_name);
    if (city_info->city_name == NULL) {
        // if the city name is not valid, store the lat and lng
        city_info->lat = lat;
        city_info->lng = lng;
    }
    city_info->temperature = temperature;
    return cache_insert(city_info);
}

/**
 * Invalidates the cached information each 1 hour.
 * this function should be run in a separate thread in the background.
 * it should clean up the cache and remove the old entries as the tempreture 
 * may change each hour.
 *
 * @return 0 after it exists successfully.
 */
void* init_cache_invalidate_cache_thread(void *)
{

    do
    {
        cache_init();
        sleep(3600);
        cache_cleanup();
    } while (1);
    return 0;
}