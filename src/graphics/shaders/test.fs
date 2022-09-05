#version 110

#define outputColor gl_FragData[0]

void main() {
    outputColor = vec4(1.0, noise1(gl_FragCoord.xy / 256.0), 1.0, 1.0);
}
