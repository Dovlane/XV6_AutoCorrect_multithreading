#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

trie_node* trie_root;

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
}

static void tp_add_word(trie_node* tp, char* word) {
    trie_node *newtp = tp->children[*word - 'a'];

    if (newtp == 0) {
        newtp = create_tree_node(tp, *word);
        tp->children[*word - 'a'] = newtp;    
    }
    word++;
    if (*word == '\0') {
        newtp->term = 1;
        return;
    }
    
    tp->subwords++;
    newtp->term = 0;    

    tp_add_word(newtp, word);
}

void trie_add_word(char *word) {
    tp_add_word(trie_root, word);
}

static trie_node* find_root(trie_node* tp, char* prefix) {
    if (tp == 0 || *prefix == '\0')
        return tp;
    
    tp = tp->children[*prefix - 'a'];
    prefix++;
    return find_root(tp, prefix); 
}

int find_subwords(char* prefix) {
    trie_node* tp = find_root(trie_root, prefix);
    if (tp == 0)
        return -1;
    
    printf("tp->subwords = %d, tp->term = %d \n", tp->subwords, tp->term);
    return tp->subwords + tp->term;
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

