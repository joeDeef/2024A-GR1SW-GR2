#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform float num_text;

void main()
{
    //FragColor = texture(texture1, TexCoord);
    if (num_text == 1.0) {
        FragColor = texture(texture1, TexCoord);
    } else if (num_text == 2.0) {
        FragColor = texture(texture2, TexCoord);
    } else if (num_text == 3.0) {
        FragColor = texture(texture3, TexCoord);
    }
}
