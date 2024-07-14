#ifndef FIFO_DISPATCHER_UTILS_H
#define FIFO_DISPATCHER_UTILS_H

#define MAX_INT_STR_SZ (11)

// returns 0 only if successful
int str_to_int(char* s, int* dest);

// removes trailing newline if it exists
void remove_newline(char* s);

#endif //FIFO_DISPATCHER_UTILS_H
