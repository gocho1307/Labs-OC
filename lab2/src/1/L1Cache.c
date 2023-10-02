#include "L1Cache.h"

unsigned char L1Cache[L1_SIZE];
unsigned char DRAM[DRAM_SIZE];
unsigned int time;
Cache SimpleCache;

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

/**************** L1 cache ************************************/
void initCache() {
    for (int i = 0; i <= L1_LINE_NUM; i++) {
        SimpleCache.line[i].Valid = 0;
    }
    SimpleCache.init = 1;
}

unsigned int getOffset(int address) { return address & 0x01; }

unsigned int getIndex(int address) { return address & 0x01E; }

unsigned int getTag(int address) { return address >> 5; }

void accessL1(int address, unsigned char *data, int mode) {
    unsigned int Offset = getOffset(address);
    unsigned int Index = getIndex(address);
    unsigned int Tag = getTag(address);

    unsigned int MemAddress = address - Offset;
    unsigned char TempBlock[BLOCK_SIZE];

    CacheLine *Line = &(SimpleCache.line[Index]);

    /* Checks for cache miss */
    if (!Line->Valid || Line->Tag != Tag) { // if block not present - miss
        accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

        if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
            accessDRAM(MemAddress, &(L1Cache[Index * BLOCK_SIZE]),
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
        memcpy(&(L1Cache[Index * BLOCK_SIZE + Offset * WORD_SIZE]), data,
               WORD_SIZE);
        time += L1_READ_TIME;
    } else if (mode == MODE_WRITE) { // write data from cache line
        memcpy(data, &(L1Cache[Index * BLOCK_SIZE + Offset * WORD_SIZE]),
               WORD_SIZE);
        time += L1_WRITE_TIME;
        Line->Dirty = 1;
    }
}

void read(int address, unsigned char *data) {
    accessL1(address, data, MODE_READ);
}

void write(int address, unsigned char *data) {
    accessL1(address, data, MODE_WRITE);
}
