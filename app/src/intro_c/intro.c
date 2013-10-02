#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <log4c.h>

#include "intro_c/util.h"

static log4c_category_t *prac1;

void greeting(const char *greet_path, const char *name, char **greet_buf) {
    FILE *fIn;
    char *str = "(Missing greeting) ???, ", buf[80];
    
    *greet_buf = (char*)calloc(strlen(buf) + strlen(name) + 1, sizeof(char));
    strncpy(*greet_buf, str, 1 + strlen(str));
    strncat(*greet_buf, name, 1 + strlen(name));
    
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "greeting()");
    
    if (NULL == (fIn = fopen(greet_path, "r"))) {
        perror(greet_path);
        //exit(EXIT_FAILURE);
        return;
	}
    get_line(buf, sizeof(buf), fIn, false);  
    
    strncpy(*greet_buf, buf, 1 + strlen(buf));
    strncat(*greet_buf, name, 1 + strlen(name));
    
    fclose(fIn);
}

char delay_char(float microsecs) {
    char ch = '\0';
    float secs = microsecs / (float)(1.0e6); 
    float secs_floor = floorf(secs), nsecs = (secs - floorf(secs)) * (1.0e9);
    struct timespec time_delay = {.tv_sec = (int)secs_floor, 
        .tv_nsec = (long)nsecs};
    
    while (true) {
        nanosleep(&time_delay, NULL);    // usleep(microsecs);
        puts("Type any character when ready."); // fputs("...", stdout);
        ch = fgetc(stdin);  // scanf("%c", &ch);
        
        if ('\n' != ch && '\0' != ch)
            break;
    }
    return ch;
}
