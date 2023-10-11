#include "../src/3/L2Cache_2W.h"
#include <stdint.h>

int main() {
    uint32_t clock;
    uint32_t val;
    uint32_t TestData[30];

    for (int i = 0; i < 30; i++) {
        TestData[i] = i;
    }

    resetTime();
    initCaches();

    /* Loads a block and then causes a miss */
    /* After that it reads from the original block again */
    clock = getTime();
    printf("Time: %d\n", clock);

    read(1, (uint8_t *)&val);
    read(16384, (uint8_t *)&val);

    clock = getTime();
    printf("Time: %d\n", clock);

    read(1, (uint8_t *)&val);

    clock = getTime();
    printf("Time: %d\n", clock);

    printf("-----------------\n");

    for (int i = 1; i < 29; i++) {
        write(i, (uint8_t *)&TestData[i]);

        clock = getTime();
        printf("Time: %d\n", clock);

        read(i, (uint8_t *)&val);

        clock = getTime();
        printf("Value: %d\n", val);
        printf("Time: %d\n", clock);
    }

    return 0;
}
