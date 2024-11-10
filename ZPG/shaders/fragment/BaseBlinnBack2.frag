#version 330
#define MAX_LIGHTS 69

struct Light {
    mat4 lightMatrix;
    vec3 color;
    float kc;
    float kl;
    float kq;
};

in vec4 positionCS;   // Camera Space position
in vec4 normalCS;     // Camera Space normal

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;

out vec4 fragColor;

void main() {
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
    vec3 ambient = vec3(0.1, 0.1, 0.1);

    vec3 viewDir = normalize(-positionCS.xyz);  // View direction in camera space

    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];

        light.color = vec3(1.0, 0.0, 0.0);

        // Transform light position from light origin to camera space
        vec4 lightPositionCS = viewMatrix * light.lightMatrix * vec4(0.0, 0.0, 0.0, 1.0);

        // Calculate light direction and distance in camera space
        vec3 lightDir = normalize(lightPositionCS.xyz - positionCS.xyz);
        float distance = length(lightPositionCS.xyz - positionCS.xyz);

        // Lambertian Diffuse
        float diff = max(dot(normalize(normalCS.xyz), lightDir), 0.0);
        vec3 diffuse = diff * light.color;

        // Blinn-Phong Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normalize(normalCS.xyz), halfwayDir), 0.0), 32.0);
        vec3 specular = spec * light.color;

        // Attenuation
        float attenuation = 1.0 / (light.kc + light.kl * distance + light.kq * distance * distance);

        totalDiffuse += diffuse * attenuation;
        totalSpecular += specular * attenuation;

    }

    vec3 objectColor = vec3(0.385, 0.647, 0.812);
    fragColor = vec4(ambient + totalDiffuse * objectColor + totalSpecular, 1.0);
}
