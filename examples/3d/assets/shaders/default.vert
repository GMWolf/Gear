layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordin;

layout(location = 0) out vec2 texCoord;

struct Transform {
    vec3 position; float padding;
    vec4 rotation;
};

layout(binding = 0) uniform SceneBuffer {
    Transform transform;
    mat4 projection;
};

vec3 rotate(vec3 vec, vec4 quat) {
    vec3 t = 2 * cross(quat.xyz, vec);
    return vec + quat.w * t + cross(quat.xyz, t);
}

void main() {
    texCoord = texCoordin;
    vec3 tPos = rotate(position, transform.rotation) + transform.position;
    gl_Position = projection * vec4(tPos, 1.0);
}
