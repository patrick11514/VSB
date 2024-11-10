#version 330
#define MAX_LIGHTS 69

struct Light {
    mat4 lightMatrix;   // Light's transformation matrix (from light space to world space)
    vec3 color;         // Light color (no alpha)
    float kc;           // Constant attenuation
    float kl;           // Linear attenuation
    float kq;           // Quadratic attenuation
};

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 viewPos;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

out vec4 fragColor;

void main () {
    vec4 totalDiffuse = vec4(0.0);
    vec4 totalSpecular = vec4(0.0);
    vec3 viewDir = normalize(viewPos - (worldPosition.xyz / worldPosition.w));

    vec4 objectColor = vec4( 0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4 (0.1 ,0.1 ,0.1 ,1.0);

    vec4 totalColor = vec4(0.0);

    for (int i = 0; i < lightCount; i++) {
        vec3 lightPosition = (lights[i].lightMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

        vec3 lightDir = normalize(lightPosition - (worldPosition.xyz / worldPosition.w));

        vec3 halfwayDir = normalize(lightDir + viewDir);

        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);
        vec4 specular = spec * vec4(lights[i].color, 1.0);

        float diff = max(dot(worldNormal, lightDir), 0.0);
        vec4 diffuse = diff * vec4(lights[i].color, 1.0);
               
        totalSpecular += specular;
        totalDiffuse += diffuse;
    }

    fragColor = ambient + (totalDiffuse * objectColor) + totalSpecular;

}
