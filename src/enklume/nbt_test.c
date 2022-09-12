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

    Printer* p = cunk_open_file_as_printer(stdout);

    char* buf;
    size_t buf_size;
    printf("Using test file %s\n", argv[1]);
    if (!read_file(argv[1], &buf_size, &buf))
        return 1;

    if (string_ends_with(argv[1], ".dat")) {
        printf(".dat file detected, it needs decompression\n");
        printf("compressed size: %llu\n", buf_size);
        Growy* g = cunk_new_growy();
        cunk_inflate(buf_size, buf, g);
        free(buf);
        buf_size = cunk_growy_size(g);
        buf = cunk_growy_deconstruct(g);
        printf("Decompression successful\n");
    }

    Arena* arena = cunk_new_arena();
    NBT_Object* o = cunk_decode_nbt(buf_size, buf, arena);
    assert(o);
    cunk_print_nbt(p, o);
    cunk_print(p, "\nSize of NBT arena: ");
    cunk_print_size_suffix(p, cunk_arena_size(arena), 3);
    cunk_print(p, "\n");
    free(buf);
    cunk_arena_destroy(arena);
    return 0;
}
