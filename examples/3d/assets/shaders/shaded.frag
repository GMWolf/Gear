
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec3 normal;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

vec3 encodeSRGB(vec3 linearRGB)
{
    vec3 a = 12.92 * linearRGB;
    vec3 b = 1.055 * pow(linearRGB, vec3(1.0 / 2.4)) - 0.055;
    vec3 c = step(vec3(0.0031308), linearRGB);
    return mix(a, b, c);
}

void main() {
    vec3 col = texture(tex, texCoord.xy).rgb; //tex assumed to be linear (use srgb format)
    col *= max(0, dot(normalize(normal), -normalize(vec3(0.75,-1,0))));
    col = encodeSRGB(col);
    outColor = vec4(col, 1.0);
}
