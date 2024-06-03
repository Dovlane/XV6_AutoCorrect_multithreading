#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

trie_node* trie_root;
pthread_mutex_t trie_lock[LETTERS];
char prefix[MAX_WORD_LEN];

extern search_result auto_result;

static trie_node* create_tree_node(trie_node* parent, char c) {
    trie_node* new_node;
    new_node = calloc(1, sizeof(trie_node));
    new_node->c = c;
    new_node->term = 0;
    new_node->subwords = 0;
    new_node->parent = parent;
    for (int i = 0; i < LETTERS; i++) {
        new_node->children[i] = 0;
    }

    return new_node;
}

void trie_init() {
    trie_root = create_tree_node(0, '1');
    for (int i = 0; i < LETTERS; i++) {
        pthread_mutex_init(&trie_lock[i], NULL);
    }
}

static int tp_add_word(trie_node* tp, char* word) {
    trie_node *newtp = tp->children[*word - 'a'];

    if (newtp == 0) {
        newtp = create_tree_node(tp, *word);
        tp->children[*word - 'a'] = newtp;    
    }
    word++;
    if (*word == '\0') {
        int res = 0;
        if (newtp->term == 0) {
            newtp->term = 1;
            res = 1;
            trie_node* tmp = tp;
            while (tmp != 0) {
                tmp->subwords++;
                tmp = tmp->parent;
            }
        }
        return res;
    }

    return tp_add_word(newtp, word);
}

int trie_add_word(char *word) {
    pthread_mutex_lock(&trie_lock[*word - 'a']);
    int res = tp_add_word(trie_root, word);
    pthread_mutex_unlock(&trie_lock[*word - 'a']);
    return res;
}

static trie_node* find_root(trie_node* tp, char* prefix) {
    if (tp == 0 || *prefix == '\0')
        return tp;
    
    tp = tp->children[*prefix - 'a'];
    prefix++;
    return find_root(tp, prefix); 
}

char traversed_word[MAX_WORD_LEN];
int traverse_counter = 0;
char** words;

static void traverse_words(trie_node* tp, int pos) {
    if (tp->term == 1) {
        memmove(auto_result.words[traverse_counter++], traversed_word, MAX_WORD_LEN);
    }
    
    pos++;

    for (int i = 0; i < LETTERS; i++) {
        if (tp->children[i] != 0) {
            traversed_word[pos] = 'a' + i;
            traverse_words(tp->children[i], pos);
            traversed_word[pos] = '\0';
        }
    }
}

int get_words(char* prefix) {
    pthread_mutex_lock(&trie_lock[*prefix - 'a']);
    trie_node* tp = find_root(trie_root, prefix);
    if (tp == 0)
        return -1;
    int n = strlen(prefix);
    
    auto_result.result_count = tp->subwords + tp->term;
    auto_result.words = calloc(auto_result.result_count, sizeof(char*));
    for (int i = 0; i < auto_result.result_count; i++)
        auto_result.words[i] = calloc(MAX_WORD_LEN, sizeof(char));

    memmove(traversed_word, prefix, n);
    traverse_words(tp, n - 1);
    pthread_mutex_unlock(&trie_lock[*prefix - 'a']);
    traverse_counter = 0;
    memset(traversed_word, 0, MAX_WORD_LEN);
    printf("tp->subwords = %d, tp->term = %d \n", tp->subwords, tp->term);
    return auto_result.result_count;
}



static void free_trie_node(trie_node* tp) {
    tp->parent = 0;
    for (int i = 0; i < LETTERS; i++) {
        if (tp->children[i] != 0) {
            free_trie_node(tp->children[i]);
        }
    }
    free(tp);
}

void free_trie() {
    free_trie_node(trie_root);
}

