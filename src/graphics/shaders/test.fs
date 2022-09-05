#version 110

varying vec2 texCoord;
#define outputColor gl_FragData[0]

float checker(vec2 input) {
    vec2 ic = floor(input * vec2(16.0));
    return float(mod(ic.x + ic.y, 2.0));
}

void main() {
    outputColor = vec4(vec3(1.0, 0.0, 1.0) * checker(texCoord), 1.0);
}
