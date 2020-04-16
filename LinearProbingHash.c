#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include "../Testovac/helper.h"

/////////////////////////////////////////////////////////
//PREVZATA & UPRAVENA IMPLEMENTACIA LINEAR PROBING HASH//
//Zdroj: https://github.com/soasme/Practise/blob/68b07fd91cd3106dd19bbc43144a5c7d95f18d2e/Algorithms/3.4/linear_probing.c
/////////////////////////////////////////////////////////
//ZMENIL SOM TO ZE AKO KEY SA NEPOUZIVA STRING ALE JE////
//////////TO SAMOTNA VALUE KTORU VKLADAM.////////////////
/////////////////////////////////////////////////////////



typedef struct {
    int* values;
    int size;
} LinearProbing;

//inicializacia tabulky
LinearProbing* linear_probing_init(int size) {
    LinearProbing* table = (LinearProbing*)malloc(sizeof(LinearProbing));
    table->size = size;
    table->values = (int*)calloc(size, sizeof(int));
    return table;
}

//uvolnenie tabulky
void linear_probing_free(LinearProbing* table) {
    free(table->values);
    free(table);
}

//hashovanie hodnot
int linear_probing_hash(LinearProbing* table, int key) {
    // ref: https://gist.github.com/soasme/6109366
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;
    //uprava z table->size na 7, preto�e inak to hashovanie trvalo neskuto�ne dlho..
    //a t�m mysl�m 50k prvkov aj 2 minuty
    for (i = 0; i < 7; i++) {
        hash = (hash << 4) + key;
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

//zmena velkosti tabulky
LinearProbing* linear_probing_resize(LinearProbing* table, int size) {
    LinearProbing* new_table = linear_probing_init(size);
    int i;
    for (i = 0; i < table->size; i++) {
        if (table->values[i]) {
            int new_index = linear_probing_hash(new_table, table->values[i]) % size;
            new_table->values[new_index] = table->values[i];
        }
    }
    printf("Zmena velkosti tabulky z %d na %d\n", table->size, size);
    linear_probing_free(table);
    
    return new_table;
}

//vratim 1 ak nastala kolizia, 2 ak uz tam prvok je
int linear_probing_set(LinearProbing* table, int value) {
    int start = linear_probing_hash(table, value) % table->size;
    int index = start;
    int returnVal = 0;
    
    //iterujem tabulkou kym sa nevratim na zaciatok
    while (1) {
        if (table->values[index] == 0) {
            table->values[index] = value;
            //printf("vlozene %d na %d\n", value, index);
            return returnVal;
        }
        //ignorujem rovnake hodnoty
        if (table->values[index] == value) {
            return 2;
        }
        returnVal = 1;
        index = (index + 1) % table->size;
        if (index == start) {
            //plna tabulka
            printf("tabulka je plna\n");
            break;
        }
    }
    return -1;
}

int linear_probing_get(LinearProbing* table, int value) {
    int start = linear_probing_hash(table, value) % table->size;
    int index = start;

    //iterujem tabulkou kym sa nevratim na zaciatok
    while (1) {
        if (!table->values[index]) {
            //na pozicii je 0 cize je prazdne
            //printf("prvok sa nensiel\n");
            return -1;
        }

        if (table->values[index] == value) {
            //nasiel som hodnotu
            return table->values[index];
        }

        index = (index + 1) % table->size;
        if (index == start) {
            //presiel som tabulku
            //printf("prvok sa nensiel\n");
            break;
        }
    }
    return -1;
}

//vyspis ktory nepouzivam
void linear_probing_print(LinearProbing* table) {
    int i;
    printf("[table begin]\n");
    for (i = 0; i < table->size; i++) {
        if (table->values[i]) {
            printf("[table] [index %d] hodnota: %d\n", i, table->values[i]);
        }
    }
    printf("[table end]\n");
}


int main() {
    LinearProbing *tabulka;
    //inicializacia tabulky
    tabulka = linear_pribing_init(500);
    //vkladanie
    linear_probing_set(tabulka, 10);
    //hladanie
    linear_probing_get(tabulka, 10);
    //resize tabulky
    tabulka = linear_probing_resize(tabulka,1000);

   
    return 0;
}



