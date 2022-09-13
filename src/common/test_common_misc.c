#include "common_private.h"
#include <assert.h>

int main() {
    assert(swap_endianness(8, 0x0011223344556677) == 0x7766554433221100 && "");
    return 0;
}
