#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 Color;
out vec2 TexCoord;
uniform float uTime;

void main()
{
   Color = aColor;
   TexCoord = aTexCoord * 4.0f;
   vec3 pos = aPos;
   pos.y += sin(uTime + pos.x) / 8.0;
   gl_Position = vec4(pos.x * 2, pos.y * 2, pos.z * 2, 1.0);
}