#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
    printf("izvrsio se main\n"); 
    char** directories;
    int size = 1;
    directories = malloc(sizeof(char* ));
    while (1) {
        char command[10];
        scanf("%s", command);
        if (strcmp(command, "_add_") == 0) {
            char directory[50];
            scanf("%s", directory);
            //printf("sizeof(%s) = %d\n", directory, (int)sizeof(directory));

            directories[size - 1] = calloc(sizeof(directory) + 1, sizeof(char));
            memmove(directories[size - 1], directory, sizeof(directory));
            directories = realloc(directories, (++size) * sizeof(char* ));
        }
        else if (strcmp(command, "_stop_") == 0) {
            printf("_stop_ -> main thread is destroyed\n");
            break;
        }
    }
    for (int i = 0; i < size; i++) {
        //printf("directories[%d] = %s\n", i,  directories[i]);
        free(directories[i]);
    }
    free(directories);
    return 0;
}