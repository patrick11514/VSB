#version 430
#define MAX_LIGHTS 69
// light types
#define POINT 0
#define DIRECTIONAL 1
#define REFLECTOR 2

struct Light {
    int type;
    vec3 color;

    mat4 lightMatrix;
    float kc;
    float kl;
    float kq;

    vec3 direction;
    float angle;
};

struct GPU_Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular; // w is shininess
    vec4 pbrTextureTypes; // x=albedo, y=normal, z=metallic, w=roughness
    vec4 pbrTextureIndices;
    vec4 pbrTextureTypes2; // x=ao
    vec4 pbrTextureIndices2;
};

layout(std430, binding = 0) buffer MaterialBuffer {
    GPU_Material materials[];
};

uniform int u_MaterialIndex;
uniform sampler2D u_Textures[16]; // Increase limit if needed, 16 is fine too

uniform sampler2D irradianceMap;
uniform sampler2D prefilteredMap;
uniform sampler2D brdfLUTMap;

in vec2 uv_out;
in vec4 positionCS;
in mat3 TBN;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;

out vec4 fragColor;

const float PI = 3.14159265359;

// Spherical Map sampling
const vec2 invAtan = vec2(0.15915494309, 0.31830988618);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

// Fresnel
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Distribution
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return nom / denom;
}

// Geometry
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

void main() {
    GPU_Material material = materials[u_MaterialIndex];

    // Fetch PBR Maps
    vec3 albedo = material.diffuse.xyz;
    if (material.pbrTextureTypes.x > 0.5) {
        int texIndex = int(material.pbrTextureIndices.x);
        albedo = pow(texture(u_Textures[texIndex], uv_out).rgb, vec3(2.2)); // sRGB to Linear
    }

    vec3 normal = vec3(0.0, 0.0, 1.0);
    if (material.pbrTextureTypes.y > 0.5) {
        int texIndex = int(material.pbrTextureIndices.y);
        normal = texture(u_Textures[texIndex], uv_out).rgb;
        normal = normal * 2.0 - 1.0; 
        normal = normalize(TBN * normal); 
    } else {
        // If no normal map, just use interpolated vertex normal (N from TBN)
        normal = normalize(TBN[2]);
    }

    if (!gl_FrontFacing) {
        normal = -normal;
    }

    float metallic = 0.0;
    if (material.pbrTextureTypes.z > 0.5) {
        int texIndex = int(material.pbrTextureIndices.z);
        metallic = texture(u_Textures[texIndex], uv_out).r;
    }

    float roughness = 0.5; // Default roughness
    if (material.pbrTextureTypes.w > 0.5) {
        int texIndex = int(material.pbrTextureIndices.w);
        roughness = texture(u_Textures[texIndex], uv_out).r;
    }

    float ao = 1.0;
    if (material.pbrTextureTypes2.x > 0.5) {
        int texIndex = int(material.pbrTextureIndices2.x);
        ao = texture(u_Textures[texIndex], uv_out).r;
    }

    vec3 N = normal;
    vec3 V = normalize(-(positionCS.xyz / positionCS.w));

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
        Light light = lights[i];
        vec4 lightPositionCS4 = (viewMatrix * light.lightMatrix) * vec4(0.0, 0.0, 0.0, 1.0);
        vec3 lightPositionCS = lightPositionCS4.xyz / lightPositionCS4.w;

        vec3 L = vec3(1.0);
        float attenuation = 1.0;

        if (light.type == DIRECTIONAL) {
            vec4 cameraLightDir = viewMatrix * vec4(light.direction, 0.0);
            L = -normalize(cameraLightDir.xyz);
            attenuation = 1.0;
        } else {
            L = normalize(lightPositionCS - (positionCS.xyz / positionCS.w));
            float distance = length(lightPositionCS - (positionCS.xyz / positionCS.w));
            
            if (light.type == REFLECTOR) {
                vec4 cameraLightDir = viewMatrix * vec4(light.direction, 0.0);
                float spot = dot(normalize(cameraLightDir.xyz), -L);
                float alpha = cos(radians(light.angle));
                if (spot < alpha) {
                    continue; // outside cone
                }
                attenuation = (spot - alpha) / (1.0 - alpha);
            }
            attenuation = attenuation * clamp(1.0 / (light.kc + light.kl * distance + light.kq * distance * distance), 0.0, 1.0);
        }

        vec3 H = normalize(V + L);
        vec3 radiance = light.color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
        vec3 specular     = nominator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    // ==== IBL Ambient ====
    mat3 invView = transpose(mat3(viewMatrix));
    vec3 world_N = normalize(invView * N);
    vec3 R = reflect(-V, N);
    vec3 world_R = normalize(invView * R);

    // Diffuse Irradiance IBL
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, SampleSphericalMap(world_N)).rgb;
    vec3 diffuse    = irradiance * albedo;

    // Specular IBL
    const float MAX_REFLECTION_LOD = 8.0; // because we load levels 0 to 8
    vec3 prefilteredColor = textureLod(prefilteredMap, SampleSphericalMap(world_R), roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUTMap, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specularIBL = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specularIBL) * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping & gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);
}
