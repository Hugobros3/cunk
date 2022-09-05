#version 110

//out vec4 outputColor;
#define outputColor gl_FragData[0]

//uniform vec4 myColor;
//uniform sampler2D textureSampler;
//in vec2 texCoordPassed;

void main() {
    //outputColor = texture(textureSampler, texCoordPassed);
    outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}
