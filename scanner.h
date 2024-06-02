#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include "trie.h"


#define MAX_WORD_LEN 64
#define MAX_FILES_NUM 30
#define MAX_DIRNAME 256

void* scan(void* );

struct scanner_args {
    char directory[MAX_DIRNAME];
    trie_node* trie;
};