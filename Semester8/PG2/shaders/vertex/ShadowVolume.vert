#version 430

layout(location=0) in vec3 vp;

uniform mat4 modelMatrix;

out VS_OUT {
    vec3 worldPos;
} vs_out;

void main() {
    vs_out.worldPos = (modelMatrix * vec4(vp, 1.0)).xyz;
    // Required for robust GS pipeline behavior across drivers.
    gl_Position = vec4(vs_out.worldPos, 1.0);
}
