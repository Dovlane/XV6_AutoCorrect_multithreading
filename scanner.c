#include "scanner.h"

void* scan(void* file) {
    printf("file = %s\n", (char*) file);
    return NULL;
}