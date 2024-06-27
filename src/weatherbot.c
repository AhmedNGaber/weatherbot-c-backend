#include <apr_file_io.h>
#include "weatherbot.h"


int main(int argc, char *argv[]) {

    apr_status_t apr_ret;
    apr_pool_t *weatherbot_pool;
    apr_file_t *file;
    apr_size_t len= TOKEN_MAX_SIZE;
    char errbuf[128];
    char token[TOKEN_MAX_SIZE];
    int ret;

    apr_ret = apr_initialize();
    apr_ret = apr_pool_create_core(&weatherbot_pool);

    apr_ret = apr_file_open(&file, ".token", APR_READ, APR_OS_DEFAULT, weatherbot_pool);
    if (apr_ret != APR_SUCCESS){
        DEBUG("Error opening .token file\n");
        goto apr_error;
    }

    apr_ret = apr_file_read(file, token, &len);
    if (apr_ret != APR_SUCCESS){
        DEBUG("Error reading token file\n");
        goto apr_error;
    }

    apr_ret = apr_file_close(file);
    if (apr_ret != APR_SUCCESS) {
        DEBUG("Error closing token file\n");
        goto apr_error;
    }

    if (strlen(token) == 0 || len == 0){
        DEBUG("Token file is empty or invalid\n");
        goto apr_error;
    }

    token[strlen(token)-1] = '\0';

    DEBUG_PRINT("Welcome to weathocastbot\n");
    DEBUG_PRINT("Token: %s\n", token);

    /* Initialize the used cache for this locations info and the associated tempereture */
    cache_init();

    ret = run_weatherbot(token);
    if (ret != 0)
        DEBUG("Error running weatherbot\n");

    goto exit;

apr_error:
    apr_strerror(apr_ret, errbuf, sizeof(errbuf));
    fprintf(stderr,"apr: %s\n", errbuf);

exit:
    apr_pool_destroy(weatherbot_pool);
    apr_terminate();
    return apr_ret;
}
