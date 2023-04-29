/*  
    ===========HEADER PROGRAM HUFFMAN CODING===============
    KELAS    : 1A 
    KELOMPOK : 1B
    ANGGOTA: : MUHAMMAD RAMA NURIMANI 221524021
               RAHMA DIVINA 221524026
               RAYHAN FANEZ 221524027
    =======================================================
*/


#ifndef huffman_coding_tree_h
#define huffman_coding_tree_h
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <windows.h>
#include<ctype.h>

#define BLANK_CHARACTER '\0'
#define MAX_ASCII_CHARACTER 128

#define READ_STRING 1
#define READ_FILE 2
#define READ_MAP_CHAR 3 
#define DISPLAY_HISTORY 4 
#define DISPLAY_ABOUT 5 
#define EXIT 6
#define BUFFER_SIZE 256

/* Struktur huffman node */
typedef struct huffman_node
{
    wchar_t letter;
    int frequency;
    struct huffman_node *left;
    struct huffman_node *right;
    struct huffman_node *next;
} huffman_node_t;

/* List huffman node */
typedef struct huffman_NRLL
{
    huffman_node_t *front;
    int size;
} huffman_NRLL;

/*  Struktur untuk menyimpan binary code dari huffman node */
typedef struct codewords
{
    size_t code_length;
    uint32_t bit_code;
} codewords;

/**==========================================**/

/*
     Insialisasi sebuah List yang akan berfungsi
    untuk memproses dari list node of huffman tree
    menjadi huffman tree
    I.S : NRLL belum diinisialisasi pointer menunjuk tak tentu dan size berisi tak tentu
    F.S : NRLL pointer menunjuk ke NULL dan size diinsialisasi menjadi
 */
void init_NRLL(huffman_NRLL *NRLL);

/*
    Melakukan pengecekan terhadap NRLL, apakah NRLL tersebut kosong atau tidak
    I.S : Tidak diketahui apakah NRLL kosong atau tidak
    F.S : Diketahui apakah NRLL kosong atau tidak
 */
int is_NRLL_empty(huffman_NRLL NRLL);

/*
    Memasukkan node ke list sesuai dengan urutan prioritas
    Node dengan frekuensi lebih kecil didahulukan
    Sehingga muncul list dengan urutan ascending
    I.S : Tidak ada node di dalam list
    F.S : Terdapat node di dalam list dengan urutan ascending
 */
void input_node(huffman_NRLL *NRLL, huffman_node_t *new_node);

/*
    Mengeluarkan node dari list untuk diproses menjadi left dan right son
    I.S : Node dengan frekuensi terkecil ditunjuk front pada list belum dikeluarkan
    F.S : Node sudah dikeluarkan dari list, pointer NRLL sudah pindah ke next front
 */
huffman_node_t *delete_node(huffman_NRLL *NRLL);

/*
    Membuat node huffman
    I.S : Huffman node belum terbentuk dari gabungan 2 node frekuensi terkecil
    F.S : Huffman node terbentuk dengan left son dan right son sudah menunjuk node yang sesuai
 */
huffman_node_t *make_huffman_node(char letter, int frequency, huffman_node_t *left, huffman_node_t *right);

/*
     Output tree dari level 0
    I.S : Huffman tree belum ditampilkan ke layar
    F.S : Huffman tree ditampilkan ke layar dari level 0
*/
void print_hierarchy(huffman_node_t *current, int level);

/*
    Mencetak isi list yang terdapat pada NRLL
    I.S : List NRLL belum ditampilkan di layar
    F.S : List NRLL sudah ditampilkan di layar
 */
void print_NRLL(huffman_NRLL NRLL);

/*
    Memeriksa apakah node terindikasi sebagai daun atau bukan
    I.S : Tidak diketahui apakah node merupakan daun atau bukan
    F.S : Diketahui apakah node daun atau bukan
 */
bool is_leaf(huffman_node_t *node);

/*
    Membuat huffman tree dari kumpulan node huffman (forest)
    I.S : Huffman tree belum terbentuk dari kumpulan huffman node (forest)
    F.S : Huffman tree sudah terbentuk dari kumpulan huffman node (forest)
 */
void build_huffman(huffman_NRLL *NRLL);

/*
    Menjalankan pembentukan huffman tree dari map frequency yang ada
    dilanjut inisiasi forest sampai dengan print huffman tree
 */
huffman_node_t *create_huffman(int frequency_map[MAX_ASCII_CHARACTER]);

/*
    Mengkomputasi bit code secara rekursif lalu ditampilkan sesuai karakter dan frekuensinya
    Sumber: //https://stackoverflow.com/questions/47421732/how-can-i-generate-a-binary-code-table-of-a-huffman-tree
    I.S :  Kode setiap karakter belum diketahui
    F.S :  Kode sudah diketahui
 */
void create_code(huffman_node_t *node, codewords *table, codewords code);

/*
    Mencetak bit code ke layar
    Sumber: //https://stackoverflow.com/questions/47421732/how-can-i-generate-a-binary-code-table-of-a-huffman-tree
    I.S : Bit code belum tampil di layar
    F.S : Bit code tampil pada layar
 */
void code_print(codewords *code);

/*
    Menyimpan string yang telah diterjemahkan menjadi kode biner ke dalam encodedString.txt
    I.S : encodedString.txt belum terisi oleh kode biner
    F.S : encodedString.txt sudah terisi oleh kode biner
 */
void write_code_to_file(codewords *code);

/*
    Menerjemahkan string yang telah dikompresi ke string awal dengan membaca encodedString.txt
    I.S : String masih berbentuk hasil kompresi berupa angka biner
    F.S : String sudah diterjemahkan kembali ke string awal
 */
void decode_string(huffman_node_t *root);

/*
    Mencetak kode berbentuk tabel ke layar
    I.S : Tabel kode belum tampil di layar
    F.S : Tabel kode tampil pada layar
 */
void print_code_table(codewords *table);

/*
    Membuat NULL root saat program berakhir
    I.S : Root subvar left dan right son masih menunjuk sebuah node
    F.S : Root sudah menunjuk null
*/
void destroy_tree(huffman_node_t *root);

/*
    Menyimpan history program kedalam file history.txt
    I.S : History program belum tersimpan
    F.S : History program telah tersimpan
*/
void save_history(const char *input_string, codewords *code);

/*
    Menampilkan history ke layar dari file Dengan format input string dan kode hasilnya
    I.S : file belum ditampilkan
    F.S : file sudah ditampilkan
*/
void display_file(const char* filename);

/*
    Melakukan encoding melalui input string. Modul ini akan melakukan pemanggilan modul modul diatas
    I.S : Input dari user belum didapatkan
    F.S : Input user didapatkan dan sudah diencoding menjadi huffman code
*/
void read_via_string();

/*
    Melakukan encoding melalui input  char dan frekuensinya
    I.S : Input dari user belum didapatkan
    F.S: Input user didapatkan dan sudah diencoding menjadi huffman code

*/
void read_via_char();

/*
    Melakukan encoding melalui input file
    I.S : Input user berupa nama file dan ekstensinya belum didapatkan
    F.S : Input user didapatkan dan sudah diencoding menjadi huffman
*/
void read_via_file();

/*
    Melakukan read string secara dinamis dimana setiap karakter
    terbaca sistem akan melakukan penyesuaian memori
    I.S : Kapasitas memori pada sistem masih di inisialisasi dengan nilai awal 1
    F.S : Kapasitas sudah menyesuaikan dengan karakter yang terbaca.

*/
char *read_dynamic();


/*
    Melakukan alokasi node
    I.S: Node belum terpesan dan bisa jadi gagal
    F.S: Node berhasil dipesan sesuai 
*/
huffman_node_t *allocate_node();

/*
    Melakukan cetak teks di tengah layar
    I.S : Masukan belum dimasukkan sebagai parameter dan belum ada tampilan di tengah layar
    F.S : Masukan sudah dimasukkan sebagai parameter lalu muncul tampilan di tengah layar

*/
void printc(char Pesan[]);

/*
    Melakukan cetak judul program
    I.S : Judul belum tampil di layar
    F.S : Judul tampil di layar

*/
void print_title();

int choose_read_type();
void run_huffman();
void exit_huffman();
void ask_for_exit();
char input_char(); 
int input_integer();

#endif
