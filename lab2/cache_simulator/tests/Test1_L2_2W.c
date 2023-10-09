#include "../src/3/L2Cache_two_way.h"
#include <stdint.h>

int main() {
    uint32_t clock;

    uint32_t TestData[30];

    for (int i = 0; i < 30; i++) {
        TestData[i] = i;
    }

    resetTime();
    initCache();

    for (int i = 1; i < 29; i++) {
        write(i, (uint8_t *)&TestData[i]);

        clock = getTime();
        printf("Time: %d\n", clock);

        read(1, (uint8_t *)&TestData[i - 1]);

        clock = getTime();
        printf("Time: %d\n", clock);
    }

    return 0;
}