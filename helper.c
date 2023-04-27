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

void print_NRLL(huffman_NRLL NRLL)
{
    printf("[");
    huffman_node_t *current = NRLL.front;
    while (current != NULL)
    {
        printf("%c%d", current->letter, current->frequency);
        if (current->next != NULL)
            printf(", ");
        current = current->next;
    }
    printf("]\n");
}

char* read_dynamic(){
    char* sentence = (char*)malloc(1);
    int sentence_size = 0;
    int sentence_capacity = 1;

    // printf("Masukkan kalimat yang akan dikompresi: ");

    //loop untuk membaca input secara karakter per karakter
    char c;
    while ((c = getchar()) != '\n') {
        //jika kapasitas kalimat sudah penuh, alokasikan memori baru dengan ukuran dua kali lipat dari kapasitas sebelumnya
        //!avoid memory leak
        if (sentence_size == sentence_capacity - 1) {
            sentence_capacity *= 2;
            sentence = (char*)realloc(sentence, sentence_capacity);
        }
        sentence[sentence_size++] = c;
    }

    //tambahkan null terminator pada akhir kalimat
    sentence[sentence_size] = '\0';
    return sentence;
}

void printc(char Pesan[])
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// ^ Menyimpan info tentang screen buffer
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// ^ Console screen buffer  ^ Konsol input                  ^output
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	//        ^ Mendapat jumlah karakter maksimal pada baris konsol

	if (((columns - strlen(Pesan)) / 2) > 1)
	{
		for (size_t i = 0; i < ((columns - strlen(Pesan)) / 2); i++)
		{
			printf(" ");
		}
	}
	// ^ Printf spasi sebanyak yang dibutuhkan teks agar memiliki posisi tengah

	printf("%s", Pesan);
	// ^ Print pesan yang ingin diletakan di tengah layar
}

void print_title(){
    printf("\n\n");
    printc("888    888888     88888888888888888888888888b     d888       d8888888b    888   .d8888b.  .d88888b. 8888888b.8888888888b    888 .d8888b.  \n");  
    printc("888    888888     888888       888       8888b   d8888      d888888888b   888   d88P  Y88bd88P' 'Y88b888' 'Y88b 888  8888b   888d88P  Y88b\n"); 
    printc("888    888888     888888       888       88888b.d88888     d88P88888888b  888   888    888888     888888    888 888  88888b  888888    888\n"); 
    printc("8888888888888     8888888888   8888888   888Y88888P888    d88P 888888Y88b 888   888       888     888888    888 888  888Y88b 888888       \n");        
    printc("888    888888     888888       888       888 Y888P 888   d88P  888888 Y88b888   888       888     888888    888 888  888 Y88b888888  88888\n"); 
    printc("888    888888     888888       888       888  Y8P  888  d88P   888888  Y88888   888    888888     888888    888 888  888  Y88888888    888\n"); 
    printc("888    888Y88b. .d88P888       888       888   '   888 d8888888888888   Y8888   Y88b  d88PY88b. .d88P888  .d88P 888  888   Y8888Y88b  d88P\n"); 
    printc("888    888 'Y88888P' 888       888       888       888d88P     888888    Y888   'Y8888P'  'Y88888P' 8888888P'8888888888    Y888 'Y8888P88 \n");
    printf("\n\n");
    printc("\t\t\t\t\t\t\t      created by Kelompok 1B Kelas 1A\n");
    printf("\n\n\n\n\n");
}
