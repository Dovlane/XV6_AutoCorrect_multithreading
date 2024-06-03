#include "trie.h"
#include "scanner.h"

typedef struct scanned_file 
{
	char file_name[256]; 
	time_t mod_time; 
    int flag;
} scanned_file;

scanned_file scanned_files[MAX_FILES_NUM];

int issep(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void reading_file(FILE* curr_file) {
    char c;
    int flag = 0;
    int index = 0;
    int ignore_counter = 0;
    char word[MAX_WORD_LEN];
    memset(word, 0, MAX_WORD_LEN);
    
    while ((c = fgetc(curr_file)) != EOF)
    {
        switch (flag)
        {

        case 0:
            if (isalpha(c)) {
                flag = 1;
                //printf("char %c\n", c);
            }
            else if (issep(c)){
                //printf("tab\n");
                break;
            }
            else {
                flag = 2;
                ignore_counter = MAX_WORD_LEN - 1;
                break;
            }
        case 1:
            if (index == MAX_WORD_LEN - 1 || issep(c)) {
                memset(word + index, 0, MAX_WORD_LEN - index);
                if (index > 0) {
                    int res = trie_add_word(word);
                    //printf("%s %d %d \n", word, index, res);
                }
                memset(word, 0, MAX_WORD_LEN);
                flag = 0;
                index = 0;
            }
            else if (isalpha(c)) {
                if (isupper(c)) {
                    c = c - 'A' + 'a';
                }
                word[index++] = c;
            }
            else {
                //printf("propala rec %s\n", word);
                memset(word, 0, MAX_WORD_LEN);
                ignore_counter = MAX_WORD_LEN - 1 - index;
                index = 0;
                flag = 2;
            }
            break;
        case 2:
            if (issep(c)) {
                flag = 0;
                ignore_counter = 0;
                break;
            }
            ignore_counter--;
            if (ignore_counter == 0) {
                flag = 0;
            }
            break;
        default:
            break;
        }
    }
    if (word[0] != '\0') {
        int res = trie_add_word(word);
        //printf("%s %d %d \n", word, index, res);
    }
}

void* scan(void* scn_args) {
    
    DIR* FD;
    struct dirent* in_file;
    FILE *curr_file;

    char* dirname = ((struct scanner_args*)scn_args)->directory;
    
    int index = 0;

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
        snprintf(filepath, MAX_DIRNAME, "%s/%s", dirname, in_file->d_name);

        struct stat file_stat;
        stat(filepath, &file_stat);
        
        if (S_ISREG(file_stat.st_mode)) {
            curr_file = fopen(filepath, "r");
            
            memmove(scanned_files[index].file_name, filepath, MAX_DIRNAME);
            scanned_files[index].mod_time = file_stat.st_mtime;
            scanned_files[index].flag = 1;
            index++;

            if (curr_file == NULL)
            {
                fprintf(stderr, "Error : Failed to open curr file - %s\n", strerror(errno));
                return NULL;
            }

            reading_file(curr_file);
            
            printf("|\r");

            fclose(curr_file);
        }
    }
    
    closedir(FD);

    while (1) {
        if (NULL == (FD = opendir (dirname))) 
        {
            fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
            return NULL;
        }
        
        index = 0;

        while ((in_file = readdir(FD))) 
        {
            if (!strcmp (in_file->d_name, "."))
                continue;
            if (!strcmp (in_file->d_name, ".."))    
                continue;
          
            char filepath[MAX_DIRNAME];
            memset(filepath, 0, MAX_DIRNAME);
            snprintf(filepath, MAX_DIRNAME, "%s/%s", dirname, in_file->d_name);

            struct stat file_stat;
            stat(filepath, &file_stat);

            if (!S_ISREG(file_stat.st_mode)) {
                continue;
            }

            struct scanned_file* sf;

            int read_file = 0;
            int new_file = 1;
            for (sf = scanned_files; sf->flag == 1; sf++) {
                if (strncmp(sf->file_name, filepath, MAX_DIRNAME) == 0) {
                    if (file_stat.st_mtime > sf->mod_time) {
                        read_file = 1;
                    }
                    new_file = 0;
                    sf->mod_time = file_stat.st_mtime;
                    break;
                }
            }

            if (new_file) {
                memmove(sf->file_name, filepath, MAX_DIRNAME);
                sf->mod_time = file_stat.st_mtime;
                sf->flag = 1;
                read_file = 1;
            }

            if (read_file) {
                curr_file = fopen(filepath, "r");
                if (curr_file == NULL)
                {
                    fprintf(stderr, "Error : Failed to open curr file - %s\n", strerror(errno));
                    return NULL;
                }
                reading_file(curr_file);
                printf("\r");
                fclose(curr_file);
            }
        }
        closedir(FD);

        sleep(5);
    }
    return NULL;
}