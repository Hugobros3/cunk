#include "common_private.h"
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

static void test_bit_fetches() {
    int64_t arr[] = { 0, 1, 2, 3, 0xDEADBEEF };
    int64_t r = fetch_bits(arr, sizeof(int64_t) * 4 * CHAR_BIT, 64);
    assert(r == 0xDEADBEEF);
    r = fetch_bits(arr, sizeof(int64_t) * 4 * CHAR_BIT + 4, 60);
    assert(r == 0x0DEADBEE);
}

int main() {
    assert(swap_endianness(8, 0x0011223344556677) == 0x7766554433221100 && "");
    test_bit_fetches();
    return 0;
}
