#version 330

in vec4 worldPosition;
in vec3 worldNormal;
out vec4 fragColor;
void main () {
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(lightPosition - vec3(worldPosition.x / worldPosition.w, worldPosition.y / worldPosition.w, worldPosition.z / worldPosition.w));
    float dot_product = max(dot(lightDir, normalize(worldNormal)), 0.0);
    vec4 diffuse = dot_product * vec4( 0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);
    fragColor = ambient + diffuse;
}
