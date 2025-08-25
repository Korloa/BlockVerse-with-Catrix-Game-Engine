#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 u_Position;
uniform vec2 u_Size;
uniform vec2 u_ScreenSize;

void main() {
    vec2 pixelPos = u_Position + aPos * u_Size;
    vec2 ndc = (pixelPos / u_ScreenSize) * 2.0 - 1.0;
    gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0);
    TexCoord = aTexCoord;
}