#include <stdio.h>
#include <telebot.h>
#include <unistd.h>
#include <string.h>
#include "weatherbot_rust_wrapper.h"
#include "weatherbot.h"

int run_weatherbot(char* token) {
    char str[4096];
    float lat, lng, temperature;
    int index, count, ret, offset = -1;
    telebot_user_t me;
    telebot_handler_t handle;
    telebot_error_e telebot_ret;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};


    if (telebot_create(&handle, token) != TELEBOT_ERROR_NONE)
    {
        printf("Telebot create failed\n");
        return -1;
    }


    if (telebot_get_me(handle, &me) != TELEBOT_ERROR_NONE)
    {
        printf("Failed to get bot information\n");
        telebot_destroy(handle);
        return -1;
    }

    printf("ID: %d\n", me.id);
    printf("First Name: %s\n", me.first_name);
    printf("User Name: %s\n", me.username);

    telebot_put_me(&me);

    while (1)
    {
        telebot_update_t *updates;
        telebot_ret = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        if (telebot_ret != TELEBOT_ERROR_NONE)
            continue;
        printf("Number of updates: %d\n", count);
        for (index = 0; index < count; index++)
        {
            message = updates[index].message;
            if (message.text)
            {
                printf("%s: %s \n", message.from->first_name, message.text);

                if (strstr(message.text, "/start"))
                {
                    snprintf(str, SIZE_OF_ARRAY(str), "Hello %s, Nice to see you here\n"
                                                      "if you want to know current temperature\n"
                                                      "of any city, just send the city name\n"
                                                      "or the Latitude and longitude in this format \"12.34500:34.56700\"\n",
                             message.from->first_name);
                }
                else
                {
                    ret = city_info_parser(message.text, &lat, &lng);

                    switch (ret) {
                        case INVALID_PARAM:
                            printf("Invalid parameters for parsing city info!\n");
                            break;
                        case INVALID_GEOCODE:
                            snprintf(str, SIZE_OF_ARRAY(str), "Sorry! <b>%s</b> is not a valid Latitude and Longitude!\n"
                                                              "please try again with format 12.3456:67.89100",
                                     message.text);
                            break;
                        case VALID_GEOCODE:
                            temperature = get_city_temperature_by_geometry(lat, lng);
                            snprintf(str, SIZE_OF_ARRAY(str), "Current temperature for <b>%f:%f:</b> is: <b>%2.1lf</b>°C", lat, lng, temperature);
                            break;
                        case VALID_STRING:
                            ret = is_city_name_valid(message.text);
                            if (ret) {
                                snprintf(str, SIZE_OF_ARRAY(str), "Sorry! <b>%s</b> is not a valid city name!", message.text);
                            } else {
                                /* Check first if the requeste location was already cached */
                                ret = find_city_info_cached(message.text, lat, lng);
                                if (ret == TEMPERATURE_NOT_CACHED)
                                {
                                    temperature = get_city_temperature_by_name(message.text);
                                    cache_city_info(message.text, lat, lng, temperature);
                                } else {
                                    temperature = ret;
                                }
                                snprintf(str, SIZE_OF_ARRAY(str), "Current temperature for <b>%s</b> is: <b>%2.1lf</b>°C", message.text, temperature);
                            }
                            break;
                        default:
                            break;
                    }
                }
                telebot_ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");

                if (telebot_ret != TELEBOT_ERROR_NONE)
                {
                    printf("Failed to send message: %d \n", ret);
                }
            }
            offset = updates[index].update_id + 1;
            // prevent requesting to many city names as there is a limit on the number of requests 1 per second
            sleep(1);
        }
        telebot_put_updates(updates, count);
        sleep(1);
    }
    telebot_destroy(handle);
}