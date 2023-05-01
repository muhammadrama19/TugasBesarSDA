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

void print_code_table(codewords *table)
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
void code_print(codewords *code)
{
    size_t n = code->code_length;
    while (n-- > 0)
    {
        putchar('0' + ((code->bit_code >> n) & 1));
    }
}

void write_code_to_file(codewords *code)
{
    size_t n = code->code_length;
    FILE *file_to_write = fopen("encodedString.txt", "a");
    FILE *file_result = fopen("result.txt", "a");

    // mengisi file dengan code setelah kompresi
    while (n-- > 0)
    {
        fputc(('0' + ((code->bit_code >> n) & 1)), file_to_write);
        fputc(('0' + ((code->bit_code >> n) & 1)), file_result);
    }
    fclose(file_to_write);
    fclose(file_result);
}

void save_history(const char *input_string, codewords *code)
{
    // Membuka file history.txt untuk menambahkan hasil kompresi
    FILE *file_to_write = fopen("history.txt", "a");

    // Menyimpan string input sebelum dikompresi ke dalam file
    fputs(input_string, file_to_write);
    fputs(", ", file_to_write);

    // Membuka file untuk membaca string setelah dikompresi
    FILE *file_to_read = fopen("encodedString.txt", "r");
    char compressed_str[MAX_ASCII_CHARACTER * 8 + 1];
    fgets(compressed_str, MAX_ASCII_CHARACTER * 8 + 1, file_to_read);
    fclose(file_to_read);

    // Menyimpan string hasil kompresi ke dalam file
    fputs(compressed_str, file_to_write);
    fputs("\n", file_to_write);
    fputs("\n", file_to_write);

    // Menutup file history.txt
    fclose(file_to_write);
}

void display_file(const char* filename)
{
    system("cls");
    FILE *file_to_read = fopen(filename, "r");
    if (file_to_read == NULL)
    {
        printf("File history.txt tidak ditemukan.\n");
        return;
    }
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
    printf("\nTekan 1 : Kembali ke Menu, 0: Keluar Program\n");
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
    printc("=======Menutup program=======\n");
    printc("Terimakasih telah menggunakan program kami semoga bermanfaat!\n");
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
        display_file("history.txt");
        break;
    }
    case DISPLAY_ABOUT:
    {
        display_file("about.txt");
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
    printc("================\n");
    printc("Huffman Coding\n");
    printc("created by: Kelompok 1B Kelas 1A\n");
    printc("===================\n");
}

int choose_read_type()
{
    system("cls");
    print_title();
    printf("\n\n");
    printc("1. Input via kalimat\n");
    printc("Contoh: JTK POLBAN\n");
    printc("\n");
    printc("2. Input via file txt\n");
    printc("Contoh: tesFile.txt\n");
    printc("\n");
    printc("3. Input via karakter dan frekuensinya\n");
    printc("Contoh: Karakter 'a', jumlah frekuensi: 2\n");
    printc("\n");
    printc("4. History\n");
    printc("\n");
    printc("5. About\n");
    printc("\n");
    printc("6. Exit\n");
    printc("\n");
    printc("Pilihan:  ");

    int answer;
    scanf("%d", &answer);
    return answer;
}



char input_char()
{
    char input[100];
    Start:
    fgets(input, 100, stdin);
    if (strlen(input) == 2) {
        return input[0];
    } else {
        printf("Invalid input. Masukan berupa karakter!.\n");
        printf("Masukan karakter: ");
        goto Start;
    }
}

int input_integer(void) {
    int input, valid;
    char buffer[1024];
    do {
        valid = 1;
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Invalid input. Masukan berupa angka!\n");
            printf("Masukan angka: ");
            valid = 0;
        }
        else if (sscanf(buffer, "%d", &input) != 1) {
            printf("Invalid input. Masukan berupa angka!\n");
            printf("Masukan angka: ");
            valid = 0;
        }
        else {
            // Check for non-integer characters in the input
            for (char *p = buffer; *p != '\0'; p++) {
                if (!isspace(*p) && !isdigit(*p) && *p != '+' && *p != '-') {
                    printf("Invalid input. Masukan berupa angka.\n");
                    printf("Masukan angka: ");
                    valid = 0;
                    break;
                }
            }
        }
    } while (!valid);
    return input;
}

