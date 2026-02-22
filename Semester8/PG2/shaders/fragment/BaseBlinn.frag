#version 430
#define MAX_LIGHTS 69
//light types
#define POINT 0
#define DIRECTIONAL 1
#define REFLECTOR 2

#define MATERIAL 10
#define TEXTURE 11

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

struct GPU_Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular; // w is shininess
    vec4 textureInfo; // x=type, y=index
};

layout(std430, binding = 0) buffer MaterialBuffer {
    GPU_Material materials[];
};

uniform int u_MaterialIndex;
uniform sampler2D u_Textures[16];

in vec2 uv_out;
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

    vec4 text = vec4(1.0);
    
    GPU_Material material = materials[u_MaterialIndex];

    if (material.textureInfo.x > 0.5) {
        int texIndex = int(material.textureInfo.y);
        text = texture(u_Textures[texIndex], uv_out);
    }

    vec4 ambientColor = vec4(material.ambient.xyz, 1.0) * text;

    for (int i = 0; i < lightCount; ++i) {
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

        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.specular.w);
        vec4 specular = spec * vec4(light.color, 1.0) * vec4(material.specular.xyz, 1.0);

        float diff = max(dot(normalCS, lightDir), 0.0);
        vec4 diffuse = diff * vec4(light.color, 1.0) * vec4(material.diffuse.xyz, 1.0) * text;
             
        float attenuation = 1;

        if (light.type == DIRECTIONAL) {
            attenuation = 1;
        } else {
            if (light.type == REFLECTOR) {
                vec4 cameraLightDir = viewMatrix * vec4(light.direction, 0.0);
                float spot = dot(normalize(cameraLightDir.xyz), -lightDir);
                
                float alpha = cos(radians(light.angle));

                if (spot < alpha) {
                    diffuse = vec4(0.0);
                    specular = vec4(0.0);
                }

                attenuation = (spot - alpha) / (1 - alpha);
            }

            float distance = length(lightPositionCS - (positionCS.xyz / positionCS.w));  // Compute the distance from light to fragment
            attenuation = attenuation * clamp(1 / (light.kc + light.kl * distance + light.kq * distance * distance), 0.0, 1.0);
        }

        totalSpecular += specular * attenuation;
        totalDiffuse += diffuse * attenuation;
    }

    fragColor = ambientColor + totalDiffuse + totalSpecular;

}
