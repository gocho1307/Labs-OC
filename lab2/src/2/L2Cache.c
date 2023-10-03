#include "L2Cache.h"

unsigned char L1Cache[L1_SIZE];
Cache L1_Cache;
unsigned char L2Cache[L2_SIZE];
Cache L2_Cache;
unsigned char DRAM[DRAM_SIZE];
unsigned int time;

/**************** Time Manipulation ***************************/
void resetTime() { time = 0; }

unsigned int getTime() { return time; }

/**************** RAM memory (byte addressable) ***************/
void accessDRAM(int address, unsigned char *data, int mode) {
    if (address >= DRAM_SIZE - WORD_SIZE + 1) {
        exit(-1);
    }

    if (mode == MODE_READ) {
        memcpy(data, &(DRAM[address]), BLOCK_SIZE);
        time += DRAM_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&(DRAM[address]), data, BLOCK_SIZE);
        time += DRAM_WRITE_TIME;
    }
}

/**************** General *************************************/
void initCache() {
    /* Init L1 Cache */
    L1_Cache.line = (CacheLine *)malloc(256 * sizeof(CacheLine));
    if (L1_Cache.line == NULL) {
        exit(-1);
    }
    for (int i = 0; i <= 256; i++) {
        L1_Cache.line[i].Valid = 0;
    }
    L1_Cache.init = 1;

    /* Init L2 Cache */
    L2_Cache.line = (CacheLine *)malloc(512 * sizeof(CacheLine));
    if (L2_Cache.line == NULL) {
        exit(-1);
    }
    for (int i = 0; i <= 512; i++) {
        L2_Cache.line[i].Valid = 0;
    }
    L2_Cache.init = 1;
}

/**************** L1 Cache ************************************/
void accessL1(int address, unsigned char *data, int mode) {
    unsigned int Offset = address & 0x03F;
    unsigned int Index = (address >> 6) & 0x0FF;
    unsigned int Tag = address >> 14;

    unsigned char TempBlock[BLOCK_SIZE];

    CacheLine *Line = &(L1_Cache.line[Index]);

    /* Checks for cache miss */
    if (!Line->Valid || Line->Tag != Tag) {      // if block not present - miss
        accessL2(address, TempBlock, MODE_READ); // get new block from DRAM

        if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
            accessL2(address, &(L1Cache[Index * BLOCK_SIZE]),
                     MODE_WRITE); // then write back old block
        }

        memcpy(&(L1Cache[Index * BLOCK_SIZE]), TempBlock,
               BLOCK_SIZE); // copy new block to cache line
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0;
    } // if miss, then replaced with the correct block

    /* Actually access the Cache */
    if (mode == MODE_READ) { // read data from cache line
        memcpy(&(L1Cache[Index * BLOCK_SIZE + Offset]), data, WORD_SIZE);
        time += L1_READ_TIME;
    } else if (mode == MODE_WRITE) { // write data from cache line
        memcpy(data, &(L1Cache[Index * BLOCK_SIZE + Offset]), WORD_SIZE);
        time += L1_WRITE_TIME;
        Line->Dirty = 1;
    }
}

/**************** L2 Cache ************************************/
void accessL2(int address, unsigned char *data, int mode) {
    unsigned int Offset = address & 0x03F;
    unsigned int Index = (address >> 6) & 0x01FF;
    unsigned int Tag = address >> 15;

    unsigned int MemAddress = address - Offset;
    unsigned char TempBlock[BLOCK_SIZE];

    CacheLine *Line = &(L2_Cache.line[Index]);

    /* Checks for cache miss */
    if (!Line->Valid || Line->Tag != Tag) { // if block not present - miss
        accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

        if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
            accessDRAM(MemAddress, &(L2Cache[Index * BLOCK_SIZE]),
                       MODE_WRITE); // then write back old block
        }

        memcpy(&(L2Cache[Index * BLOCK_SIZE]), TempBlock,
               BLOCK_SIZE); // copy new block to cache line
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0;
    } // if miss, then replaced with the correct block

    /* Actually access the Cache */
    if (mode == MODE_READ) { // read data from cache line
        memcpy(&(L2Cache[Index * BLOCK_SIZE + Offset]), data, WORD_SIZE);
        time += L2_READ_TIME;
    } else if (mode == MODE_WRITE) { // write data from cache line
        memcpy(data, &(L2Cache[Index * BLOCK_SIZE + Offset]), WORD_SIZE);
        time += L2_WRITE_TIME;
        Line->Dirty = 1;
    }
}

/**************** Interfaces **********************************/
void read(int address, unsigned char *data) {
    accessL1(address, data, MODE_READ);
}

void write(int address, unsigned char *data) {
    accessL1(address, data, MODE_WRITE);
}
