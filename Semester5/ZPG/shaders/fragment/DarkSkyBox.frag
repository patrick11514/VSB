#version 330

uniform samplerCube textureUnit; 

in vec3 position;

out vec4 fragColor;

void main () {
    fragColor = texture(textureUnit, position) * vec4(0.2, 0.2, 0.2, 1.0);
}
