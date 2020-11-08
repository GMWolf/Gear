
layout(location = 0) in vec2 texCoord;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(texCoord.xy, 0.0, 1.0);
}
