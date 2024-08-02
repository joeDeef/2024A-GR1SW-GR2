#version 330 core
out vec4 FragColor;
in vec3 ourColor;

// variable for time
uniform float time; 

void main()
{
    // change the color dynamically for each RGB component
    float r = sin(time) * 0.3 + 0.3;
    float g = cos(time) * 0.3 + 0.3;
    float b = sin(time * 0.3) * 0.3 + 0.3;
    FragColor = vec4(r + ourColor.r, g + ourColor.g, b + ourColor.b, 1.0f);
}