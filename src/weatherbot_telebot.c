#include <telebot.h>
#include <apr_time.h>
#include <apr_strings.h>
#include "weatherbot_rust_wrapper.h"
#include "weatherbot.h"

int run_weatherbot(char* token) {
    char str[4096];
    bool cache_hit = false;
    float lat, lng, temperature;
    int index, count, ret, offset = -1;
    telebot_user_t me;
    telebot_handler_t handle;
    telebot_error_e telebot_ret;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};


    if (telebot_create(&handle, token) != TELEBOT_ERROR_NONE)
    {
        DEBUG_PRINT("Telebot create failed\n");
        return -1;
    }


    if (telebot_get_me(handle, &me) != TELEBOT_ERROR_NONE)
    {
        DEBUG_PRINT("Failed to get bot information\n");
        telebot_destroy(handle);
        return -1;
    }

    DEBUG_PRINT("ID: %d\n", me.id);
    DEBUG_PRINT("First Name: %s\n", me.first_name);
    DEBUG_PRINT("User Name: %s\n", me.username);

    telebot_put_me(&me);

    while (1)
    {
        telebot_update_t *updates;
        telebot_ret = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        if (telebot_ret != TELEBOT_ERROR_NONE)
            continue;
        DEBUG_PRINT("Number of updates: %d\n", count);
        for (index = 0; index < count; index++)
        {
            message = updates[index].message;
            if (message.text)
            {
                DEBUG_PRINT("%s: %s \n", message.from->first_name, message.text);

                if (apr_strnatcmp(message.text, "/start") == 0)
                {
                    apr_snprintf(str, SIZE_OF_ARRAY(str), "Hello %s, Nice to see you here\n"
                                                      "if you want to know current temperature\n"
                                                      "of any city, just send the city name\n"
                                                      "or the Latitude and longitude in this format \"12.34500:34.56700\"\n",
                             message.from->first_name);
                }
                else
                {
                    /* Check first if the requeste location was already cached */
                    cache_hit = find_city_info_cached(message.text, &temperature);
                    if (cache_hit)
                    {
                        apr_snprintf(str, SIZE_OF_ARRAY(str), "Current temperature for <b>%s</b> is: <b>%2.1lf</b>°C", message.text, temperature);
                        telebot_ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                        if (telebot_ret != TELEBOT_ERROR_NONE)
                        {
                            DEBUG_PRINT("Failed to send message: %d \n", ret);
                        }
                        offset = updates[index].update_id + 1;
                        continue;
                    }

                    ret = city_info_parser(message.text, &lat, &lng);
                    switch (ret) {
                        case INVALID_PARAM:
                            DEBUG_PRINT("Invalid parameters for parsing city info!\n");
                            break;
                        case INVALID_GEOCODE:
                            apr_snprintf(str, SIZE_OF_ARRAY(str), "Sorry! <b>%s</b> is not a valid Latitude and Longitude!\n"
                                                              "please try again with format 12.3456:67.89100",
                                     message.text);
                            break;
                        case VALID_GEOCODE:
                            temperature = get_city_temperature_by_geometry(lat, lng);
                            cache_city_info(message.text, temperature);
                            apr_snprintf(str, SIZE_OF_ARRAY(str), "Current temperature for <b>%f:%f:</b> is: <b>%2.1lf</b>°C", lat, lng, temperature);
                            break;
                        case VALID_STRING:
                            ret = is_city_name_valid(message.text);
                            if (ret) {
                                apr_snprintf(str, SIZE_OF_ARRAY(str), "Sorry! <b>%s</b> is not a valid city name!", message.text);
                            } else {
                                temperature = get_city_temperature_by_name(message.text);
                                apr_snprintf(str, SIZE_OF_ARRAY(str), "Current temperature for <b>%s</b> is: <b>%2.1lf</b>°C", message.text, temperature);
                                cache_city_info(message.text, temperature);
                            }
                            break;
                        default:
                            break;
                    }
                }

                telebot_ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");

                if (telebot_ret != TELEBOT_ERROR_NONE)
                {
                    DEBUG_PRINT("Failed to send message: %d \n", ret);
                }
            }
            offset = updates[index].update_id + 1;
            // prevent requesting to many city names as there is a limit on the number of requests 1 per second
            apr_sleep(1000000);
        }
        telebot_put_updates(updates, count);
    }
    telebot_destroy(handle);
}