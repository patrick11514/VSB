#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 uv;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 positionCS;
out vec3 normalCS;
out vec2 uv_out;

void main () {
    positionCS = (viewMatrix * modelMatrix) * vec4(vp, 1.0);
    gl_Position = projectionMatrix * positionCS;
    normalCS = normalize(mat3(transpose(inverse(viewMatrix * modelMatrix))) * norm);
    uv_out = uv;
}
