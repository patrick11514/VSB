#version 430

layout(location=0) in vec3 vp;

uniform mat4 modelMatrix;

out VS_OUT {
    vec3 worldPos;
} vs_out;

// Task 7: pass world-space positions to the shadow-volume geometry shader.
void main() {
    vs_out.worldPos = (modelMatrix * vec4(vp, 1.0)).xyz;
    gl_Position = vec4(vs_out.worldPos, 1.0);
}
