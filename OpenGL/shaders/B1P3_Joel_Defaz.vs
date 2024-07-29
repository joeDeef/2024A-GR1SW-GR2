#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float tiempo;
out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    float new_x = sin(tiempo)/0.8f;
    gl_Position = vec4(aPos.x + new_x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}