#version 330

layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out float color;
void main () {
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);
     color = (100 + 100 + 100) / 3.0;
}
