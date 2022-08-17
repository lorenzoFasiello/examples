#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct
{
    char *bene;
    char EU;
    int quantita;
}comando;

typedef struct
{
    char *bene;
    int quantita;
}merce;

typedef struct
{
    comando *array;
    size_t used; //nIndex
    size_t size; //dimensioneArray
}arrayComando;
typedef struct
{
    merce *array;
    size_t used; //nIndex
    size_t size; //dimensioneArray
}arrayMerce;


void initArrayComado(arrayComando *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(comando));
    a->used = 0;
    a->size = initialSize;
}

void insertArrayComando (arrayComando *a, comando element)
{
    if (a->used == a->size) {
        a->size += 1;
        a->array = realloc(a->array, a->size * sizeof(comando));
    }
    a->array[a->used++] = element;
}


void removeElementByIndexComando(arrayComando *a, int index)
{
    arrayComando temp_array;
    initArrayComado(&temp_array, 1);

    for (int i=0; i<a->size; i++) // scorro l'array a
    {
        if (i==index) //Quando incontro index lo salto (continue)
        {
            continue;
        }
        insertArrayComando(&temp_array, a->array[i]);
    }
    a->array = temp_array.array;
    a->size = temp_array.size;
    a->used = temp_array.used;
}

void initArrayMerce(arrayMerce *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(merce));
    a->used = 0;
    a->size = initialSize;
}

void insertArrayMerce (arrayMerce *a, merce element)
{
    if (a->used == a->size) {
        a->size += 1;
        a->array = realloc(a->array, a->size * sizeof(merce));
    }
    a->array[a->used++] = element;
}


void removeElementByIndexMerce(arrayMerce *a, int index)
{
    arrayMerce temp_array;
    initArrayMerce(&temp_array, 1);

    for (int i=0; i<a->size; i++) // scorro l'array a
    {
        if (i==index) //Quando incontro index lo salto (continue)
        {
            continue;
        }
        insertArrayMerce(&temp_array, a->array[i]);
    }
    a->array = temp_array.array;
    a->size = temp_array.size;
    a->used = temp_array.used;
}


void freeArrayComado(arrayComando *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void freeArrayMerce(arrayMerce *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void printArrayComando(arrayComando *a){
    for(int k=0; k < a->size; k++)
    {
        printf ("nomeOggetto: %s E/U: %c quantita: %d \n", a->array[k].bene, a->array[k].EU, a->array[k].quantita);
    }
}

void printArrayMerce(arrayMerce *a){
    for(int k=0; k < a->size; k++)
    {
        printf ("nomeOggetto: %s quantita: %d \n", a->array[k].bene, a->array[k].quantita);
    }
}

int get_arg(char* arg, int* val) //mi permette di verificare se x e y siano 2 interi
{
    char* check;
    int num;
    num = strtol(arg, &check, 10);
    if((check && strlen(check) != 0) || num < 0) //strlen mi serve a calcolare la lunghezza di una stringa, escluso il carattere terminatore.
    {
        printf("Gli argomenti devono essere interi e positivi");
        return 1;
    }
    *val = num;
    return 0;
}
arrayMerce readAndSetArrayFromFile(FILE *file){
    arrayMerce list;
    initArrayMerce(&list, 1);
    char line[255];
    merce temp_merce;
    printf("%s",( fgets ( line, sizeof ( line), file)));
    while ( ( fgets ( line, sizeof ( line), file))) {
        if ((sscanf(line, "%s %d[^\n]", &temp_merce.bene, &temp_merce.quantita)) == 3) {
            printf("--> %s %d\n",temp_merce.bene, temp_merce.quantita);
            insertArrayMerce(&list, temp_merce);
        }
    }
    printf("size of goods: %d\n",list.size);
    return list;
}
void writeArrayIntoFile(arrayMerce *a){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );
    char *path = "c:\\mag_";
    char *format = ".txt";
    char str_file[80];
    strcat(str_file, path);
    strcat(str_file, asctime (timeinfo));
    strcat(str_file, format);
    printf("%s\n",str_file);
    FILE *fw = fopen(str_file,"w");
    if (fw == NULL)
    {
        printf("!ERROR! error to open file in writing\n");
        exit(-1);
    }
    for(int k=0; k < a->size; k++)
    {
        fprintf(fw,"%s %d\n",a->array[k].bene, a->array[k].quantita);
    }
    fclose(fw);

}

int main(int argc, char** argv)
{
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("!ERROR!\n");
        exit(-1);
    }
    printf("Aperto con successo!\n");

    arrayMerce magazzino = readAndSetArrayFromFile(fp);
    printArrayMerce(&magazzino);

//    arrayComando comandi;
//    initArrayComado(&comandi,1);
//    printf("scrivi i comandi (bene E/U qty): \n");
//    printf("per finire scrivere STOP");
//    comando temp_cmd;
//    char *cmd_stop;
//    cmd_stop = "STOP";
//    do
//    {
//        scanf("%s", &temp_cmd.bene);
//        if(temp_cmd.bene==cmd_stop)
//        {
//            break;
//        }
//        scanf(" %c %d", &temp_cmd.EU, &temp_cmd.quantita);
//        insertArrayComando(&comandi,temp_cmd);
//    }while(temp_cmd.bene!=cmd_stop);
//    printArrayComando(&comandi);
//
//    for(int i=0; i<comandi.size; i++){
//        for(int j=0;j<magazzino.size;j++){
//            if(comandi.array[i].bene == magazzino.array[j].bene){
//                switch (comandi.array[i].EU) {
//                    case 'E':
//                        magazzino.array[j].quantita += comandi.array[i].quantita;
//                        break;
//                    case 'U':
//                        magazzino.array[j].quantita -= comandi.array[i].quantita;
//                        break;
//                }
//            }
//            else if(comandi.array[i].EU=='E'){
//                merce good = {comandi.array[i].bene,comandi.array[i].quantita};
//                insertArrayMerce(&magazzino,good);
//            }
//        }
//    }
//    printArrayMerce(&magazzino);
//    writeArrayIntoFile(&magazzino);























    fclose(fp);
    exit(0);
}
