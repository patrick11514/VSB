#version 330
#define MAX_LIGHTS 69

struct Light {
    mat4 lightMatrix;   // Light's transformation matrix (from light space to world space)
    vec3 color;         // Light color (no alpha)
    float kc;           // Constant attenuation
    float kl;           // Linear attenuation
    float kq;           // Quadratic attenuation
};

in vec4 positionCS; // Camera space position (world position transformed by view matrix)
in vec4 normalCS;   // Camera space normal (transformed by view matrix)

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;  // View matrix to transform coordinates into camera space

out vec4 fragColor;

void main () {
    vec4 totalDiffuse = vec4(0.0);
    vec4 totalSpecular = vec4(0.0);
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0); // Ambient light
    vec3 normal = normalCS.xyz;

    vec3 viewDir = normalize(-positionCS.xyz);  // Corrected viewDir as vec3 (from camera to fragment)

    // Loop through all lights
    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];

        // Light position in camera space (view space) using light's transformation matrix and viewMatrix
        vec4 lightPositionCS = viewMatrix * (light.lightMatrix * vec4(0.0, 0.0, 0.0, 1.0));
        float distance = length(lightPositionCS.xyz - positionCS.xyz);  // Compute the distance from light to fragment

        // Light direction from fragment to light
        vec3 lightDir = normalize(lightPositionCS.xyz - positionCS.xyz);  // Make sure this is normalized

        // Halfway vector (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);

        // Diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        vec4 diffuse = diff * vec4(light.color, 1.0); // Using color as vec3, expanding to vec4

        // Specular shading
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec4 specular = spec * vec4(light.color, 1.0); // Using color as vec3, expanding to vec4

        // Attenuation based on distance
        float attenuation = 1.0 / (light.kc + light.kl * distance + light.kq * distance * distance);

        totalDiffuse += diffuse * attenuation;
        totalSpecular += specular * attenuation;
    }

    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0); // Object color
    //fragColor = ambient + (totalDiffuse * objectColor) + totalSpecular; // Final color with attenuation
    fragColor = vec4(diffuse.rgb, 1.0);
}
