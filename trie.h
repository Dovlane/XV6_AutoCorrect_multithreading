#define LETTERS 26
#define MAX_WORD_LEN 64

typedef struct trie_node //cvor unutar trie strukture
{
	char c; //slovo ovog cvora
	int term; //flag za kraj reci
	int subwords; //broj reci u podstablu, ne racunajuci sebe
	struct trie_node *parent; //pokazivac ka roditelju
	struct trie_node *children[LETTERS]; //deca
} trie_node;

void trie_add_word(char *word);
int find_subwords(char* prefix);
void free_trie();
void trie_init();