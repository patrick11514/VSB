#version 330

layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 position;

void main () {
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0);
    position = vp;
}
