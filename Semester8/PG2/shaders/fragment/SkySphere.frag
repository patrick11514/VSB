#version 430

out vec4 fragColor;
in vec3 localPos;

uniform sampler2D envMap;

const vec2 invAtan = vec2(0.15915494309, 0.31830988618);

vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;

    uv.y = 1.0 - uv.y;

    return uv;
}

// Task 8: sample the spherical environment map for the background.
void main() {
    vec2 uv = SampleSphericalMap(normalize(localPos));
    
    vec3 color = texture(envMap, uv).rgb;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);
}