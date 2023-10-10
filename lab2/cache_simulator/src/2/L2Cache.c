#include "L2Cache.h"

uint8_t L1Cache[L1_SIZE];
Cache L1_Cache;
uint8_t L2Cache[L2_SIZE];
Cache L2_Cache;
uint8_t DRAM[DRAM_SIZE];
uint32_t time;

/**************** Time Manipulation ***************************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/**************** RAM memory (byte addressable) ***************/
void accessDRAM(int address, uint8_t *data, int mode) {
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
void accessL1(int address, uint8_t *data, int mode) {
    uint32_t Offset = address & 0x03F;
    uint32_t Index = (address >> 6) & 0x0FF;
    uint32_t Tag = address >> 14;

    uint8_t TempBlock[BLOCK_SIZE];

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
void accessL2(int address, uint8_t *data, int mode) {
    uint32_t Offset = address & 0x03F;
    uint32_t Index = (address >> 6) & 0x01FF;
    uint32_t Tag = address >> 15;

    uint32_t MemAddress = address - Offset;
    uint8_t TempBlock[BLOCK_SIZE];

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
void read(int address, uint8_t *data) { accessL1(address, data, MODE_READ); }

void write(int address, uint8_t *data) { accessL1(address, data, MODE_WRITE); }
