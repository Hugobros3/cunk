#version 110

attribute vec3 vertexIn;
attribute vec2 texCoordIn;
//#define vertexIn gl_Vertex.xyz

uniform mat4 myMatrix;

varying vec2 texCoord;

void main() {
    gl_Position = myMatrix * vec4(vertexIn, 1.0);
    texCoord = texCoordIn;
}
