#ifndef __L2CACHE_TWO_WAY_H__
#define __L2CACHE_TWO_WAY_H__

#include "../Cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************** Time Manipulation ***************************/
void resetTime();

unsigned int getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(int, unsigned char *, int);

/*********************** Cache *************************/

typedef struct CacheLine {
    unsigned char Valid;
    unsigned char Dirty;
    unsigned int Tag;
} CacheLine;

typedef struct AssociativeCacheLine {
    unsigned char Valid;
    unsigned char Dirty;
    unsigned int Tag;
    short int Access;
} AssociativeCacheLine;

typedef struct Cache {
    int init;
    CacheLine *line;
} Cache;

typedef struct AssociativeCache {
    int init;
    AssociativeCacheLine *line;
} AssociativeCache;

void initCache();
void accessL1(int, unsigned char *, int);
void accessL2(int, unsigned char *, int);
void missHandler(unsigned int, unsigned char *, unsigned int, unsigned int,
                 AssociativeCacheLine *);
void hitHandler(int, unsigned char *, unsigned int, unsigned int,
                AssociativeCacheLine *);

/*********************** Interfaces *************************/

void read(int, unsigned char *);

void write(int, unsigned char *);

#endif // __L2CACHE_TWO_WAY_H__
