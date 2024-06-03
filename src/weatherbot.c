#include <stdio.h>
#include <unistd.h>
#include <apr_file_io.h>
#include "weatherbot.h"


int main(int argc, char *argv[]) {

    apr_status_t apr_ret;
    apr_pool_t *pool;
    apr_file_t *file;
    apr_size_t len= TOKEN_MAX_SIZE;
    char errbuf[128];
    char token[TOKEN_MAX_SIZE];
    int ret;

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

    ret = run_weatherbot(token);
    if (ret != 0)
        fprintf(stderr, "Error running weatherbot\n");

    goto exit;

apr_error:
    apr_strerror(apr_ret, errbuf, sizeof(errbuf));
    fprintf(stderr,"apr: %s\n", errbuf);

exit:
    apr_pool_destroy(pool);
    apr_terminate();
    return apr_ret;
}
