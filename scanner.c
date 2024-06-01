#include "scanner.h"

void* scan(void* dirname) {
    
    DIR* FD;
    struct dirent* in_file;
    FILE    *curr_file;

    if (NULL == (FD = opendir (dirname))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        return NULL;
    }


    while ((in_file = readdir(FD))) 
    {
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;

        
        
        char filepath[MAX_DIRNAME];
        memset(filepath, 0, MAX_DIRNAME);
        snprintf(filepath, MAX_DIRNAME, "%s/%s", (char*)dirname, in_file->d_name);

        curr_file = fopen(filepath, "r");

        if (curr_file == NULL)
        {
            fprintf(stderr, "Error : Failed to open curr file - %s\n", strerror(errno));
            return NULL;
        }

        char word[MAX_WORD_LEN];
        while (fgets(word, MAX_WORD_LEN, curr_file) != NULL)
        {
            printf("%s ", word);
        }
        printf("\r");

        fclose(curr_file);
    }
    closedir(FD);
    return NULL;
}