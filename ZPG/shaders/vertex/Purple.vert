#version 330

layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main () {
     gl_Position = vec4 (vp, 1.0);
}
