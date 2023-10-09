#include "L2Cache_two_way.h"

unsigned char L1Cache[L1_SIZE];
Cache L1_Cache;
unsigned char L2Cache[L2_SIZE];
AssociativeCache L2_Cache;
unsigned char DRAM[DRAM_SIZE];
unsigned int time;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

unsigned int getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(int address, unsigned char *data, int mode) {

    if (address >= DRAM_SIZE - WORD_SIZE + 1)
        exit(-1);

    if (mode == MODE_READ) {
        memcpy(data, &(DRAM[address]), BLOCK_SIZE);
        time += DRAM_READ_TIME;
    }

    if (mode == MODE_WRITE) {
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
    L2_Cache.line = (AssociativeCacheLine *)malloc(256 * sizeof(CacheLine));
    if (L2_Cache.line == NULL) {
        exit(-1);
    }
    for (int i = 0; i <= 512; i++) {
        L2_Cache.line[i].Valid = 0;
    }
    L2_Cache.init = 1;
}

/*********************** L1 cache *************************/

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
    unsigned int Index = (address >> 6) & 0x0FF;
    unsigned int Tag = address >> 14;
    short int LineAccessed;

    unsigned int MemAddress = address - Offset;
    unsigned char TempBlock[BLOCK_SIZE];

    AssociativeCacheLine *LineOne = &(L2_Cache.line[Index]);
    AssociativeCacheLine *LineTwo = &(L2_Cache.line[Index + 1]);

    if (LineOne->Valid && LineOne->Tag == Tag) { // hit on line one

        hitHandler(mode, data, Index, Offset, LineOne);
        LineAccessed = 1;

    } else if (LineTwo->Valid && LineTwo->Tag == Tag) { // hit on line two

        hitHandler(mode, data, Index + 1, Offset, LineTwo);
        LineAccessed = 2;

    } else if (!LineOne->Valid) { //  line one is still unnocupied

        missHandler(MemAddress, TempBlock, Index, Tag, LineOne);
        LineAccessed = 1; // so we can update access

    } else if (!LineTwo->Valid) { // line two is still unnocupied

        missHandler(MemAddress, TempBlock, Index + 1, Tag, LineTwo);
        LineAccessed = 2;

    } else {  // miss, both lines are occupied and none match

        // Check which one was last accessed
        if (LineOne->Access > LineTwo->Access) {
            missHandler(MemAddress, TempBlock, Index, Tag, LineOne);
            LineAccessed = 1;
        } else {
            missHandler(MemAddress, TempBlock, Index + 1, Tag, LineTwo);
            LineAccessed = 2;
        }
    }

    if (LineAccessed == 1) { // update access values, so we can use LRU
        LineOne->Access = 0; // 0 means most recently used
        LineTwo->Access = 1; // 1 means least recently used
    } else {
        LineTwo->Access = 0; 
        LineOne->Access = 1; 
    }
}

void missHandler(unsigned int MemAddress, unsigned char *TempBlock,
                 unsigned int Index, unsigned int Tag,
                 AssociativeCacheLine *Line) {
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
    }
}

void hitHandler(int mode, unsigned char *data, unsigned int Index,
                unsigned int Offset, AssociativeCacheLine *Line) {
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
