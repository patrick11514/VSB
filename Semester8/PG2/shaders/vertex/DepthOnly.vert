#version 430

layout(location=0) in vec3 vp;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

// Task 6: write light-space depth for the shadow map pass.
void main() {
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vp, 1.0);
}
