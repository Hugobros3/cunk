#include "cunk/memory.h"
#include "cunk/nbt.h"
#include "cunk/print.h"

#include "../common/common_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static int64_t swap_endianness(int bytes, int64_t i) {
    int64_t acc = 0;
    for (int byte = 0; byte < bytes; byte++)
        acc |= ((i >> byte * 8) & 0xFF) << (bytes - 1 - byte) * 8;
    return acc;
}

int main(int argc, char** argv) {
    assert(swap_endianness(8, 0x0011223344556677) == 0x7766554433221100 && "");

    char* buf;
    size_t buf_size;
    printf("Using test file %s\n", argv[1]);
    if (!read_file(argv[1], &buf_size, &buf))
        return 1;

    Arena* arena = cunk_new_arena();
    NBT_Object* o = cunk_decode_nbt(buf_size, buf, arena);
    assert(o);
    Printer* p = cunk_open_file_as_printer(stdout);
    cunk_print_nbt(p, o);
    free(buf);
    cunk_arena_destroy(arena);
    return 0;
}
