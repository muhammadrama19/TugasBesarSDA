int choose_read_type()
{
    system("cls");
    //printf("\tProgram Simulasi Huffman Coding\t\n\n");
    print_title();
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
