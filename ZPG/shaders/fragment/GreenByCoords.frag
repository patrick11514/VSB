#version 330

in float color;
out vec4 frag_colour;
void main () {
     frag_colour = vec4 (0.0, 0.5 + color, 0.0, 1.0);
}
