#include "header.h"
#include <stdio.h>

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

void print_hierarchy(huffman_node_t *current, int level)
{
    if (current == NULL)
    {
        return;
    }

    if (current->right != NULL)
    {
        print_hierarchy(current->right, level + 1);
        printf("%*s|\n", level * 4, "");
    }

    printf("%*s", level * 4, "");

    if (current->letter != '\0')
    {
        printf("(%d) '%c'\n", current->frequency, current->letter);
    }
    else
    {
        printf("(%d)\n", current->frequency);
    }

    if (current->left != NULL)
    {
        printf("%*s|\n", level * 4, "");
        print_hierarchy(current->left, level + 1);
    }
}

void print_code_table(codeblocks *table)
{
    printf("\nTabel Konversi\n");
    int x;
    for (x = 0; x < MAX_ASCII_CHARACTER; ++x)
    {
        if (table[x].code_length)
        {
            printf("\'%c\'\t", x);
            code_print(table + x);
            printf("\n");
        }
    }
}
void code_print(codeblocks *code)
{
    size_t n = code->code_length;
    while (n-- > 0)
    {
        putchar('0' + ((code->bit_code >> n) & 1));
    }
}

void write_code_to_file(codeblocks *code)
{
    size_t n = code->code_length;
    FILE *file_to_write = fopen("encodedString.txt", "a");

    // mengisi file dengan code setelah kompresi
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

    // mengisi file dengan code setelah kompresi
    while (n-- > 0)
    {
        fputc(('0' + ((code->bit_code >> n) & 1)), file_to_write);
    }
    fclose(file_to_write);
}

void save_history(const char *input_string, codeblocks *code)
{
    // Membuka file history.txt untuk menambahkan hasil kompresi
    FILE *file_to_write = fopen("history.txt", "a");

    // Menyimpan string input sebelum dikompresi ke dalam file
    fputs(input_string, file_to_write);
    fputs(", ", file_to_write);

    // Membuka file hasil.txt untuk membaca string setelah dikompresi
    FILE *file_to_read = fopen("hasil.txt", "r");
    char compressed_str[MAX_ASCII_CHARACTER * 8 + 1];
    fgets(compressed_str, MAX_ASCII_CHARACTER * 8 + 1, file_to_read);
    fclose(file_to_read);

    // Menyimpan string hasil kompresi ke dalam file
    fputs(compressed_str, file_to_write);
    fputs("\n", file_to_write);

    // Menutup file history.txt
    fclose(file_to_write);
}

void display_history()
{
    system("cls");
    FILE *file_to_read = fopen("history.txt", "r");
    if (file_to_read == NULL)
    {
        printf("File history.txt tidak ditemukan.\n");
        return;
    }

    printf("Isi dari file history.txt:\n");
    char line[100];
    while (fgets(line, sizeof(line), file_to_read))
    {
        printf("%s", line);
    }

    fclose(file_to_read);
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

char *read_dynamic()
{
    char *sentence = (char *)malloc(1);
    int sentence_size = 0;
    int sentence_capacity = 1;

    // printf("Masukkan kalimat yang akan dikompresi: ");

    // loop untuk membaca input secara karakter per karakter
    char c;
    while ((c = getchar()) != '\n')
    {
        // jika kapasitas kalimat sudah penuh, alokasikan memori baru dengan ukuran dua kali lipat dari kapasitas sebelumnya
        //! avoid memory leak
        if (sentence_size == sentence_capacity - 1)
        {
            sentence_capacity *= 2;
            sentence = (char *)realloc(sentence, sentence_capacity);
        }
        sentence[sentence_size++] = c;
    }

    // tambahkan null terminator pada akhir kalimat
    sentence[sentence_size] = '\0';
    return sentence;
}

void ask_for_exit()
{
    int answer;
    printf("\nApakah Anda ingin mensimulasikan kembali? 0: Tidak, 1: Ya\n");
    scanf("%d", &answer);

    switch (answer)
    {
    case 1:
    {
        run_huffman();
        break;
    }
    case 0:
    {
        exit_huffman();
        break;
    }
    default:
    {
        printf("\nPilihan tidak valid! Pilih sesuai pilihan yang tersedia!\n");
        ask_for_exit();

        break;
    }
    }
}

void exit_huffman()
{
    system("cls");
    printf("\tMenutup program...\n");
    printf("\tProgram berakhir...\n");
    exit(1);
}

void run_huffman()
{
    int read_t = choose_read_type();
    switch (read_t)
    {
    case READ_STRING:
    {
        read_via_string();
        break;
    }
    case READ_FILE:
    {
        read_via_file();
        break;
    }
    case READ_MAP_CHAR:
    {
        read_via_char();
        break;
    }
    case DISPLAY_HISTORY:
    {
        display_history();
        break;
    }
    case EXIT:
    {
        exit_huffman();
        break;
    }
    default:
    {
        printf("\nPilihan tidak valid! Pilih sesuai pilihan yang tersedia!\n");
        printf("Tekan apapun untuk melanjutkan");
        getchar();
        run_huffman();
    }
    }
    ask_for_exit();
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

void print_title()
{
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

int choose_read_type()
{
    system("cls");
    // printf("\tProgram Simulasi Huffman Coding\t\n\n");
    // print_title();
    printf("\n\n");
    printc("1. Input via kalimat\n");
    printc("Contoh: JTK POLBAN\n");
    printf("\n");
    printc("2. Input via file txt\n");
    printc("Contoh: tesFile.txt\n");
    printf("\n");
    printc("3. Input via karakter dan frekuensinya\n");
    printc("Contoh: Karakter 'a', jumlah frekuensi: 2\n");
    printf("\n");
    printc("4. History\n");
    printf("\n");
    printc("5. About\n");
    printf("\n");
    printc("6. Exit\n");
    printf("\n");
    printc("Pilihan:  ");

    int answer;
    scanf("%d", &answer);
    return answer;
}
