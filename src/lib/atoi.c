#include <stdint.h>

int atoi(const char *nptr) {
    int num = 0;
    while (*nptr != '\0') 
        num = 10*num + (*nptr++ - '0'); 
    return num;
}

