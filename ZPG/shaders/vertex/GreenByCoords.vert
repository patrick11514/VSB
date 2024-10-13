#version 330

layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
out float color;
void main () {
     gl_Position = modelMatrix * vec4 (vp, 1.0);
     color = (gl_Position.x + gl_Position.y + gl_Position.z) / 3.0;
}
