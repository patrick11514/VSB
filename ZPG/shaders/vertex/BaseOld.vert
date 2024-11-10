#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 norm;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 positionCS; // Camera Space position
out vec4 normalCS; // Camera Space normal

void main(void){
    positionCS = (viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    gl_Position = projectionMatrix * positionCS;
    normalCS = normalize(transpose(inverse(modelMatrix)) * vec4(norm, 1.0));
}
