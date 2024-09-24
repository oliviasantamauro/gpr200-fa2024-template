#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;

uniform float uTime;
uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, TexCoord);
}