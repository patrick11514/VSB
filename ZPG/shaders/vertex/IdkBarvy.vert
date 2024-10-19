#version 330

layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 color;
void main () {
     gl_Position = modelMatrix * vec4 (vp, 1.0);
     color = vec3(0.5 + gl_Position.x / gl_Position.w, 0.5 + gl_Position.y / gl_Position.w, 0.5 + gl_Position.z / gl_Position.w);
}
