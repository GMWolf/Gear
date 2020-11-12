
#include "srgb.glsl"
#include "pbr.glsl"

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec3 viewDir;

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D occlusion;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D metallicRoughness;

layout(location = 0) out vec4 outColor;


mat3 computeTBN() {
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent.xyz);
    vec3 X = normalize(T - N * dot(N, T));
    vec3 B = cross(X, N) * tangent.w;
    return mat3(T, B, N);
}

vec3 reconstructZ(vec2 v) {
    return vec3(v, 1.0 - dot(v,v));
}

void main() {

    mat3 TBN = computeTBN();
    vec3 n = reconstructZ((texture(normalMap, texCoord.xy).rg - 0.5) * 2.0);
    vec3 normalVec = TBN * n;

    PBRFragment pbrFrag;
    pbrFrag.albedo = texture(albedo, texCoord.xy).rgb;
    pbrFrag.metalicity = texture(metallicRoughness, texCoord.xy).b;
    pbrFrag.roughness = texture(metallicRoughness, texCoord.xy).g;
    pbrFrag.emmisivity = vec3(0.0);
    pbrFrag.normal = normalVec;

    LightFragment lightFrag;
    lightFrag.lightDirection = -normalize(vec3(0.75,-1,0));
    lightFrag.radiance = vec3(1.0f);

    vec3 col = pbrColor(pbrFrag, lightFrag, normalize(viewDir));

    float AO = texture(occlusion, texCoord.xy).r;
    col += 0.03 * pbrFrag.albedo * AO;

    col = encodeSRGB(col);
    outColor = vec4(col, 1.0);
    //outColor.rgb = tangent.xyz * 0.5 + 0.5;

}
