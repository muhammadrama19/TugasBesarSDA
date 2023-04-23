#include"header.h"
#include"NRLL.c"


huffman_node_t* make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right)
{
    huffman_node_t *new_node = (huffman_node_t *)malloc(sizeof(huffman_node_t));

    if (new_node == NULL)
    {
        printf("Alokasi gagal...");
        return;
    }
    new_node->letter=letter;
    new_node->frequency=frequency;
    new_node->left=left;
    new_node->right=right;
    new_node->next=NULL; //NULL Karena sudah mulai membentuk tree
}



