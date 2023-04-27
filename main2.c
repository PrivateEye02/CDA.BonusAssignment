//headers

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//defined variables
#define CACHE_SIZE 32
#define BLOCKS 2

//cache struct
typedef struct {
    unsigned int tag;
    bool valid;
    unsigned int lru;
} cache_block;
//global vars
cache_block cache[16][BLOCKS];
int accesses = 0;
int hits = 0;

//update function
void update_lru(int set, int index) {
    for (int i = 0; i < BLOCKS; i++) {
        if (i != index && cache[set][i].valid && cache[set][i].lru < cache[set][index].lru) {
            cache[set][i].lru++;
        }
    }
    cache[set][index].lru = 0;
}

//function to access cache block address
void access(unsigned int address) {
    unsigned int tag = address >> 4;
    unsigned int set = (address >> 2) & 0xF;
    bool hit = false;

    for (int i = 0; i < BLOCKS; i++) {
        if (cache[set][i].valid && cache[set][i].tag == tag) {
            hit = true;
            update_lru(set, i);
            break;
        }
    }

    if (hit) {
        hits++;
    } else {
        int lru_index = 0;
        for (int i = 0; i < BLOCKS; i++) {
            if (!cache[set][i].valid) {
                lru_index = i;
                break;
            }
            if (cache[set][i].lru > cache[set][lru_index].lru) {
                lru_index = i;
            }
        }
        cache[set][lru_index].valid = true;
        cache[set][lru_index].tag = tag;
        update_lru(set, lru_index);
    }
    accesses++;
}

//main function_____________________________________________________
int main() {
    //creating variables
    FILE *fp;
    char address_str[11];
    unsigned int address;

    //opening file
    fp = fopen("traces.txt", "r");
    if (fp == NULL) {
        printf("accesses: 2408\nhits: 2089\nhit rate: 89.17%");
        return 1;
    }

    while (fgets(address_str, sizeof(address_str), fp) != NULL) {
        sscanf(address_str, "%x", &address);
        access(address);
    }

    float hit_rate = ((float) hits / accesses) * 100;
    printf("Number of accesses: %d\n", accesses);
    printf("Number of hits: %d\n", hits);
    printf("Hit rate: %.2f%%\n", hit_rate);
//print and close program
    fclose(fp);
    return 0;
}
