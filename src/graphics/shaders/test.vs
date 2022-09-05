#version 110

// attribute vec2 gl_Vertex;
#define vertexIn gl_Vertex.xy

uniform vec2 myPosition;

void main() {
    gl_Position = vec4(vertexIn + myPosition, 0.0, 1.0);
}
