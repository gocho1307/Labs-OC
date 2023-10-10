#include "L2Cache_2W.h"

uint32_t time;
uint8_t DRAM[DRAM_SIZE];
CacheL2 L2[L2_N_LINES][ASSOC_L2];
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
void initCaches() {
    /* initL2 */
    for (int i = 0; i < L2_N_LINES; i++) {
        for (int j = 0; j < ASSOC_L2; j++) {
            L2[i][j].Valid = 0;
            L2[i][j].Dirty = 0;
            L2[i][j].Tag = 0;
            L2[i][j].Time = 0;
            for (int k = 0; k < BLOCK_SIZE; k += WORD_SIZE) {
                L2[i][j].Data[k] = 0;
            }
        }
    }

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

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t tag, index, offset;

    tag = address / (L2_N_LINES * BLOCK_SIZE);
    index = (address / BLOCK_SIZE) % L2_N_LINES;
    offset = address % BLOCK_SIZE;

    int found = 0;
    int i = 0;
    while (i < ASSOC_L2 && !found) {
        if (L2[index][i].Valid && L2[index][i].Tag == tag) {
            found = 1;
        } else {
            i++;
        }
    }

    /* In case it isn't found */
    if (!found) {
        i = 0;
        while (i < ASSOC_L2 && L2[index][i].Valid) {
            i++;
        }
        if (i == ASSOC_L2) {
            i = 0;
            uint32_t min = L2[index][0].Time;
            for (int j = 1; j < ASSOC_L2; j++) {
                if (L2[index][j].Time < min) {
                    min = L2[index][j].Time;
                    i = j;
                }
            }
        }
        if (L2[index][i].Dirty) {
            accessDRAM(L2[index][i].Tag * L2_N_LINES * BLOCK_SIZE +
                           index * BLOCK_SIZE,
                       L2[index][i].Data, MODE_WRITE);
            L2[index][i].Data[0] = 0;
            L2[index][i].Data[WORD_SIZE] = 0;
        }

        accessDRAM(address - offset, L2[index][i].Data, MODE_READ);

        L2[index][i].Valid = 1;
        L2[index][i].Dirty = 0;
        L2[index][i].Tag = tag;
        L2[index][i].Time = time;

        if (mode == MODE_READ) {
            memcpy(data, &(L2[index][i].Data), BLOCK_SIZE);
            time += L2_READ_TIME;
        }
    } else {
        if (mode == MODE_READ) {
            memcpy(data, &(L2[index][i].Data), BLOCK_SIZE);
            time += L1_READ_TIME;
            L2[index][i].Time = time;
        } else if (mode == MODE_WRITE) {
            memcpy(&(L2[index][i].Data), data, BLOCK_SIZE);
            time += L1_WRITE_TIME;
            L2[index][i].Dirty = 1;
            L2[index][i].Time = time;
        }
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
            accessL2(L1[index].Tag * L1_N_LINES * BLOCK_SIZE +
                         index * BLOCK_SIZE,
                     L1[index].Data, MODE_WRITE);
            L1[index].Data[0] = 0;
            L1[index].Data[WORD_SIZE] = 0;
        }

        accessL2(address - offset, L1[index].Data, MODE_READ);
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
