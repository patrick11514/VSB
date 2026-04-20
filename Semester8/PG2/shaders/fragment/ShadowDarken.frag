#version 430

uniform float shadowOpacity;

out vec4 fragColor;

void main() {
    fragColor = vec4(0.0, 0.0, 0.0, shadowOpacity);
}
