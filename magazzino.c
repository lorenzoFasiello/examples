/*
Un'azienda deve tenere traccia dei beni presenti in un magazzino. L'utente inserisce da tastiera dei "comandi" nel se-
 -guente formato: bene EU quantità

 Dove: bene è il nome del bene; EU è la lettera 'E' per entrata, 'U' per uscita; quantità è la quantità di bene
 entrata (+) o uscita(-).

 L'utente termina il caricamento inserendo un comando pari a FINE. In tal caso il programma deve stampare le quantità
 di beni presenti a magazzino.
 Esempio:
 viti E 10
 dadi E 50
 viti U 5
 viti E 3
 FINE
*/

#include <string.h>
#include <ctype.h>
#include "stdio.h"
#include "stdlib.h"

//merce struct e funzioni
typedef struct { //oggetti nel file storage.txt
    char name[255]; //nome oggetto
    int num; //quantità dell'oggetto
} merce;

typedef struct {
    merce *array;
    size_t used;
    size_t size;
} arrayMerce;

void initArrayMerce(arrayMerce *a, size_t iniSize) {
    a->array = malloc(iniSize * sizeof(merce));
    a->used = 0;
    a->size = iniSize;
}

void insertArrayMerce(arrayMerce *a, merce element) {
    if (a->used == a->size) {
        a->size += 1;
        a->array = realloc(a->array, a->size * sizeof(merce));
    }
    a->array[a->used++] = element;
}

arrayMerce readAndSetArrayFromFile(FILE *file) {
    arrayMerce list;
    initArrayMerce(&list, 1);
    char line[100];
    merce temp_merce = {"", 0};
    while ((fgets(line, sizeof(line), file))) {
        if ((sscanf(line, "%s %d [^\n]", &temp_merce.name, &temp_merce.num)) == 2) {
            insertArrayMerce(&list, temp_merce);
        }
    }
    printf("totElementsInStorage: %d\n", list.size);
    return list;
}

arrayMerce printArrayMerce(arrayMerce *arrayMerce) {
    for (int i = 0; i < arrayMerce->size; i++) {
        printf("name: %s; num: %d;\n", arrayMerce->array[i].name, arrayMerce->array[i].num);
    }
}


//comandi struct e funzioni
typedef struct {
    char bene[255];
    char EU;
    int quantita;
} comandi;

typedef struct {
    comandi *array;
    size_t used;
    size_t size;
} arrayComandi;

void initArrayComandi(arrayComandi *arrayComandi, size_t iniSize) {
    arrayComandi->array = malloc(iniSize * sizeof(comandi));
    arrayComandi->used = 0;
    arrayComandi->size = iniSize;
}

void insertArrayComandi(arrayComandi *arrayComandi, comandi element) {
    if (arrayComandi->used == arrayComandi->size) {
        arrayComandi->size += 1;
        arrayComandi->array = realloc(arrayComandi->array, arrayComandi->size * sizeof(comandi));
    }
    arrayComandi->array[arrayComandi->used++] = element;
}

void printArrayComandi(arrayComandi *arrayComandi) {
    for (int k = 0; k < arrayComandi->size; k++) {
        printf("bene: %s; EU: %c; quantita: %d;\n", arrayComandi->array[k].bene, arrayComandi->array[k].EU,
               arrayComandi->array[k].quantita);
    }
}

//writing the result into the file
arrayMerce writeArrayIntoFile(arrayMerce *arrayMerce){
    FILE *output = fopen("output.txt", "wt");
    for (int i = 0; i < arrayMerce->size; i++){
        fprintf(output, "%s %d\n", arrayMerce->array[i].name, arrayMerce->array[i].num);
    }
    fclose(output);
}


char *stringLowerCase(char *str) {
    char *new_str;
    new_str = str;
    for (int i = 0; i < strlen(str); i++) {
        new_str[i] = tolower(str[i]);
    }
    return new_str;
}

char *stringUpperCase(char *str) {
    char *new_str;
    new_str = str;
    for (int i = 0; i < strlen(str); i++) {
        new_str[i] = toupper(str[i]);
    }
    return new_str;
}

int main() {

    //args file_input file_output?
    FILE *fp = fopen("storage.txt", "rt");


    printf("!openSuccessfully!\n");
    arrayMerce storageList = readAndSetArrayFromFile(fp);
    //printArrayMerce(&storageList);

    arrayComandi registro;
    initArrayComandi(&registro, 0);
    printf("Write for each element the following parameters: bene, E/U, quantita; (write STOP to finish);\n");
    comandi element;
    char *STOP; //to STOP
    STOP = "fine";
    do {
        scanf("%s", &element.bene);
        if (!strcmp(tolower(element.bene), STOP)) {
            break;
        }
        scanf(" %c %d", &element.EU, &element.quantita);
        insertArrayComandi(&registro, element);
    } while (strcmp(tolower(element.bene), STOP));
    //printArrayComandi(&registro);
    printf("elaborazione... \n");
    for (int i = 0; i < registro.size; i++) {
        char exist = 1;
        for (int j = 0; j < storageList.size; j++) {
            //printf("%s == %s  -> %d\n",stringLowerCase(registro.array[i].bene),stringLowerCase(storageList.array[j].name),strcmp(stringLowerCase(registro.array[i].bene),stringLowerCase(storageList.array[j].name)));

            if (!strcmp(stringLowerCase(registro.array[i].bene), stringLowerCase(storageList.array[j].name))) {
                exist = 0;
                //printf(" %c\n",tolower(registro.array[i].EU));
                switch (tolower(registro.array[i].EU)) {
                    case 'e':
                        storageList.array[j].num = storageList.array[j].num + registro.array[i].quantita;

                        //int ris =registro.array[i].quantita+ registro.array[i].quantita; //metodi alternativi
                        //registro.array[i].quantita = ris;
                        //registro.array[i].quantita += registro.array[i].quantita;
                        break;
                    case 'u':
                        storageList.array[j].num -= registro.array[i].quantita;
                        break;
                }
                break;
            }
        }
        if (exist && tolower(registro.array[i].EU) == 'e') {
            merce add_storage;
            strcpy(add_storage.name, registro.array[i].bene);
            add_storage.num = registro.array[i].quantita;
            insertArrayMerce(&storageList, add_storage);
        }
    }

    printArrayMerce(&storageList);
    writeArrayIntoFile(&storageList);


    fclose(fp);
    exit(0);

}
