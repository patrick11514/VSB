#version 330

uniform samplerCube textureUnit; 

in vec3 position;

out vec4 fragColor;

void main () {
    fragColor = texture(textureUnit, position);
}
