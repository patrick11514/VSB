#version 430
#extension GL_ARB_bindless_texture : require
#define MAX_LIGHTS 69
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
    vec4 specular;
    vec4 pbrTextureTypes;
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    vec4 pbrTextureTypes2;
    sampler2D aoMap;
    sampler2D rmaMap;
    sampler2D padding2;
    sampler2D padding3;
};

layout(std430, binding = 0) buffer MaterialBuffer {
    GPU_Material materials[];
};

uniform int u_MaterialIndex;

uniform sampler2D irradianceMap;
uniform sampler2D prefilteredMap;
uniform sampler2D brdfLUTMap;
uniform sampler2D shadowMap;

in vec2 uv_out;
in vec4 positionCS;
in vec3 positionWS;
in mat3 TBN;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform mat4 viewMatrix;
uniform float shadowBiasMin;
uniform float shadowBiasMax;
uniform int useShadowMap;
uniform float uAmbientScale;
uniform float uDirectScale;

out vec4 fragColor;

const float PI = 3.14159265359;

const vec2 invAtan = vec2(0.15915494309, 0.31830988618);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;

    uv.y = 1.0 - uv.y;
    return uv;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

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
// Task 6: compare the fragment against the shadow map with PCF filtering.
float SampleShadow(vec3 fragPosWS, vec3 N, vec3 L, mat4 lightMatrix) {
    if (useShadowMap == 0) {
        return 1.0;
    }

    vec4 fragPosLightSpace = lightMatrix * vec4(fragPosWS, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 1.0;
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0) {
        return 1.0;
    }

    float bias = max(shadowBiasMax * (1.0 - max(dot(N, L), 0.0)), shadowBiasMin);
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));

    float visibility = 0.0;
    float samples = 0.0;

    int halfKernelSize = 2;

    float spread = 1.5;

    for (int x = -halfKernelSize; x <= halfKernelSize; ++x) {
        for (int y = -halfKernelSize; y <= halfKernelSize; ++y) {
            vec2 offset = vec2(x, y) * texelSize * spread;
            float pcfDepth = texture(shadowMap, projCoords.xy + offset).r;

            visibility += (projCoords.z - bias) <= pcfDepth ? 1.0 : 0.0;
            samples += 1.0;
        }
    }

    return visibility / samples;
}

// Task 3 and 5: evaluate Cook-Torrance PBR with normal mapping and IBL.
void main() {
    GPU_Material material = materials[u_MaterialIndex];

    vec3 albedo = material.diffuse.xyz;
    if (material.pbrTextureTypes.x > 0.5) {
        vec4 albedoTex = texture(material.albedoMap, uv_out);
        albedo = pow(albedoTex.rgb, vec3(2.2));
    }

    vec3 normal = vec3(0.0, 0.0, 1.0);
    if (material.pbrTextureTypes.y > 0.5) {
        normal = texture(material.normalMap, uv_out).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBN * normal);
    } else {
        normal = normalize(TBN[2]);
    }

    float metallic = 0.0;
    float roughness = 0.5;
    float ao = 1.0;

    float Ns = material.specular.w; 
    
    if (Ns > 0.0) {
        roughness = sqrt(2.0 / (Ns + 2.0));
    }

    if (material.pbrTextureTypes2.y > 0.5) {
        vec3 rma = texture(material.rmaMap, uv_out).rgb;
        roughness = rma.r;
        metallic = rma.g;
        ao = rma.b;
    } else {
        if (material.pbrTextureTypes.z > 0.5) {
            metallic = texture(material.metallicMap, uv_out).r;
        }
        if (material.pbrTextureTypes.w > 0.5) {
            roughness = texture(material.roughnessMap, uv_out).r;
        }
        if (material.pbrTextureTypes2.x > 0.5) {
            ao = texture(material.aoMap, uv_out).r;
        }
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
                    continue;
                }
                attenuation = (spot - alpha) / (1.0 - alpha);
            }
            attenuation = attenuation * clamp(1.0 / (light.kc + light.kl * distance + light.kq * distance * distance), 0.0, 1.0);
        }

        vec3 H = normalize(V + L);
        vec3 radiance = light.color * attenuation;

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
        float shadowFactor = SampleShadow(positionWS, N, L, light.lightMatrix);
        Lo += uDirectScale * shadowFactor * (kD * albedo / PI + specular) * radiance * NdotL;
    }

    mat3 invView = transpose(mat3(viewMatrix));
    vec3 world_N = normalize(invView * N);
    vec3 R = reflect(-V, N);
    vec3 world_R = normalize(invView * R);

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, SampleSphericalMap(world_N)).rgb;
    vec3 diffuse    = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 8.0;
    vec3 prefilteredColor = textureLod(prefilteredMap, SampleSphericalMap(world_R), roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUTMap, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specularIBL = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = uAmbientScale * (kD * diffuse + specularIBL) * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);
}
