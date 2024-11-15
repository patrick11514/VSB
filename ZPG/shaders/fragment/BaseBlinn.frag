#version 330
#define MAX_LIGHTS 69
//light types
#define POINT 0
#define DIRECTIONAL 1
#define REFLECTOR 2

struct Light {
    int type;
    vec3 color;         // Light color (no alpha)

    mat4 lightMatrix;   // Light's transformation matrix (from light space to world space)
    float kc;           // Constant attenuation
    float kl;           // Linear attenuation
    float kq;           // Quadratic attenuation

    vec3 direction;     // for directional light

    float angle;        // for reflector
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
        Light light = lights[i];
        vec4 lightPositionCS4 = (viewMatrix * light.lightMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
        vec3 lightPositionCS = lightPositionCS4.xyz / lightPositionCS4.w;

        vec3 lightDir = vec3(1.0);
        if (light.type == DIRECTIONAL) {
            vec4 cameraLightDir = viewMatrix * vec4(light.direction, 0.0);
            lightDir = -normalize(cameraLightDir.xyz);
        } else {
            lightDir = normalize(lightPositionCS - (positionCS.xyz / positionCS.w));
        }

        vec3 halfwayDir = normalize(lightDir);

        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);
        vec4 specular = spec * vec4(light.color, 1.0);

        float diff = max(dot(normalCS, lightDir), 0.0);
        vec4 diffuse = diff * vec4(light.color, 1.0);
             
        float attenuation;

        if (light.type == DIRECTIONAL) {
            attenuation = 1;
        } else if (light.type == REFLECTOR) {
            vec4 cameraLightDir = viewMatrix * vec4(light.direction, 0.0);
            float spot = dot(normalize(cameraLightDir.xyz), -lightDir);
            
            float alpha = cos(radians(light.angle));

            if (spot < alpha) {
                diffuse = vec4(0.0);
                specular = vec4(0.0);
            }

            attenuation = (spot - alpha) / (1 - alpha);
        } else {
            float distance = length(lightPositionCS - (positionCS.xyz / positionCS.w));  // Compute the distance from light to fragment
            attenuation = clamp(1 / (light.kc + light.kl * distance + light.kq * distance * distance), 0.0, 1.0);
        }

        totalSpecular += specular * attenuation;
        totalDiffuse += diffuse * attenuation;
    }

    fragColor = ambient + (totalDiffuse * objectColor) + totalSpecular;

}
