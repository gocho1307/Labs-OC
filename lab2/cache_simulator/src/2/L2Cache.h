#ifndef __L2CACHE_H__
#define __L2CACHE_H__

#include "Cache.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************** Time Manipulation ***************************/
void resetTime();

uint32_t getTime();

/**************** RAM memory (byte addressable) ***************/
void accessDRAM(int, uint8_t *, int);

/**************** Cache ***************************************/
void initCache();
void accessL1(int, uint8_t *, int);
void accessL2(int, uint8_t *, int);

typedef struct CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
} CacheLine;

typedef struct Cache {
    int init;
    CacheLine *line;
} Cache;

/**************** Interfaces **********************************/
void read(int, uint8_t *);

void write(int, uint8_t *);

#endif // __L2CACHE_H__
