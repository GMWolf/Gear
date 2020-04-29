layout(location = 0) in vec2 Position;

uniform mat4 view;
void main() {
    gl_Position = view * vec4(Position, 0, 1);
}
