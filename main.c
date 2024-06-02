#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "scanner.h"

#define MAX_THREADS 20

struct scanner_args scn_args[MAX_THREADS];

int
main()
{
    printf("izvrsio se main\n"); 
    pthread_t scanner_thread[MAX_THREADS];
    int index = 0;
    
    while (1) {
        char command[10];
        printf("please enter command\n");
        scanf("%s", command);
        if (strcmp(command, "_add_") == 0) {
            memset(scn_args[index].directory, 0, MAX_DIRNAME);
            scanf("%s", scn_args[index].directory);
            struct scanner_args* sa;
            int alreadyHasThread = 0;
            for (sa = scn_args; sa < scn_args + index; sa++) {
                if (strncmp(sa->directory, scn_args[index].directory, MAX_DIRNAME) == 0) {
                    printf("Directory has already thread on it\n");
                    alreadyHasThread = 1;
                }
            }
            if (alreadyHasThread)
                continue;
            pthread_create(&scanner_thread[index], NULL, &scan, (void*) &scn_args[index].directory);

            index++;
        }
        else if (strcmp(command, "_stop_") == 0) {
            printf("_stop_ -> main thread is destroyed\n");
            break;
        }
    }

    return 0;
}