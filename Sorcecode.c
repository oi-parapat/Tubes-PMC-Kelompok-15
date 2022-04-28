//cobacoba
/*TUBES PMC Kel 15
LOGIC MINIMIZATION
coba coba*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFFER_SIZE 256
#define matsize 20


int main()
{
	// Inisialisasi
	char filename[BUFFER_SIZE];

	// Meminta masukkan
	printf("Masukkan file matrix: ");
	scanf("%s", filename);

    FILE *f = fopen(filename, "r");

	while (!f) {
        printf("Error: file invalid!\n\n");
        printf("Masukkan matrix: ");
        scanf("%s", filename);
        f = fopen(filename,"r");
	}

	// membaca ukuran matriks
    char variabel[10];
	char A,B,C,D,F;
    fgets(variabel,10,f);
    printf("variabel yang digunakan adalah %s",variabel);

	// membaca matriks pada file dan memindahkan pada program
	char buffer[BUFFER_SIZE], *token;
    int mat[matsize][matsize];
	int i = 0, j = 0;
	while (fgets(buffer, BUFFER_SIZE, f))
	{
		j = 0;
		token = strtok(buffer, ",");
		while (token != NULL)
		{
			mat[i][j] = atoi(token);
			token = strtok(NULL, ",");
			printf("%d",mat[i][j]);
			j++;
		}
		printf("\n");
		i++;
    }
	// membuat matriks lebih sederhara
	
	fclose(f);
	return 0;
}

//richard here
