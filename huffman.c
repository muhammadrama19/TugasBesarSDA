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
        create_code_table(node->left, table, code);
        code.bit_code += 1;
        create_code_table(node->right, table, code);
    }
}

void read_via_string() 
{
    system("cls");
	getchar();			//pembuang karakter enter
	int unique_chars=0, sentence_size =0, sentence_capacity =1;
	char c;
    char *sentence;

    //read dynamic untuk menghindari memory leak
	sentence=(char *)malloc(1);
	printf("Masukkan kalimat yang akan dikompresi: ");	
	 while ((c = getchar()) != '\n') {
        //cek jika penuh
        if (sentence_size == sentence_capacity - 1) {
            sentence_capacity *= 2;
            sentence = realloc(sentence, sentence_capacity);
        }
        sentence[sentence_size++] = c;
    }

    sentence[sentence_size] = '\0';

    //kalimat diubah dari array menjadi array constant
    const char* string = (const char*) sentence;
    const char* to_encode_string = (const char*) sentence;

    //jumlah frekuensi ditaruh ke dalam array, dengan indeks
	//array sebagai representasi karakter yang dihitung
	int frequency_map[MAX_ASCII_CHARACTERS] = { 0 };
	while (*string){
		frequency_map[(int)*string++]++;
	}

    //membuat huffman tree, mengembalikan root untuk proses decode nantinya
    codeblocks code = { 0 };
    codeblocks table[MAX_ASCII_CHARACTERS] = { 0 };
    
    //check unique char
    for (int i = 0; i < MAX_ASCII_CHARACTERS; i++) {
        if (frequency_map[i] > 0) {
            unique_chars++;
        }
        if (unique_chars > 1) {
            break;
        }
    }
    
    //alert for unique
    if (unique_chars ==1 || sentence[1]=='\0') {
    	printf("Tree tidak dapat terbentuk karena hanya terdapat 1 huruf atau 1 jenis huruf..\n");
        free(sentence);
        return;
	}
	
	huffman_node_t *root = create_huffman(frequency_map);

    //make code
	create_code(root, table, code);
    print_code_table(table);

    //menampilkan hasil kompresi dari stirng yang telah diterjemahkan
    printf("\nString setelah kompresi\n");
    while (*to_encode_string){
		int letter = (int) *to_encode_string++;
		print_code(table + letter);
		write_code_to_file(table + letter);
		write_code_to_file_hasil(table + letter);
	}
    printf("\n");

    //menampilkan string setelah didekompresi
    printf("\nString setelah dekompresi\n");
    decode_string(root);
    printf("\n");

    //saving
    save_history(sentence,&code);
	fclose(fopen("encodedString.txt", "w"));
	fclose(fopen("hasil.txt", "w"));
	free(sentence);
	destroy_tree(root);
}

void destroy_tree(huffman_node_t* root) {
    if (root != NULL) {
        destroy_tree(root->left);
        destroy_tree(root->right);
        free(root);
    }
}
