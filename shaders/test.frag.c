#include <stdint.h>
#include <stdbool.h>
#include <shady.h>

location(0) output vec4 fragColor;
location(0) input vec2 texCoord;

fragment_shader
void main() {
    //fragColor = (vec4) { texCoord.x, texCoord.y, 1.0f, 1.0f };
    bool fillx = (((int) (texCoord[0] * 16.0f))) % 2 == 1;
    bool filly = (((int) (texCoord[1] * 16.0f))) % 2 == 1;
    bool fill = fillx ^ filly;
    if (fill)
      fragColor = (vec4) {1.0f, 1.0f, 0.0f, 1.0f };
    else
      fragColor = (vec4) { 0.0f, 0.0f, 0.0f, 1.0f };
}
