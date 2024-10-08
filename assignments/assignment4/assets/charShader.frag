#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoord;

uniform float uTime;
uniform sampler2D texture2;

void main()
{
   vec4 texColor = texture(texture2, TexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}