#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "scanner.h"

#define MAX_THREADS 20

struct scanner_args scn_args[MAX_THREADS];

void word_from_line(char* line, int size, int start, int* end, int* last_word) {
    int i;
    *last_word = 0;
    for (i = start; i < size; i++) {
        char c = line[i];
        if (c == '\n') {
            *last_word = 1;
            break;
        } 
        if (c == ' ' || c == '\t') {
            break;
        }
    }
    *end = i;
    if (*end == size)
        *last_word = 1;
}

int
main()
{
    printf("izvrsio se main\n"); 
    pthread_t scanner_thread[MAX_THREADS];
    int index = 0;
    
    while (1) {
        char command[MAX_WORD_LEN];
        char prev[MAX_WORD_LEN];
        char curr[MAX_WORD_LEN];
        memset(prev, 0, MAX_WORD_LEN);
        memset(curr, 0, MAX_WORD_LEN);

        printf("please enter command\n");
        
        int word_counter = 0;
        if (fgets(command, sizeof(command), stdin) != NULL) {
            int last_word = 0;
            int start = 0;
            int end;
            
            while (!last_word) {
                memmove(prev, curr, MAX_WORD_LEN);
                word_from_line(command, MAX_WORD_LEN, start, &end, &last_word);
                memset(curr, 0, MAX_WORD_LEN);
                memmove(curr, command + start, end - start);
                word_counter++;
                start = end;
                while (start < MAX_WORD_LEN && command[start] == ' ') {
                    start++;
                }
            }
        }
        else {
            printf("Error readin line\n");
        }

        if (word_counter == 2 && strncmp(prev, "_add_\0", 6) == 0) {
            memset(scn_args[index].directory, 0, MAX_DIRNAME);
            memmove(scn_args[index].directory, curr, MAX_WORD_LEN);
            struct scanner_args* sa;
            int already_has_thread = 0;
            for (sa = scn_args; sa < scn_args + index; sa++) {
                if (strncmp(sa->directory, scn_args[index].directory, MAX_DIRNAME) == 0) {
                    printf("Directory has already thread on it\n");
                    already_has_thread = 1;
                }
            }
            if (already_has_thread)
                continue;
            pthread_create(&scanner_thread[index], NULL, &scan, (void*) &scn_args[index].directory);

            index++;
        }
        else if (strncmp(command, "_stop_\n", 7) == 0) {
            printf("_stop_ -> main thread is destroyed\n");
            break;
        }
        else {
            //printf("prefix = %s\n", curr);
            char c;
            while ((c = getchar()) != EOF) {
                
            }
            clearerr(stdin);
        }
    }

    return 0;
}