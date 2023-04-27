#include "header.h"

void init_NRLL(huffman_NRLL *NRLL)
{
    NRLL->front = NULL;
    NRLL->size = 0;
}

bool is_NRLL_empty(huffman_NRLL NRLL)
{
    return NRLL.size = 0;
}

void input_node_front(huffman_NRLL *NRLL, huffman_node_t *new_node)
{
    NRLL->front = new_node;
    new_node->next = NULL;
}

void input_node_middle(huffman_NRLL *NRLL, huffman_node_t *new_node, huffman_node_t *help_pointer)
{
    new_node->next = help_pointer->next;
    help_pointer->next = new_node;
}

void input_node_rear(huffman_NRLL *NRLL, huffman_node_t *new_node, huffman_node_t *help_pointer)
{
    new_node->next = NULL;
    help_pointer->next = new_node;
}

void input_node(huffman_NRLL *NRLL, huffman_node_t *new_node)
{
    if (is_NRLL_empty(*NRLL))
    {
        input_node_front(NRLL, new_node);
    }
    else
    {
        if (NRLL->front->frequency >= new_node->frequency)
        {
            new_node->next = NRLL->front;
            NRLL->front = new_node;
        }
        else
        {
            huffman_node_t *help_pointer;
            help_pointer = NRLL->front;

            while (help_pointer->next != NULL && help_pointer->next->frequency < new_node->frequency)
            {
                help_pointer = help_pointer->next;
            }

            if (help_pointer->next == NULL)
            {
                input_node_rear(NRLL, new_node, help_pointer);
            }
            else
            {
                input_node_middle(NRLL, new_node, help_pointer);
            }
        }
    }
    NRLL->size++;
}

huffman_node_t *delete_node(huffman_NRLL *NRLL)
{
    if (is_NRLL_empty(*NRLL))
    {
        printf("Antrian kosong");
        exit(EXIT_FAILURE);
    }

    huffman_node_t *delete_node = NRLL->front;
    NRLL->front = NRLL->front->next;
    NRLL->size--;
    delete_node->next = NULL;
    return delete_node;
}
