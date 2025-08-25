#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform float u_Alpha;

void main() {
    vec4 texColor = texture(u_Texture, TexCoord);
    FragColor = vec4(texColor.rgb, texColor.a * u_Alpha);
}