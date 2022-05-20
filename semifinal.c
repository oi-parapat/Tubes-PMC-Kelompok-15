/*EL2006 Pemecahan Masalah dengan C 2021/2022
*Modul            : Logic Minimization
*Percobaan        : 1
*Hari dan Tanggal : Kamis, 28 April 2022
*Kelompok         : 15
*Anggota          : 18320030 - Oi
                    18320032 - Ilsa Rostiana
                    18320034 - Richard Albertus Moektijaya
                    18320042 - Rd. Elviana La�salina Muhlis
*Nama File        : Final.c
*Deskripsi        : Program Minimalisasi Fungsi Logika dengan metode Quine-Mccluskey
*/


#include <stdio.h>
#include <stdlib.h>
#define YELLOW "\033[0;33m"
#define RESET "\x1b[0m"

struct node
{
    int data[257], bin[26], noofones, isimplicant, minarr[1000];
    char term[26];
    struct node* right;
};

struct node* root, * head, * improot, * save, * fin;
int var, min, number = 1, columns = 2, check = 1, limit, imptable[100][100], counter = 0, essential[1000], t = 0, no = 0, minterms[1000];
char a[26], b[26];       //variable names are stored as alphabets, can be modified to work for more variables

void arrange();                         //the minterms are arranged according to their magnitude
void swap(struct node*, struct node*);  //data of two nodes is swapped
void store_minterms();                  //minterms are stored in an array
void group1();                          //the minterms are grouped according to the number of ones
void further_groupings();               //the minterms are paired
void disp();                            //various column with pairings are displayed

void end_loop(struct node*);            //the extra node in a list is deleted
void collect();                         //converts the term from binary notation to variables (OUTPUT)
void implicants(struct node*);          //initializes each term as an implicant
void display_implicants();              //the implicants are displayed
void variables();               //the variables for the function are stored
void implicants_table();        //the prime implicants table is formed and essential implicants are found
void other_implicants();    //the prime implicants other than the essential ones are collected
void final_terms();         //the final terms in the minimized function are noted

void convert();             //reduces the prime implicants which occur more than once to one ??
void func();                //the minimized function is displayed (OUTPUT)

int main()
{
    int i, j, k, x;
    struct node* temp;
    printf(YELLOW "\n[ PROGRAM LOGIC MINIMIZATION - QUINE MCCLUSKEY]\n" RESET);

    // Input jumlah variabel
    printf("\nJumlah Variabel: ");
    scanf("%d", &var);

    // Input jumlah minterm
    printf("\nJumlah Minterm: ");
    scanf("%d", &min);

    i = min - 1;
    root = temp = (struct node*)malloc(sizeof(struct node)); // Alokasi memori temp untuk minterm

    // Input setiap minterm
    printf("\nMasukkan setiap minterm, satu per satu:\n");
    scanf("%d", &temp->data[0]);                     // Minterm pertama disimpan

    j = temp->data[0];
    temp->noofones = 0;
    x = var;
    k = 0;
    while (x--)      // Konversi minterm ke notasi biner
    {
        if (j % 2 == 1)
        {
            temp->bin[k] = 1;
            temp->noofones++;   // Jumlah angka 1 dalam notasi biner dihitung
        }
        else
        {
            temp->bin[k] = 0;
        }
        j = j / 2;
        k++;
    }

    while (i--)      // Minterm sisanya disimpan
    {
        temp = temp->right = (struct node*)malloc(sizeof(struct node));
        scanf("%d", &temp->data[0]);

        j = temp->data[0];
        temp->noofones = 0;
        x = var;
        k = 0;
        while (x--)                  // Konversi minterm lainnya ke notasi biner
        {
            if (j % 2 == 1)
            {
                temp->bin[k] = 1;
                temp->noofones++;
            }
            else
            {
                temp->bin[k] = 0;
            }
            j = j / 2;
            k++;
        }
    }
    temp->right = NULL; // Ujung node adalah NULL

    arrange();          //Pemanggilan fungsi-fungsi menurut kebutuhannya
    store_minterms();
    group1();
    disp();
    end_loop(root);
    head = (struct node*)malloc(sizeof(struct node));
    while (check > 0)
    {
        further_groupings();
    }
    save->right = NULL;          // Menyimpan nilai NULL di link field of list yang menyimpan implikan utama
    printf("Tidak ada pasangan yang terbentuk sehingga tidak diperlukan perhitungan lebih lanjut\n\n");
    end_loop(improot);
    collect();
    display_implicants();
    variables();
    implicants_table();
    other_implicants();
    final_terms();
    end_loop(fin);
    convert();
    func();
    return 0;
}

void arrange()          // Mengurutkan minterm berdasarkan besarnya
{
    struct node* temp1, * temp2;
    temp1 = temp2 = root;
    while (temp1 != NULL)
    {
        temp2 = root;
        while (temp2 != NULL)
        {
            if (temp1->data[0] < temp2->data[0])       // Jika tidak urut maka mereka ditukar posisinya dengan fungsi swap
            {
                swap(temp1, temp2);
            }
            temp2 = temp2->right;
        }
        if (temp1->right == NULL)
        {
            // ??????
            limit = temp1->data[0];           // Besarnya minterm terakhir dicatat kemudian untuk tabel implikan utama
        }
        temp1 = temp1->right;
    }
}

void swap(struct node* temp1, struct node* temp2)        // Fungsi menukar semua data dari dua node
{
    int x, y, i = 0;
    i = var;
    for (i = 0; i < var; i++)      // Notasi biner ditukar
    {
        y = temp1->bin[i];
        temp1->bin[i] = temp2->bin[i];
        temp2->bin[i] = y;
    }
    y = temp1->noofones;          // Jumlah angka 1 ditukar
    temp1->noofones = temp2->noofones;
    temp2->noofones = y;

    x = temp1->data[0];           // data(minterm) ditukar
    temp1->data[0] = temp2->data[0];
    temp2->data[0] = x;
}

void store_minterms()       //Array untuk menyimpan seluruh minterm
{
    int i = 0;
    struct node* temp;
    temp = root;
    while (temp != NULL)
    {
        minterms[i] = temp->data[0];
        i++;
        temp = temp->right;
    }
}

void group1()       // Minterms diatur sesuai dengan jumlah angka 1 pada notasi biner
{
    int i, count = 0, j, k = 0;
    struct node* temp, * next;
    temp = save = root;
    root = next = (struct node*)malloc(sizeof(struct node));
    for (i = 0; i <= var; i++)
    {
        temp = save;
        while (temp != NULL)
        {
            if (temp->noofones == i)       // Minterm diatur menurut jumlah angka 1: dari 0, lalu 1, lalu 2, dst ...
            {
                next->data[0] = temp->data[0];
                k++;
                for (j = 0; j < var; j++)
                {
                    next->bin[j] = temp->bin[j];
                }
                next->noofones = temp->noofones;
                next = next->right = (struct node*)malloc(sizeof(struct node));
            }
            temp = temp->right;
        }
    }
    minterms[k] = -1;
    next->right = NULL;
}

void disp()     // Untuk menampilkan berbagai kolom dengan pasangannya
{
    int i, j = min;
    struct node* temp;
    temp = root;
    printf("\n\nKolom #%d\n", number);          // number memberitahu kita kolom berapa yang sedang dicetak
    while (temp->right != NULL)
    {
        printf("%d\t", temp->data[0]);
        for (i = var - 1; i >= 0; i--)
        {
            printf("%d", temp->bin[i]);
        }
        temp = temp->right;
        printf("\n");
    }
    temp->right = NULL;
    number++;
}
//---------------------------------------------------------------------------------------------------//

void end_loop(struct node* ptr)         // Mengurangi jumlah node dalam list dengan satu node tambahan
{
    struct node* temp;
    temp = ptr;
    while (temp->right->right != NULL)
    {
        temp = temp->right;
    }
    temp->right = NULL;
}

void further_groupings()    // Pengelompokan lebih lanjut berdasarkan perbedaan notasi biner
{
    int i, count, k, j, x;
    struct node* temp, * next, * p, * imp;
    check = 0;
    if (columns == 2)      // Untuk kolom kedua
    {
        imp = improot = (struct node*)malloc(sizeof(struct node));
        p = head;
    }
    else        // Untuk kolom lain seterusnya
    {
        imp = save;
        root = head;
        p = head = (struct node*)malloc(sizeof(struct node));
    }
    temp = root;
    implicants(root);
    printf("\n\nKolom #%d\n", number);
    while (temp != NULL)
    {
        next = temp->right;
        while (next != NULL)
        {
            count = 0;
            if (next->noofones - temp->noofones == 1)        // Jika dua term berbeda jumlah angka 1 nya sebesar satu
            {
                for (i = 0; i < var; i++)
                {
                    if (temp->bin[i] != next->bin[i])
                    {
                        k = i;            //tempat di mana mereka(term) yang berbeda tersebut (dari atas) dicatat
                        count++;
                    }
                }
            }
            if (count == 1)        // Memeriksa apakah kedua term berbeda hanya pada satu tempat dalam notasi biner
            {
                temp->isimplicant = 0;        //Jika ya maka mereka bukan implikan utama
                next->isimplicant = 0;
                check++;
                for (i = 0; i < var; i++)
                {
                    p->bin[i] = temp->bin[i];         // Notasi biner disimpan
                }
                p->bin[k] = -1;
                x = 0;
                for (j = 0; j < columns / 2; j++)            // data dari term pertama disimpan
                {
                    p->data[x] = temp->data[j];
                    x++;
                }
                for (j = 0; j < columns / 2; j++)            // data dari term kedua disimpan
                {
                    p->data[x] = next->data[j];
                    x++;
                }
                p->noofones = temp->noofones;
                for (j = 0; j < columns; j++)      // Pasangan yang dihasilkan ditampilkan
                {
                    printf("%d,", p->data[j]);
                }
                printf("\b ");
                printf("\t");
                for (i = var - 1; i >= 0; i--)
                {
                    if (p->bin[i] == -1)
                        printf("-");
                    else
                        printf("%d", p->bin[i]);
                }
                printf("\n");
                p = p->right = (struct node*)malloc(sizeof(struct node));           // Node tambahan yang ada untuk dihapus
            }
            next = next->right;
        }
        temp = temp->right;
    }
    p->right = NULL;

    if (check != 0)
    {
        end_loop(head);     // Node ekstra dihapus
    }
    temp = root;
    while (temp != NULL)           // Loop untuk memilih iplikan utama
    {
        if (temp->isimplicant == 1)        // Jika term adalah implikan utama, ia disimpan secara terpisah dalam list dengan head pointer improot 
        {
            i = 0;
            for (i = 0; i < columns / 2; i++)
            {
                imp->data[i] = temp->data[i];
            }
            imp->data[i] = -1;
            for (i = 0; i < var; i++)
            {
                imp->bin[i] = temp->bin[i];
            }
            imp = imp->right = (struct node*)malloc(sizeof(struct node));
        }
        temp = temp->right;
    }
    save = imp;
    columns = columns * 2;
    number++;
}

void implicants(struct node* ptr)       // Inisialisasi setiap term sebagai implikan utama
{
    struct node* temp;
    temp = ptr;
    while (temp != NULL)
    {
        temp->isimplicant = 1;
        temp = temp->right;
    }
}

void display_implicants()       // Menampilkan seluruh implikannya
{
    int i = 0;
    struct node* temp;
    temp = improot;
    printf("\n\nImplikan utamanya adalah: \n\n");
    while (temp != NULL)
    {
        i = 0;
        i = var - 1;
        while (i >= 0)     // Menampilkan notasi binernya
        {
            if (temp->bin[i] == -1)
            {
                printf("-");
            }
            else
            {
                printf("%d", temp->bin[i]);
            }
            i--;
        }
        printf("\t\t");
        i = 0;
        while (temp->data[i] != -1)        // Menampilkan pasangan mintermnya
        {
            printf("%d,", temp->data[i]);
            i++;
        }
        printf("\b ");
        temp = temp->right;
        printf("\n\n");
        counter++;
    }
}

void collect()          // Mengurangi term yang muncul lebih dari satu kali menjadi sekali
{
    int common = 0, i;
    struct node* temp1, * temp2, * temp3;
    temp1 = temp2 = improot;
    while (temp1 != NULL)
    {
        temp2 = temp1->right;
        while (temp2 != NULL)
        {
            common = 0;
            for (i = 0; i < var; i++)          // Jika notasi biner mereka sama maka akan dihapus
            {
                if (temp2->bin[i] == temp1->bin[i])
                {
                    common++;
                }
            }
            if (common == var)
            {
                temp3 = improot;
                while (temp3->right != temp2)      // Term yang berulang dihapus
                {
                    temp3 = temp3->right;
                }
                temp3->right = temp2->right;
                temp2 = temp3;
            }
            temp2 = temp2->right;
        }
        temp1 = temp1->right;
    }
}

void variables()            //stores variables(alphabets)
{
    int i;
    for (i = 0; i < 26; i++)
    {
        a[i] = 65 + i;      // Variabel normal huruf kapital dengan ASCII
        b[i] = 97 + i;      // Komplemennya huruf kecil
    }
}

void convert()          // Konversi notasi biner setiap term ke variabel
{
    int i, j;
    struct node* temp;
    temp = fin;
    while (temp != NULL)
    {
        j = 0;
        for (i = 0; i < var; i++)
        {
            if (temp->bin[i] == 0)
            {
                temp->term[j] = b[i];
                j++;
            }
            if (temp->bin[i] == 1)
            {
                temp->term[j] = a[i];
                j++;
            }
            if (temp->bin[i] == -1)
            {
                j++;
            }
        }
        temp = temp->right;
    }
}

void func()         //Menampilkan fungsi yang telah diminimisasi dalam bentuk Sum Of Products (SOP)
{
    struct node* temp;
    temp = fin;
    printf("\n\nFungsi yang telah diminimisasi adalah : ");
    while (temp != NULL)
    {
        printf("%s", temp->term);
        if (temp->right != NULL)
        {
            printf(" + ");
        }
        temp = temp->right;
    }
    printf("\n\n");
}

void implicants_table()         // Fungsi untuk membuat tabel implikan utama sekaligus memilih implikan utama esensial
{
    struct node* temp;
    int i, j, k, m, n, x, y, count = 0, count2 = 0, a = 0;
    for (i = 0; i < counter; i++)
    {
        for (j = 0; j <= limit; j++)
        {
            imptable[i][j] = 0;           // 0 atau - ditempatkan pada seluruh bagiantabel
        }
    }
    i = 0;
    j = 0;
    k = 0;
    temp = improot;
    while (temp != NULL)
    {
        k = 0;
        while (temp->data[k] != -1)
        {
            imptable[i][temp->data[k]] = 1;  // 1 atau X ditempatkan untuk kolom dengan indeks yang sama dengan angka dalam pasangannya
            k++;
        }
        i++;
        temp = temp->right;
    }
    printf("\n\n\t\t\tTabel Implikan Prima\n\n\n");
    temp = improot;
    i = 0;
    printf(" ");
    while (minterms[i] != -1)
    {
        printf("%d\t", minterms[i]);         // Mintermnya ditampilkan dalam satu baris
        i++;
    }
    printf("\n\n");
    for (i = 0; i < counter; i++)          //X dan - ditempatkan untuk term yang memiliki nilai minterm yang sesuai
    {
        printf(" ");
        a = 0;
        for (j = 0; j <= limit; j++)
        {
            if (j == minterms[a])
            {
                if (imptable[i][j] == 0)
                {
                    printf("-");
                }
                if (imptable[i][j] == 1)
                {
                    printf("X");
                }
                printf("\t");
                a++;
            }
        }
        y = 0;
        while (temp->data[y] != -1)        // Mencetak pasangan mintermnya
        {
            printf("%d,", temp->data[y]);
            y++;
        }
        printf("\b ");
        temp = temp->right;
        printf("\n\n");
    }
    printf("\n\n");
    for (i = 0; i < counter; i++)      // Untuk menemukan implikan utama esensial
    {
        for (j = 0; j <= limit; j++)
        {
            count = 0;
            if (imptable[i][j] == 1)
            {
                y = j;
                x = i;
                for (k = 0; k < counter; k++)
                {
                    if (imptable[k][j] == 1)       // Cek apakah hanya ada satu X dalam kolomnya
                    {
                        count++;
                    }
                }
                if (count == 1)  // Menaruh - sebagai pengganti X di setiap kolom tabel yang satu barisnya hanya berisi satu X dalam satu kolom
                {
                    essential[t] = x;
                    t++;
                    for (n = 0; n <= limit; n++)
                    {
                        if (imptable[i][n] == 1)
                        {
                            for (m = 0; m < counter; m++)
                            {
                                imptable[m][n] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    essential[t] = -1;
    i = 0;
}

void other_implicants()     // Setelah menemukan implikan prima esensial, term lain yang diperlukan ditandai
{
    no = 0;           // Untuk mengecek apakah ada term yang ditemukan pada tiap iterasi
    int count1 = 0, count2 = 0;
    int i, j;
    for (i = 0; i < counter; i++)
    {
        count1 = 0;
        for (j = 0; j <= limit; j++)
        {
            if (imptable[i][j] == 1)       // Masing-masing jumlah X dan 1 dihitung
            {
                no++;
                count1++;
            }
        }
        if (count1 > count2)       // Untuk menemukan term dengan jumlah X maksimum dalam satu baris
        {
            essential[t] = i;
            count2 = count1;
        }
    }
    for (j = 0; j <= limit; j++)           // Menghapus X di baris serta X yang ada di kolom yang sama
    {
        if (imptable[essential[t]][j] == 1)
        {
            for (i = 0; i < counter; i++)
            {
                imptable[i][j] = 0;
            }
        }
    }
    t++;
    essential[t] = -1;
    if (no > 0)            // Jika satu atau lebih istilah ditemukan, fungsi dipanggil lagi (rekursi)
    {
        other_implicants();
    }
}

void final_terms()          // Dalam fungsi ini semua term dalam fungsi yang diminimalkan disimpan di linked list
{
    int i = 0, j, c = 0, x;
    struct node* temp, * ptr;
    fin = temp = (struct node*)malloc(sizeof(struct node));
    while (essential[i] != -1)
    {
        ptr = improot;
        x = essential[i];
        for (j = 0; j < x; j++)        // Agar pointer menunjuk ke node yang indeksnya disimpan dalam array bernama essential
        {
            ptr = ptr->right;
        }
        j = 0;
        while (ptr->data[j] != -1)         // Data dari node disimpan
        {
            temp->data[j] = ptr->data[j];
            j++;
        }
        temp->data[j] = -1;
        for (j = 0; j < var; j++)          // Notasi binernya disimpan
        {
            temp->bin[j] = ptr->bin[j];
        }
        temp = temp->right = (struct node*)malloc(sizeof(struct node));
        i++;
        c++;
    }
    temp->right = NULL;
}