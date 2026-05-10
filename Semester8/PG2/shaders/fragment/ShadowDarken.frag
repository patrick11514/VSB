#version 430

uniform float shadowOpacity;

out vec4 fragColor;

// Task 7: apply the final shadow opacity over the stencil mask.
void main() {
    fragColor = vec4(0.0, 0.0, 0.0, shadowOpacity);
}
