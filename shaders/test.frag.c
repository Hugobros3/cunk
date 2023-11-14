#include <stdint.h>

#define location(i) __attribute__((annotate("shady::location::"#i)))
#define input       __attribute__((address_space(389)))
#define output      __attribute__((address_space(390)))

typedef float vec4     __attribute__((ext_vector_type(4)));
typedef float vec2     __attribute__((ext_vector_type(2)));

location(0) output vec4 fragColor;
location(0) input vec2 texCoord;

__attribute__((annotate("shady::entry_point::Fragment")))
void main() {
    //fragColor = (vec4) { texCoord.x, texCoord.y, 1.0f, 1.0f };
    int fillx = (((int) (texCoord[0] * 16.0f))) % 2;
    int filly = (((int) (texCoord[1] * 16.0f))) % 2;
    int fill = fillx ^ filly;
    if (fill)
      fragColor = (vec4) {1.0f, 1.0f, 0.0f, 1.0f };
    else
      fragColor = (vec4) { 0.0f, 0.0f, 0.0f, 1.0f };
}
