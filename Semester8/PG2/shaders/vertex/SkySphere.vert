#version 430

layout(location = 0) in vec3 vp;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 localPos;

// Task 8: remove camera translation so the sky sphere stays at infinity.
void main() {
    localPos = vp;

    mat4 rotView = mat4(mat3(viewMatrix));
    vec4 clipPos = projectionMatrix * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}