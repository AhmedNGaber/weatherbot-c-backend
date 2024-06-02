#include <stdio.h>
#include <unistd.h>
#include <apr_file_io.h>
#include <telebot.h>
#include "weatherbot_rust_wrapper.h"

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))
#define TOKEN_MAX_SIZE 128

int main(int argc, char *argv[]) {

    apr_status_t apr_ret;
    apr_pool_t *pool;
    apr_file_t *file;
    apr_size_t len= TOKEN_MAX_SIZE;
    char errbuf[128];
    char token[TOKEN_MAX_SIZE];

    apr_ret = apr_initialize();
    if( apr_ret != APR_SUCCESS ){
        fprintf(stderr, "Error initializing APR\n");
        goto apr_error;
    }

    apr_ret = apr_pool_create(&pool, NULL);
    if (apr_ret != APR_SUCCESS){
        fprintf(stderr, "Error creating memory pool\n");
        goto apr_error;
    }

    apr_ret = apr_file_open(&file, ".token", APR_READ, APR_OS_DEFAULT, pool);
    if (apr_ret != APR_SUCCESS){
        fprintf(stderr, "Error opening .token file\n");
        goto apr_error;
    }

    apr_ret = apr_file_read(file, token, &len);
    if (apr_ret != APR_SUCCESS){
        fprintf(stderr, "Error reading token file\n");
        goto apr_error;
    }

    apr_ret = apr_file_close(file);
    if (apr_ret != APR_SUCCESS) {
        fprintf(stderr, "Error closing token file\n");
        goto apr_error;
    }

    if (strlen(token) == 0 || len == 0){
        fprintf(stderr, "Token file is empty or invalid\n");
        goto apr_error;
    }

    token[strlen(token)-1] = '\0';

    printf("Welcome to weathocastbot\n");
    printf("Token: %s\n", token);

    //For Testing: Call telebot functions with token
    telebot_user_t me;
    telebot_handler_t handle;
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

    int index, count, offset = -1;
    telebot_error_e ret;
    telebot_message_t message;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};

    while (1)
    {
        telebot_update_t *updates;
        ret = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        if (ret != TELEBOT_ERROR_NONE)
            continue;
        printf("Number of updates: %d\n", count);
        for (index = 0; index < count; index++)
        {
            message = updates[index].message;
            if (message.text)
            {
                printf("%s: %s \n", message.from->first_name, message.text);
                if (strstr(message.text, "/dice"))
                {
                    telebot_send_dice(handle, message.chat->id, false, 0, "");
                }
                else
                {
                    char str[4096];
                    if (strstr(message.text, "/start"))
                    {
                        snprintf(str, SIZE_OF_ARRAY(str), "Hello %s, Nice to see you here\n"\
                                                          "if you want to know current temperature\n"\
                                                          "of any city, just send the city name\n"\
                                                          "or the Latitude and longitude in this format \"12.34500:34.56700\"\n"
                                                          , message.from->first_name);
                    }
                    else
                    {
                        int err;
                        err = is_city_name_valid(message.text);
                        if (err != 0)
                        {
                            snprintf(str, SIZE_OF_ARRAY(str), "Sorry! <b>%s</b> is not a valid city name!\n"\
                                                              "please try again", message.text);
                        }
                        else
                        {
                            double temperature;
                            temperature = get_city_temperature_by_name(message.text);
                            snprintf(str, SIZE_OF_ARRAY(str), "Current temprature for <b>%s</b> is: <b>%2.1lf</b>°C", message.text, temperature);
                        }
                    }
                    ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                }
                if (ret != TELEBOT_ERROR_NONE)
                {
                    printf("Failed to send message: %d \n", ret);
                }
            }
            offset = updates[index].update_id + 1;
        }
        telebot_put_updates(updates, count);

        sleep(1);
    }

    telebot_destroy(handle);

    goto exit;

apr_error:
    apr_strerror(apr_ret, errbuf, sizeof(errbuf));
    fprintf(stderr,"apr: %s\n", errbuf);

exit:
    apr_pool_destroy(pool);
    apr_terminate();
    return apr_ret;
}
