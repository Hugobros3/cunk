#version 110

// attribute vec2 gl_Vertex;
#define vertexIn gl_Vertex.xyz

uniform mat4 myMatrix;

void main() {
    gl_Position = myMatrix * vec4(vertexIn, 1.0);
}
