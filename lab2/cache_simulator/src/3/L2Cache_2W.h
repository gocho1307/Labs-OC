#ifndef __L2CACHE_2W_H__
#define __L2CACHE_2W_H__

#include "Cache.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSOC_L2 1
#define L1_N_LINES (L1_SIZE / BLOCK_SIZE)
#define L2_N_LINES (L2_SIZE / (BLOCK_SIZE * ASSOC_L2))

/**************** Time Manipulation ****************************/
void resetTime();
uint32_t getTime();

/**************** RAM Memory (byte addressable) ****************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/**************** Caches ***************************************/
void initCaches();
void accessL2(uint32_t, uint8_t *, uint32_t);
void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct CacheL2 {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
    uint32_t Time;
    uint8_t Data[L2_SIZE];
} CacheL2;

typedef struct CacheL1 {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
    uint8_t Data[L1_SIZE];
} CacheL1;

/**************** Interfaces ***********************************/
void read(uint32_t, uint8_t *);
void write(uint32_t, uint8_t *);

#endif // __SIMPLECACHE_H__
