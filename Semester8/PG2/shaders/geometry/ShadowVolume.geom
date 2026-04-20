#version 430

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 18) out;

in VS_OUT {
    vec3 worldPos;
} gs_in[];

uniform mat4 viewProjection;
uniform vec3 lightDirection;
uniform float extrusionDistance;

float FaceFacing(vec3 a, vec3 b, vec3 c) {
    vec3 n = normalize(cross(b - a, c - a));
    return dot(n, -normalize(lightDirection));
}

void EmitTriangle(vec3 a, vec3 b, vec3 c) {
    gl_Position = viewProjection * vec4(a, 1.0);
    EmitVertex();
    gl_Position = viewProjection * vec4(b, 1.0);
    EmitVertex();
    gl_Position = viewProjection * vec4(c, 1.0);
    EmitVertex();
    EndPrimitive();
}

void EmitQuad(vec3 a, vec3 b, vec3 c, vec3 d) {
    gl_Position = viewProjection * vec4(a, 1.0);
    EmitVertex();
    gl_Position = viewProjection * vec4(b, 1.0);
    EmitVertex();
    gl_Position = viewProjection * vec4(c, 1.0);
    EmitVertex();
    gl_Position = viewProjection * vec4(d, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    vec3 p0 = gs_in[0].worldPos;
    vec3 p1 = gs_in[2].worldPos;
    vec3 p2 = gs_in[4].worldPos;

    vec3 a0 = gs_in[1].worldPos;
    vec3 a1 = gs_in[3].worldPos;
    vec3 a2 = gs_in[5].worldPos;

    float fMain = FaceFacing(p0, p1, p2);

    if (fMain <= 0.0) {
        return;
    }

    vec3 extrude = normalize(lightDirection) * extrusionDistance;
    vec3 e0 = p0 + extrude;
    vec3 e1 = p1 + extrude;
    vec3 e2 = p2 + extrude;

    // Front and back caps keep volume closed for z-fail.
    EmitTriangle(p0, p1, p2);
    EmitTriangle(e2, e1, e0);

    float fAdj0 = FaceFacing(p1, p0, a0);
    float fAdj1 = FaceFacing(p2, p1, a1);
    float fAdj2 = FaceFacing(p0, p2, a2);

    if ((fMain > 0.0 && fAdj0 <= 0.0) || (fMain <= 0.0 && fAdj0 > 0.0)) {
        EmitQuad(p0, p1, e0, e1);
    }

    if ((fMain > 0.0 && fAdj1 <= 0.0) || (fMain <= 0.0 && fAdj1 > 0.0)) {
        EmitQuad(p1, p2, e1, e2);
    }

    if ((fMain > 0.0 && fAdj2 <= 0.0) || (fMain <= 0.0 && fAdj2 > 0.0)) {
        EmitQuad(p2, p0, e2, e0);
    }
}
