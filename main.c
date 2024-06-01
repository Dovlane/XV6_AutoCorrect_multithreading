#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    printf("izvrsio se main\n"); 
    while (1) {
        char* command;
        scanf("%s", command);
        if (strcmp(command, "_add_") == 0) {
            printf("I recognised add\n");
        }
        else if (strcmp(command, "_stop_") == 0) {
            printf("_stop_ -> main thread is destroyed\n");
            break;
        }
    }
    return 0;
}