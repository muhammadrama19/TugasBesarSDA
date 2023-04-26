#include"header.h"
void print_code(codeblocks *code)
{
    
    size_t n = code->code_length;
    while (n-- > 0)
    {
        putchar('0' + ((code->bit_code >> n) & 1));
    }

}

void print_code_table(codeblocks *table)
{
    printf("\nTabel Konversi\n");
    int i;
    for (i = 0; i < MAX_ASCII_CHARACTERS; ++i)
    {
        if (table[i].code_length)
        {
            printf("\'%c\'\t", i);
            code_print(table + i);
            printf("\n");
        }
    }
}
void print_hierarchy(huffman_node_t *current, int level)
{
    if (current == NULL) {
        return;
    }

    if (current->right != NULL) {
        printHierarchy(current->right, level + 1);
        printf("%*s|\n", level * 4, "");
    }

    printf("%*s", level * 4, "");

    if (current->letter != '\0') {
        printf("(%d) '%c'\n", current->frequency, current->letter);
    } else {
        printf("(%d)\n", current->frequency);
    }

    if (current->left != NULL) {
        printf("%*s|\n", level * 4, "");
        printHierarchy(current->left, level + 1);
    }
}