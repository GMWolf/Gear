in vec2 uv;
out vec4 color;
uniform sampler2D tex;
void main(){
    float a = texture(tex, uv).r;

    if (a == 0) discard;

    color.rgb = vec3(1);
    color.a = a;
}