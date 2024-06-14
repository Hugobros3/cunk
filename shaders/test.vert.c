#include <stdint.h>
#include <stdbool.h>
#include <shady.h>

input vec3 vertexIn;
input vec2 texCoordIn;

uniform_constant mat4 myMatrix;

location(0) output vec2 texCoord;

vertex_shader
void main() {
    vec4 v;
    v.xyz = vertexIn;
    v.w = 1.0;
    gl_Position = mul_mat4f_vec4f(myMatrix, v);
    texCoord = texCoordIn;
}
