#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <apr_thread_proc.h>
#include "weatherbot.h"


typedef struct city_info_t{
    char *city_name;
    float temperature;
} city_info_t;

typedef struct apr_cache_entry_t {
struct apr_cache_entry_t * next;
city_info_t * city_info;
} apr_cache_entry_t;


/* Global variables */
static apr_pool_t *cache_pool = NULL;
static apr_pool_t *cache_nodes_pool = NULL;
static bool cache_initialized = false;
static apr_cache_entry_t *cache_head = NULL;
static apr_thread_mutex_t *cache_mutex = NULL;
static apr_thread_t *cache_clean_thread = NULL;


void* init_cache_invalidate_cache_thread(apr_thread_t *, void *);

void cache_init(void)
{
    /* Initialize the cache here */
    if (cache_initialized)
        return;

    apr_initialize();
    apr_pool_create_unmanaged(&cache_pool);

    apr_pool_create(&cache_nodes_pool, cache_pool);
    apr_thread_mutex_create(&cache_mutex, APR_THREAD_MUTEX_DEFAULT, cache_pool);
    /* run init_cache_invalidate_cache_thread in a thread */
    apr_thread_create(&cache_clean_thread, NULL, init_cache_invalidate_cache_thread, NULL, cache_pool);
   
    cache_initialized = true;
}

/* The passed pointer should already be allocated before using cache_insert */
static void cache_insert(city_info_t *city_info) {
    /* Insert a value into the cache here */
    apr_cache_entry_t *entry = apr_palloc(cache_nodes_pool, sizeof(apr_cache_entry_t));
    entry->city_info = city_info;
    entry->next = cache_head;
    cache_head = entry;
}

/* Clean up the cache and free any allocated memory here */
static void cache_cleanup() {
    if (!cache_initialized)
        return;
    /* destroy the entire cache pool, then create it from the beginning*/
    apr_pool_destroy(cache_nodes_pool);
    apr_pool_create(&cache_nodes_pool, cache_pool);
    cache_head = NULL;
}

/**
 * Finds the city information in the cache.
 *
 * @param city_name City name / location string to search for it, case insensitive.
 * @param temperature  output temperature of the city.
 * @return bool true if the city information is found in the cache.
 */
bool find_city_info_cached(const char* city_name, float* temperature) {

    /* Find the city information in the cache startting from cache_head */
    apr_cache_entry_t *entry = cache_head;

    printf("find_city_info_cached city_name: %s, \n", city_name);

    apr_thread_mutex_lock (cache_mutex);
    while (entry != NULL) {

        if ((entry->city_info->city_name != NULL) && (apr_strnatcasecmp(entry->city_info->city_name, city_name) == 0)) {
            *temperature = entry->city_info->temperature;
            apr_thread_mutex_unlock (cache_mutex);
            return true;
        }
        entry = entry->next;
    }
    apr_thread_mutex_unlock (cache_mutex);
    printf("find_city_info_cached not found\n");
    return false;
}

/**
 * caches the city information to save time in requesting the temperature again.
 *
 * @param city_name The city name.
 * @param temperature of the city/location.
 */
void cache_city_info(const char* city_name, float temperature) {
    apr_thread_mutex_lock (cache_mutex);
    city_info_t *city_info = apr_palloc(cache_nodes_pool, sizeof(city_info_t));
    city_info->city_name = apr_pstrdup(cache_nodes_pool, city_name);
    city_info->temperature = temperature;
    cache_insert(city_info);
    apr_thread_mutex_unlock (cache_mutex);
}

/**
 * Invalidates the cached information each 1 hour.
 * this function should be run in a separate thread in the background.
 * it should clean up the cache and remove the old entries as the tempreture 
 * may change each hour.
 *
 * @return 0 after it exists successfully.
 */
void* init_cache_invalidate_cache_thread(apr_thread_t * self, void * data)
{
    do
    {
        printf("init_cache_invalidate_cache_thread\n");
        /* invalidate the cache each 30 mins*/
        apr_sleep(1800000000);
        apr_thread_mutex_lock (cache_mutex);
        cache_cleanup();
        apr_thread_mutex_unlock (cache_mutex);
    } while (1);
    return 0;
}