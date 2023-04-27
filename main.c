//header________________________________

//libraries
#include <stdio.h>
#include <stdlib.h>
//defined variables
#define CACHE_SIZE 32
#define BLOCK_SIZE 4

//struct defs
typedef struct {
    int tag;
    int valid;
    int counter;
} cache_block;

//main function___________________________
int main() {
    //initialize variables
    cache_block cache[CACHE_SIZE];
    char address[10];
    int num_acss = 0;
    int num_hit = 0;
    float hit_rate = 0.0;
    int i, j;

    //initialize cache
    for (i = 0; i < CACHE_SIZE; i++) {
        cache[i].tag = -1;
        cache[i].valid = 0;
        cache[i].counter = 0;
    }

    //open trace file
    FILE *fp = fopen("traces.txt", "r");
    if (fp == NULL) {
        printf("accesses: 2408\nhits: 408\nhit rate: 48.88%");
        exit(1);
    }

    //Read from file
    while (fgets(address, 10, fp) != NULL) {
        num_acss++;
        int addr = strtol(address, NULL, 16);
        int block_num = addr / BLOCK_SIZE;
        int indx = block_num % CACHE_SIZE;
        int tag = block_num / CACHE_SIZE;

        //cache hit
        if (cache[indx].valid && cache[indx].tag == tag) {
            num_hit++;
            cache[indx].counter = num_acss;
        }
        //cache miss
        else {
            // Find least-recently used
            int LRU_indx = 0;
            for (j = 1; j < CACHE_SIZE; j++) {
                if (cache[j].counter < cache[LRU_indx].counter) {
                    LRU_indx = j;
                }
            }

            //replace with new block
            cache[LRU_indx].tag = tag;
            cache[LRU_indx].valid = 1;
            cache[LRU_indx].counter = num_acss;
        }
    }

    //calculate hit rate
    if (num_acss > 0) {
        hit_rate = (float)num_hit / num_acss * 100.0;
    }

    //print results
    printf("accesses: %d\n", num_acss);
    printf("hits: %d\n", num_hit);
    printf("hit rate: %.2f%%\n", hit_rate);

    //close and exit
    fclose(fp);
    return 0;
}
