layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 texCoordin;
layout(location = 3) in vec4 tangentIn;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 tangent;
layout(location = 3) out vec3 viewDir;
layout(location = 4) out flat int drawID;

struct Transform {
    vec3 position;
    float scale;
    vec4 rotation;
};

layout(binding = 0) uniform CameraBuffer {
    Transform cameraTransform;
    mat4 projection;
};

layout(std430, binding = 1) buffer TransformBuffer {
    Transform objectTransforms[];
};

vec3 rotate(vec3 vec, vec4 quat) {
    vec3 t = 2 * cross(quat.xyz, vec);
    return vec + quat.w * t + cross(quat.xyz, t);
}

vec4 conjugate(vec4 quat) {
    return vec4(-quat.xyz, quat.w);
}

vec3 applyTransform(vec3 pos, Transform transform) {
    return (rotate(pos, transform.rotation) * transform.scale) + transform.position;
}

Transform inverseTransform(Transform transform) {
    Transform result;
    result.position = rotate(-transform.position, conjugate(transform.rotation));
    result.rotation = conjugate(transform.rotation);
    result.scale = 1.0; //TODO: scale
    return result;
}



void main() {

    Transform transform = objectTransforms[gl_DrawID];

    texCoord = texCoordin;
    texCoord.y = -texCoord.y;
    normal = rotate(normalIn, transform.rotation);
    tangent.xyz = rotate(tangentIn.xyz, transform.rotation);
    tangent.w = tangentIn.w;
    vec3 tPos = applyTransform(position, transform);
    vec3 vPos = applyTransform(tPos, inverseTransform(cameraTransform));
    gl_Position = projection * vec4(vPos, 1.0);
    viewDir = cameraTransform.position - tPos;
    drawID = gl_DrawID;
}
