
#include "srgb.glsl"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D occlusion;
layout(binding = 2) uniform sampler2D normalMap;

layout(location = 0) out vec4 outColor;



mat3 computeTBN() {
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent.xyz);
    vec3 X = normalize(T - N * dot(N, T));
    vec3 B = cross(X, N) * tangent.w;
    return mat3(T, B, N);
}

void main() {

    mat3 TBN = computeTBN();
    vec3 normalVec = TBN * normalize((texture(normalMap, texCoord.xy).rgb - 0.5) * 2.0);
    float light = max(0, dot(normalize(normalVec), -normalize(vec3(0.75,-1,0))));
    light += 0.1 * texture(occlusion, texCoord.xy).r;

    vec3 col = texture(albedo, texCoord.xy).rgb; //tex assumed to be linear (use srgb format)
    col *= light;
    col = encodeSRGB(col);
    outColor = vec4(col.rgb, 1.0);
}
