#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 norm;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

out vec4 worldPosition;
out vec3 worldNormal;
out vec3 viewPos;
//uniform mat3 normalMatrix; //(M-1)T

void main(void){
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    worldPosition = modelMatrix * vec4(vp, 1.0f);
    worldNormal = normalize(mat3(transpose(inverse(modelMatrix))) * norm);

    viewPos = cameraPosition;
}
