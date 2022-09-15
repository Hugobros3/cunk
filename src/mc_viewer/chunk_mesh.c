#include "chunk.h"
#include "cunk/memory.h"

#define CUBE(V) \
V(-1, -1, -1,   0, 0) \
V(-1,  1,  1,   1, 1) \
V(-1,  1, -1,   0, 1) \
V(-1,  1,  1,   1, 1) \
V(-1, -1, -1,   0, 0) \
V(-1, -1,  1,   1, 0) \
\
V(-1, -1,  1,   0, 0) \
V(1,  -1,  1,   1, 0) \
V(1,   1,  1,   1, 1) \
V(-1, -1,  1,   0, 0) \
V(1,   1,  1,   1, 1) \
V(-1,  1,  1,   0, 1) \
\
V(1,  -1, -1,   1, 0) \
V(1,   1, -1,   1, 1) \
V(1,   1,  1,   0, 1) \
V(1,  -1, -1,   1, 0) \
V(1,   1,  1,   0, 1) \
V(1,  -1,  1,   0, 0) \
\
V(-1, -1, -1,   1, 0) \
V(1,   1, -1,   0, 1) \
V(1,  -1, -1,   0, 0) \
V(-1, -1, -1,   1, 0) \
V(-1,  1, -1,   1, 1) \
V(1,   1, -1,   0, 1) \
\
V(-1,  1, -1,   0, 1) \
V(1,   1,  1,   1, 0) \
V(1,   1, -1,   1, 1) \
V(-1,  1, -1,   0, 1) \
V(-1,  1,  1,   0, 0) \
V(1,   1,  1,   1, 0) \
\
V(-1, -1, -1,   0, 0) \
V(1,  -1, -1,   1, 0) \
V(1,  -1,  1,   1, 1) \
V(-1, -1, -1,   0, 0) \
V(1,  -1,  1,   1, 1) \
V(-1, -1,  1,   0, 1) \

static int cubeData[] = {
#define V(x, y, z, t, s) x, y, z, t, s,
CUBE(V)
#undef V
};

static void paste_cube(Growy* g, unsigned x, unsigned y, unsigned z) {
    float tmp[5];
#define V(cx, cy, cz, t, s) tmp[0] = (int) ((cx + 1) / 2) + (float) x; tmp[1] = (int) ((cy + 1) / 2) + (float) y; tmp[2] = (int) ((cz + 1) / 2) + (float) z; tmp[3] = t; tmp[4] = s; cunk_growy_append_bytes(g, sizeof(tmp), (char*) tmp);
    CUBE(V)
#undef V
}

#include <stdio.h>

void chunk_mesh(const Chunk* chunk, Growy* g, size_t* num_verts) {
    *num_verts = 0;
    for (unsigned section = 0; section < CUNK_CHUNK_SECTIONS_COUNT; section++) {
        for (unsigned x = 0; x < CUNK_CHUNK_SIZE; x++)
            for (unsigned y = 0; y < CUNK_CHUNK_SIZE; y++)
              for (unsigned z = 0; z < CUNK_CHUNK_SIZE; z++) {

                  unsigned y2 = y + section * CUNK_CHUNK_SIZE;
                  // fprintf(stderr, "%d %d %d %d\n", x, y, z, y2);
                  if (chunk_get_block_data(chunk, x, y2, z) != air_data) {
                      paste_cube(g, x, y2, z);
                      *num_verts += 36;
                  }
              }
    }
}
