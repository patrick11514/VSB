#version 430

uniform float debugOpacity;

out vec4 fragColor;

void main() {
    // Bright magenta overlay to reveal stencil-marked shadowed pixels.
    fragColor = vec4(1.0, 0.0, 1.0, debugOpacity);
}
