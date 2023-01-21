#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include "../mutex/mutex.h"

static lazy_mutex_controller_t logs_mutex;

void lazy_error(const char * message, const char * function , long long line){
    char * log = NULL;
    asprintf(&log,
             "error what: %s where: %s line: %lld",
             message,
             function,
             line);
    lazy_mutex_lock(&logs_mutex);
    perror(log);
    lazy_mutex_unlock(&logs_mutex);
    free(log);
}