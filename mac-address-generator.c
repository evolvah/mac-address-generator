#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Based on http://home.thep.lu.se/~bjorn/crc/crc32_simple.c */

void mac_addr(const void *data, size_t n_bytes, uint32_t* mac_high, uint32_t* mac_low) {
    static uint32_t table[0x100];
    for (size_t i = 0; i < 0x100; ++i) {
        uint32_t r = i;
        for (int j = 0; j < 8; ++j)
            r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
        table[i] = r ^ (uint32_t)0xFF000000L;
    }

    *mac_low = 0;
    for (size_t i = 0; i < n_bytes; ++i)
        *mac_low = table[(uint8_t)*mac_low ^ ((uint8_t*)data)[i]] ^ *mac_low >> 8;

    *mac_high = 0;
    for (size_t i = 0; i < n_bytes; ++i)
        *mac_high = table[(uint8_t)*mac_high ^ ((uint8_t*)data)[n_bytes-i-1]] ^ *mac_high >> 8;
    *mac_high = *mac_high & 0xFFFF;
}

int main(int ac, char** av) {
    const uint8_t chip_id1[8] = {1,2,3,4,5,6,7,8};
    const uint8_t chip_id2[8] = {1,2,3,4,5,6,7,9};
    const uint8_t chip_id3[8] = {0,2,3,4,5,6,7,8};
    const uint8_t chip_id4[8] = {0,0,0,0,0,0,0,0};
    const uint8_t chip_id5[8] = {1,1,1,1,1,1,1,1};
    uint32_t mac_high;
    uint32_t mac_low;

    /* test 1 */
    mac_addr(chip_id1, 8, &mac_high, &mac_low);
    printf("%04x%08x\n", mac_high, mac_low);

    /* test 2 */
    mac_addr(chip_id2, 8, &mac_high, &mac_low);
    printf("%04x%08x\n", mac_high, mac_low);

    /* test 3 */
    mac_addr(chip_id3, 8, &mac_high, &mac_low);
    printf("%04x%08x\n", mac_high, mac_low);

    /* test 4 */
    mac_addr(chip_id4, 8, &mac_high, &mac_low);
    printf("%04x%08x\n", mac_high, mac_low);

    /* test 5 */
    mac_addr(chip_id5, 8, &mac_high, &mac_low);
    printf("%04x%08x\n", mac_high, mac_low);

    return 0;
}
