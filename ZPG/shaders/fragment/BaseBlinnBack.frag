#version 330
#define MAX_LIGHTS 69

struct Light {
    mat4 lightMatrix;
    vec3 color;
    float kc;
    float kl;
    float kq;
};


in vec4 positionCS; // Camera Space position
in vec4 normalCS; // Camera Space normal

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;

out vec4 fragColor;

void main () {
    vec4 totalDiffuse = vec4(0.0);
    vec4 totalSpecular = vec4(0.0);
    vec4 ambient = vec4 (0.1, 0.1, 0.1, 1.0);
    vec4 viewDir = vec4 (0.0, 0.0, 1.0, 1.0); // view Dir in camera space

    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];
        vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

        vec4 lightPositionCS = (viewMatrix * light.lightMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
        float distance = length(lightPositionCS - positionCS);

        vec4 lightDir = normalize(lightPositionCS - positionCS);

        vec4 halfwayDir = normalize(lightDir + viewDir);

        float spec = pow(max(dot(normalCS, halfwayDir), 0.0), 32.0);
        vec4 specular = spec * color;

        float diff = max(dot(normalCS, lightDir), 0.0);
        vec4 diffuse = diff * color;

        float attenuation = 1.0 / (light.kc + light.kl * distance + light.kq * distance * distance);

        totalDiffuse += diffuse * attenuation;
        totalSpecular += specular * attenuation;

    }

    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    fragColor = ambient + (totalDiffuse * objectColor) + totalSpecular;
}
