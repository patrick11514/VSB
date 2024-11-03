#version 330

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 viewPos;
out vec4 fragColor;

void main () {
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);
    vec3 lightDir = normalize(lightPosition - (worldPosition.xyz / worldPosition.w));

    vec4 ambient = vec4 (0.1 ,0.1 ,0.1 ,1.0);

    vec3 reflectDir = reflect(-lightDir, worldNormal);
    vec3 viewDir = normalize(viewPos - (worldPosition.xyz / worldPosition.w));

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec4 specular = spec * vec4(lightColor, 1.0);

    vec4 objectColor = vec4( 0.385, 0.647, 0.812, 1.0);

    float diff = max(dot(worldNormal, lightDir), 0.0);
    vec4 diffuse = diff * vec4(lightColor, 1.0);

    fragColor = ambient + (diffuse * objectColor) + specular;

}
