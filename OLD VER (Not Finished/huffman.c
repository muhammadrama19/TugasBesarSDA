/*  
    ===========PROGRAM HUFFMAN CODING===============
    KELAS    : 1A 
    KELOMPOK : 1B
    ANGGOTA: : MUHAMMAD RAMA NURIMANI 221524021
               RAHMA DIVINA 221524026
               RAYHAN FANEZ FATHIADI 221524027
    =======================================================
*/


#include "header.h"
#include "NRLL.c"
#include"helper.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include<string.h>



huffman_node_t *make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right)
{
    huffman_node_t *new_node = (huffman_node_t *)malloc(sizeof(huffman_node_t));

    assert(new_node != NULL);

    // fill new node with info
    new_node->letter = letter;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right;
    new_node->next = NULL;

    return new_node;
}



bool is_leaf(huffman_node_t *node)
{
    return node->left == NULL && node->right == NULL;
}

void build_huffman(huffman_NRLL *NRLL)
{
    if (is_NRLL_empty(*NRLL))
    {
        printf("Gagal membuat forest...");
        exit(EXIT_FAILURE);
    }

    printf("\nMemulai proses pembentukan huffman tree...\n\n");
    if ((*NRLL).size == 1)
    {
        // hfndling untuk teks hanya dengan satu karakter
        print_NRLL(*NRLL);
    }
    else
    {
        int steps = 1;
        while ((*NRLL).size > 1)
        {
            huffman_node_t *smallest, *nd_smallest;

            // ambil dua node dengan frequency terkecil
            smallest = delete_node(NRLL);
            nd_smallest = delete_node(NRLL);

            // buat node dengan frequency gabungan dari keduanya
            // dengan anak smallest sebagai anak kiri dan nd_smallest anak kanan
            huffman_node_t *new_node = make_huffman_node(BLANK_CHARACTER, smallest->frequency + nd_smallest->frequency, smallest, nd_smallest);

            // masukkan kembali ke NRLL untuk diurutkan
            input_node(NRLL, new_node);
            printf("Step %d:\t", steps++);
            print_NRLL(*NRLL);
        }
    }
}

huffman_node_t *create_huffman(int frequency_map[MAX_ASCII_CHARACTER])
{
    int i;
    huffman_NRLL forest;
    init_NRLL(&forest);

    for (i = 0; i < MAX_ASCII_CHARACTER; i++)
    {
        if (frequency_map[i])
        {
            huffman_node_t *new_node = make_huffman_node(i, frequency_map[i], NULL, NULL);
            input_node(&forest, new_node);
        }
    }
    system("cls");
    printf("Kondisi awal setiap node dalam NRLL\n");
    print_NRLL(forest);

    // buat huffman tree
    build_huffman(&forest);
    printf("\nHierarki Huffman Tree\n");
    print_hierarchy(forest.front, 0);
    return forest.front;
}


void create_code(huffman_node_t *node, codewords *table, codewords code)
{
    if (is_leaf(node))
    {
        table[node->letter] = code;
    }
    else
    {
        code.code_length++;
        code.bit_code <<= 1;
        create_code(node->left, table, code);
        code.bit_code += 1;
        create_code(node->right, table, code);
    }
}

void decode_string(huffman_node_t *root)
{
    FILE *file_to_read = fopen("encodedString.txt", "r");

    assert(file_to_read != NULL);

    huffman_node_t *current = root;
    char letter;
    while ((letter = fgetc(file_to_read)) != EOF)
    {
        if (letter == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        if (is_leaf(current))
        {
            printf("%c", current->letter);
            current = root;
        }
    }
}


void read_via_char()
{
    system("cls");
    getchar(); // pembuang karakter enter

    int i, sum_of_character, frequency_map[MAX_ASCII_CHARACTER] = {0};
    char *input_string;

    // meminta inputan jumlah karakter yang akan dikompresi
    printf("Masukkan banyak karakter yang akan encoding : ");
    scanf("%d", &sum_of_character);
    printf("\n");

    // membaca karakter dan frekuensinya
    for (i = 0; i < sum_of_character; )
    {
        char letter;
        int frequency=0;
        printf("Karakter ke - %d\n", i + 1);
        printf("Masukkan karakter\t: ");
        scanf(" %c", &letter);

        while ((int)letter < 0 || (int)letter > 127) {
            printf("Karakter harus di antara ASCII 0-127. Silakan masukkan kembali: ");
            scanf(" %c", &letter);
        }

        // check if the letter is already in the input string
        int j;
        for (j = 0; j < i; j++) {
            if (input_string[j] == letter) {
                printf("Karakter %c telah dimasukkan sebelumnya.\n", letter);
                printf("Masukkan frekuensi tambahan : ");
                scanf("%d", &frequency);
                while (frequency <= 0) {
                    printf("Frekuensi harus lebih besar dari 0. Silakan masukkan kembali: ");
                    scanf("%d", &frequency);
                }
                frequency_map[letter] += frequency;
                break; // break out of the loop
            }
        }

        // if the letter is not in the input string, add it
        if (j == i) {
            printf("Masukkan frekuensi (lebih besar dari 0) : ");
            scanf("%d", &frequency);
            while (frequency <= 0) {
                printf("Frekuensi harus lebih besar dari 0. Silakan masukkan kembali: ");
                scanf("%d", &frequency);
            }
            input_string[i] = letter;
            frequency_map[letter] = frequency;
            i++;
        }

        printf("\n");
    }

    codewords code = {0};
    codewords table[MAX_ASCII_CHARACTER] = {0};
    huffman_node_t *root = create_huffman(frequency_map);
    create_code(root, table, code);
    print_code_table(table);

    const char *string_new = (const char *)input_string;
    const char *for_history = (const char *)input_string;

    printf("\nHasil Encode:");
    while (*string_new)
    {
        int letter = (int)*string_new++;
        code_print(table + letter);
        write_code_to_file(table + letter);
        write_code_to_file_hasil(table + letter);
    }
        printf("\nString setelah decode\n");
        decode_string(root);
        printf("\n");
        save_history(for_history, &code);
        fclose(fopen("encodedString.txt", "w"));
        fclose(fopen("hasil.txt", "w"));
        destroy_tree(root);
}

void read_via_string()
{
    system("cls");
    getchar(); // pembuang karakter enter

    // insialisasi
    printf("Masukan kalimat: ");
    char *sentence = read_dynamic();

    // kalimat diubah dari array menjadi array constant
    const char *string = (const char *)sentence;
    const char *to_encode_string = (const char *)sentence;

    // jumlah frekuensi ditaruh ke dalam array, dengan indeks
    // array sebagai representasi karakter yang dihitung
    int frequency_map[MAX_ASCII_CHARACTER] = {0};
    while (*string)
    {
        frequency_map[(int)*string++]++;
    }

    // check if the input string has only one character
    int unique_chars = 0;
    for (int i = 0; i < MAX_ASCII_CHARACTER; i++)
    {
        if (frequency_map[i] > 0)
        {
            unique_chars++;
        }
        if (unique_chars > 1)
        {
            break;
        }
    }

    if (unique_chars == 1 || sentence[1] == '\0')
    {
        printf("tidak bisa membuat tree dengan satu jenis karakter\n");
        free(sentence);
        return;
    }

    // membuat huffman tree, mengembalikan root untuk proses decode nantinya
    codewords code = {0};
    codewords table[MAX_ASCII_CHARACTER] = {0};
    huffman_node_t *root = create_huffman(frequency_map);

    // make code
    create_code(root, table, code);
    print_code_table(table);

    // menampilkan hasil kompresi dari stirng yang telah diterjemahkan
    // hasil kompresi ditaruh ke dalam encodedString.txt
    // disimpan ke dua txt, txt untuk proses decode nantinya dan untuk
    printf("\nString setelah diencode\n");
    while (*to_encode_string)
    {
        int letter = (int)*to_encode_string++;
        code_print(table + letter);
        write_code_to_file(table + letter);
        write_code_to_file_hasil(table + letter);
    }
    printf("\n");

    // menampilkan string setelah dikembalikan dari file teks encodedString.txt
    printf("\nString setelah decode\n");
    decode_string(root);
    printf("\n");

    // membersihkan file encodedString.txt agar bisa dipakai ullang tanpa tercampur
    // dengan kode sebelumnya
    save_history(sentence, &code);
    fclose(fopen("encodedString.txt", "w"));
    fclose(fopen("hasil.txt", "w"));
    destroy_tree(root);
    free(sentence);
}



void read_via_file(){
    system("cls");
    getchar();
    char letter;
    printf("Masukkan nama file beserta directory (jika bukan satu folder) dan format filenya: ");
    // scanf("%s", filename);
    char *filename = read_dynamic();
    char *file_pointer = (char *)filename;
    FILE *file_to_read;
    file_to_read = fopen(file_pointer, "r");

    if (!file_to_read){
        printf("File tidak ditemukan");
        ask_for_exit();
    }
    int frequency_map[MAX_ASCII_CHARACTER] = {0};
    while ((letter = fgetc(file_to_read)) != EOF){
        frequency_map[(int)letter++]++;
    }
    fclose(file_to_read);
    codewords code = { 0 };
    codewords table[MAX_ASCII_CHARACTER] = { 0 };
    huffman_node_t *root = create_huffman(frequency_map);

    create_code(root, table, code);
    print_code_table(table);

    printf("\nString setelah dikompresi\n");
    file_to_read = fopen(file_pointer, "r");
    char input_str[MAX_ASCII_CHARACTER * 1000] = {0}; // inisialisasi string input
    while ((letter = fgetc(file_to_read)) != EOF){
        code_print(table + letter);
        write_code_to_file(table + letter);
        write_code_to_file_hasil(table + letter);
        sprintf(input_str + strlen(input_str), "%c", letter); // menambahkan karakter ke string input
    }
    printf("\n");
    printf("\nString setelah dekompresi\n");
    decode_string(root);
    save_history(input_str, &code);
    printf("\n");
    fclose(fopen("encodedString.txt", "w"));
	fclose(fopen("hasil.txt", "w"));
    fclose(file_to_read);
	destroy_tree(root);
}
