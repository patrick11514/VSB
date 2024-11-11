#version 330
#define MAX_LIGHTS 69

struct Light {
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
    vec4 totalSpecular = vec4(0.0);
    vec3 viewDir = normalize(-(positionCS.xyz / positionCS.w));

    vec4 objectColor = vec4( 0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4 (0.1 ,0.1 ,0.1 ,1.0);

    vec4 totalColor = vec4(0.0);

    for (int i = 0; i < lightCount; i++) {
        vec4 lightPositionCS4 = (viewMatrix * lights[i].lightMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
        vec3 lightPositionCS = lightPositionCS4.xyz / lightPositionCS4.w;

        vec3 lightDir = normalize(lightPositionCS - (positionCS.xyz / positionCS.w));

        vec3 halfwayDir = normalize(lightDir);
        float distance = length(lightPositionCS - (positionCS.xyz / positionCS.w));  // Compute the distance from light to fragment

        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);
        vec4 specular = spec * vec4(lights[i].color, 1.0);

        float diff = max(dot(normalCS, lightDir), 0.0);
        vec4 diffuse = diff * vec4(lights[i].color, 1.0);
              
        float attenuation = clamp(1 / (lights[i].kc + lights[i].kl * distance + lights[i].kq * distance * distance), 0.0, 1.0);

        totalSpecular += specular * attenuation;
        totalDiffuse += diffuse * attenuation;
    }

    fragColor = ambient + (totalDiffuse * objectColor) + totalSpecular;

}
