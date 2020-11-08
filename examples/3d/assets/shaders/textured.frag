
layout(location = 0) in vec2 texCoord;

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 col = texture(tex, texCoord.xy).rgb;
    outColor = vec4(col, 1.0);
}
