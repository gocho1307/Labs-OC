#include "L1Cache.h"

uint32_t time;
uint8_t DRAM[DRAM_SIZE];
CacheL1 L1[L1_N_LINES];

/**************** Time Manipulation ****************************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/**************** RAM Memory (byte addressable) ****************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {
    if (address >= DRAM_SIZE - WORD_SIZE + 1)
        exit(-1);

    if (mode == MODE_READ) {
        memcpy(data, &(DRAM[address]), BLOCK_SIZE);
        time += DRAM_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&(DRAM[address]), data, BLOCK_SIZE);
        time += DRAM_WRITE_TIME;
    }
}

/**************** Caches ***************************************/
void initCache() {
    /* initL1 */
    for (int i = 0; i < L1_N_LINES; i++) {
        L1[i].Valid = 0;
        L1[i].Dirty = 0;
        L1[i].Tag = 0;
        for (int j = 0; j < BLOCK_SIZE; j += WORD_SIZE) {
            L1[i].Data[j] = 0;
        }
    }

    /* initDRAM */
    for (int i = 0; i < DRAM_SIZE; i += WORD_SIZE) {
        DRAM[i] = 0;
    }
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t tag, index, offset;

    tag = address / (L1_N_LINES * BLOCK_SIZE);
    index = (address / BLOCK_SIZE) % L1_N_LINES;
    offset = address % BLOCK_SIZE;

    if (L1[index].Valid && L1[index].Tag == tag) {
        if (mode == MODE_READ) {
            memcpy(data, &(L1[index].Data[offset]), WORD_SIZE);
            time += L1_READ_TIME;
        } else if (mode == MODE_WRITE) {
            memcpy(&(L1[index].Data[offset]), data, WORD_SIZE);
            time += L1_WRITE_TIME;
            L1[index].Dirty = 1;
        }
    } else {
        if (L1[index].Dirty) {
            accessDRAM(L1[index].Tag * L1_N_LINES * BLOCK_SIZE +
                           index * BLOCK_SIZE,
                       L1[index].Data, MODE_WRITE);
        }

        accessDRAM(address - offset, L1[index].Data, MODE_READ);
        if (mode == MODE_READ) {
            memcpy(data, &(L1[index].Data[offset]), WORD_SIZE);
            time += L1_READ_TIME;
            L1[index].Dirty = 0;
            L1[index].Valid = 1;
            L1[index].Tag = tag;
        } else if (mode == MODE_WRITE) {
            memcpy(&(L1[index].Data[offset]), data, WORD_SIZE);
            time += L1_WRITE_TIME;
            L1[index].Dirty = 1;
            L1[index].Valid = 1;
            L1[index].Tag = tag;
        }
    }
}

/**************** Interfaces ***********************************/
void read(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_WRITE);
}
