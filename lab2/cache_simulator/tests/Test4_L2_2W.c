#include "../src/3/L2Cache_2W.h"
#include <stdint.h>

int main() {
    uint32_t clock;
    int value;

    resetTime();
    initCaches();

    // Check if write/read adress match
    for (int i = 4; i < 257; i = i * 4) {
        write(i, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i, i, clock);
    }

    // Writes again to see if it's going to update
    int i = 2;
    write(16, (unsigned char *)(&i));

    for (int i = 256; i > 1; i = i / 4) {
        read(i, (unsigned char *)(&value));
        clock = getTime();
        printf("Read; Address %d; Value %d; Time %d\n", i, value, clock);
    }

    return 0;
}
