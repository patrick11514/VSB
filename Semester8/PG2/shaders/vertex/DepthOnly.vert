#version 430

layout(location=0) in vec3 vp;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vp, 1.0);
}
