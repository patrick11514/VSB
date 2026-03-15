#version 430

layout(location=0) in vec3 vp;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 positionCS;
out vec2 uv_out;
out mat3 TBN;

void main () {
    positionCS = (viewMatrix * modelMatrix) * vec4(vp, 1.0);
    gl_Position = projectionMatrix * positionCS;
    
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * norm);
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
    uv_out = uv;
}
