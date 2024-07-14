#include "../include/utils.h"

#include <stdlib.h>
#include <string.h>

// returns 0 only if successful
int str_to_int(char* s, int* dest)
{  
    char* endptr;
    int val = (int)strtol(s, &endptr, 10);
    if (*endptr) return 1;
    *dest = val;
    return 0;
}

void remove_newline(char* s)
{
    if ('\n' == s[strlen(s) - 1]) 
        s[strlen(s) - 1] = '\0';
}
