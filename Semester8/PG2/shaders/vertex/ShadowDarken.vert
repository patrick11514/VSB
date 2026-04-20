#version 430

void main() {
    vec2 p;
    if (gl_VertexID == 0) {
        p = vec2(-1.0, -1.0);
    } else if (gl_VertexID == 1) {
        p = vec2(3.0, -1.0);
    } else {
        p = vec2(-1.0, 3.0);
    }

    gl_Position = vec4(p, 0.0, 1.0);
}
