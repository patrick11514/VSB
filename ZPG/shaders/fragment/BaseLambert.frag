#version 330
#define MAX_LIGHTS 69
//light types
#define POINT_LIGHT 0
#define REFLECTOR 1 

struct Light {
    int lightType;
    mat4 lightMatrix;   // Light's transformation matrix (from light space to world space)
    vec3 color;         // Light color (no alpha)
    float kc;           // Constant attenuation
    float kl;           // Linear attenuation
    float kq;           // Quadratic attenuation
};

in vec4 positionCS;
in vec3 normalCS;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;

out vec4 fragColor;

void main () {
    vec4 totalDiffuse = vec4(0.0);
    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];
        vec4 lightPositionCS4 = (viewMatrix * light.lightMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
        vec3 lightPositionCS = lightPositionCS4.xyz / lightPositionCS4.w;

        float distance = length(lightPositionCS - (positionCS.xyz / positionCS.w));  // Compute the distance from light to fragment
        vec3 lightDir = normalize(lightPositionCS - (positionCS.xyz / positionCS.w));

        float diff = max(dot(lightDir, normalize(normalCS)), 0.0);
        vec4 diffuse = diff * vec4(light.color, 1.0);
       
        float attenuation = clamp(1 / (light.kc + light.kl * distance + light.kq * distance * distance), 0.0, 1.0);

        totalDiffuse += diffuse * attenuation;
    }

    fragColor = ambient + totalDiffuse * objectColor;
}
