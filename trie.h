#define LETTERS 26

typedef struct trie_node //cvor unutar trie strukture
{
	char c; //slovo ovog cvora
	int term; //flag za kraj reci
	int subwords; //broj reci u podstablu, ne racunajuci sebe
	struct trie_node *parent; //pokazivac ka roditelju
	struct trie_node *children[LETTERS]; //deca
} trie_node;
