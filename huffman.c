#include "header.h"
#include "NRLL.c"

bool is_leaf(huffman_node_t *node)
{
    return node->left == NULL && node->right == NULL;
}

huffman_node_t *make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right)
{
    huffman_node_t *new_node = (huffman_node_t *)malloc(sizeof(huffman_node_t));


    if (new_node == NULL)
    {
        printf("Alokasi gagal...");
        return;
    }
    new_node->letter = letter;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right;
    new_node->next = NULL; // NULL Karena sudah mulai membentuk tree
    return new_node;
}

huffman_node_t *create_huffman(int frequency_map[MAX_ASCII_CHARACTERS])
{
    int i;
    huffman_NRLL forest;
    init_NRLL(&forest);

    for (i = 0; i < MAX_ASCII_CHARACTERS; i++)
    {
        if (frequency_map[i])
        {
            //pemrosesan awal dimana masih berbentuk list
            huffman_node_t *new_node = make_huffman_node(i, frequency_map[i], NULL, NULL);
            
            input_node(&forest, new_node);
        }
    }

    system("cls");
    printf("Kondisi awal setiap node dalam list\n");
    print_NRLL(forest);

    // buat huffman tree
    build_huffman_tree(&forest);
    printf("\nHierarki Huffman Tree\n");
    print_hierarchy(forest.front, 0);
    return forest.front;
}

void build_huffman_tree(huffman_NRLL *NRLL)
{
    if (is_NRLL_empty(*NRLL));
    {
        printf("Gagal membuat...");
        exit(EXIT_FAILURE);
    }

    printf("\nMemulai proses pembentukan huffman tree...\n\n");
    if ((*NRLL).size == 1)
    {
        // handling untuk teks hanya dengan satu karakter
        print_NRLL(*NRLL);
    }
    else
    {
        int steps = 1;
        while ((*NRLL).size > 1)
        {
            huffman_node_t *smallest=(huffman_node_t*)malloc(sizeof(huffman_node_t));
            huffman_node_t *second_smallest=(huffman_node_t*)malloc(sizeof(huffman_node_t));

            if (smallest==NULL || second_smallest)
            {
                printf("Alokasi gagal!");
                return;
            }
            
            // ambil dua node dengan frequency terkecil
            smallest = delete_node(NRLL);
            second_smallest = delete_node(NRLL);

            // buat node dengan frequency gabungan dari keduanya
            // dengan anak smallest sebagai anak kiri dan nd_smallest anak kanan
            huffman_node_t *new_node = make_huffman_node(BLANK_CHAR, smallest->frequency + second_smallest->frequency, smallest, second_smallest);

            // masukkan kembali ke queue untuk diurutkan
            input_node(NRLL, new_node);
            printf("Step %d:\t", steps++);
            print_NRLL(*NRLL);
        }
    }
}

void create_code(huffman_node_t *node, codeblocks *table, codeblocks code)
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

void read_via_char() //not finished
{
    print_title();
    system("cls");
    getchar(); // pembuang karakter enter

    int i, sum_of_character, frequency_map[MAX_ASCII_CHARACTERS] = {0};
    char *input_string;

    // meminta inputan jumlah karakter yang aHan diencode
    printf("Masukkan banyak karakter yang aHan diencode : ");
    scanf("%d", &sum_of_character);
    printf("\n");

    input_string = (char *)malloc(10000 * sizeof(char)); // allocate memory for the input string

    // membaca karakter dan frekuensinya
    for (i = 0; i < sum_of_character; i++)
    {
        char letter;
        printf("Karakter ke - %d\n", i + 1);
        printf("Masukkan karakter\t: ");
        scanf(" %c", &letter);

        // handle ketika ada karakter yang telah ada diinput kembali
        // karakter akan ditambah frekuensinya
        if (frequency_map[letter])
        {
            int frequency;
            scanf("%d", &frequency);
            frequency_map[letter] += frequency;
        }
        else
        {
            printf("Masukkan frekuensi\t: ");
            scanf("%d", &frequency_map[letter]);
        }
        input_string[i] = letter; // Add the letter to the input string
        printf("\n");
    }
    input_string[i] = '\0'; // Null-terminate the input string
    codeblocks code = {0};
    codeblocks table[MAX_ASCII_CHARACTERS] = {0};
    huffman_node_t *root = create_huffman(frequency_map);
    if (root==NULL)
    {
        printf("Alokasi gagal, root tidak terbentuk.");
    }
    
    create_code(root, table, code);
    print_code_table(table);

    const char *string_new = (const char *)input_string;
    const char *for_history = (const char *)input_string;

    printf("\nHasil encode:");
    while (*string_new)
    {
        int letter = (int)*string_new++;
        code_print(table + letter);
        write_code_to_file(table + letter);
        write_code_to_file_hasil(table + letter);

        printf("\nString setelah didecode\n");
        decode_string(root);
        printf("\n");
        save_history(for_history, &code);
        fclose(fopen("encodedString.txt", "w"));
        fclose(fopen("hasil.txt", "w"));
        destroy_tree(root);
        free(input_string);
    }
}

void read_via_string()
{
    print_title();
    system("cls");
    getchar(); // pembuang karakter enter
    int unique_chars = 0, sentence_size = 0, sentence_capacity = 1;
    char c;
    char *sentence = read_dynamic();

    // kalimat diubah dari array menjadi array constant
    const char *string = (const char *)sentence;
    const char *to_encode_string = (const char *)sentence;

    // jumlah frekuensi ditaruh ke dalam array, dengan indeks
    // array sebagai representasi karakter yang dihitung
    int frequency_map[MAX_ASCII_CHARACTERS] = {0};
    while (*string)
    {
        frequency_map[(int)*string++]++;
    }

    // membuat huffman tree, mengembalikan root untuk proses decode nantinya
    codeblocks code = {0};
    codeblocks table[MAX_ASCII_CHARACTERS] = {0};

    // check unique char
    for (int i = 0; i < MAX_ASCII_CHARACTERS; i++)
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

    // alert for unique
    if (unique_chars == 1 || sentence[1] == '\0')
    {
        printf("Tree tidak dapat terbentuk karena hanya terdapat 1 huruf atau 1 jenis huruf..\n");
        free(sentence);
        return;
    }

    huffman_node_t *root = create_huffman(frequency_map);
    if (root==NULL)
    {
        printf("Alokasi gagal, root tidak terbentuk.");
    }

    // make code
    create_code(root, table, code);
    print_code_table(table);

    // menampilkan hasil encode dari stirng yang telah diterjemahkan
    printf("\nString setelah diencode\n");
    while (*to_encode_string)
    {
        int letter = (int)*to_encode_string++;
        print_code(table + letter);
        write_code_to_file(table + letter);
        write_code_to_file_hasil(table + letter);
    }
    printf("\n");

    // menampilkan string setelah didecode
    printf("\nString setelah didecode\n");
    decode_string(root);
    printf("\n");

    // saving
    save_history(sentence, &code);
    fclose(fopen("encodedString.txt", "w"));
    fclose(fopen("hasil.txt", "w"));
    free(sentence);
    destroy_tree(root);
}

void destroy_tree(huffman_node_t *root)
{
    if (root != NULL)
    {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free(root);
    }
}

void write_code_to_file(codeblocks *code)
{
    int n = code->code_length;
    FILE *file_to_write = fopen("encodedString.txt", "a");

    // mengisi file dengan code setelah encode
    while (n-- > 0)
    {
        fputc(('0' + ((code->bit_code >> n) & 1)), file_to_write);
    }
    fclose(file_to_write);
}

void write_code_to_file_hasil(codeblocks *code)
{
    size_t n = code->code_length;
    FILE *file_to_write = fopen("hasil.txt", "a");

    // mengisi file dengan code setelah encode
    while (n-- > 0)
    {
        fputc(('0' + ((code->bit_code >> n) & 1)), file_to_write);
    }
    fclose(file_to_write);
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

void save_history(const char *input_string, codeblocks *code)
{
    // Membuka file history.txt untuk menambahkan hasil encode
    FILE *file_to_write = fopen("history.txt", "a");

    // Menyimpan string input sebelum diencode ke dalam file
    fputs(input_string, file_to_write);
    fputs(", ", file_to_write);

    // Membuka file hasil.txt untuk membaca string setelah diencode
    FILE *file_to_read = fopen("hasil.txt", "r");
    char compressed_str[MAX_ASCII_CHARACTERS * 8 + 1];
    fgets(compressed_str, MAX_ASCII_CHARACTERS * 8 + 1, file_to_read);
    fclose(file_to_read);

    // Menyimpan string hasil encode ke dalam file
    fputs(compressed_str, file_to_write);
    fputs("\n", file_to_write);

    // Menutup file history.txt
    fclose(file_to_write);
}

