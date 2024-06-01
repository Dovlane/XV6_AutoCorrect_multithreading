#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "scanner.h"

#define MAX_THREADS 20

int
main()
{
    printf("izvrsio se main\n"); 
    char directories[MAX_THREADS][50];
    pthread_t scanner_thread[MAX_THREADS];
    int index = 0;
    
    while (1) {
        char command[10];
        scanf("%s", command);
        if (strcmp(command, "_add_") == 0) {
            scanf("%s", directories[index]);
            
            pthread_create(&scanner_thread[index], NULL, &scan, (void*) &directories[index]);
            pthread_join(scanner_thread[index], NULL);

            index++;
        }
        else if (strcmp(command, "_stop_") == 0) {
            printf("_stop_ -> main thread is destroyed\n");
            break;
        }
    }

    return 0;
}