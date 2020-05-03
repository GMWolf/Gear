layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 Texcoord;

uniform mat4 view;

out vec2 uv;
void main()
{
    gl_Position = view * vec4(Position, 0, 1);
    uv = Texcoord;
}