#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D text1;
uniform sampler2D text2;
uniform float num_text;

void main()
{
    if(num_text == 1){
        FragColor = vec4(texture(text1, TexCoord).rgb, 1.0);
    }else{
        FragColor = vec4(texture(text2, TexCoord).rgb, 1.0);
    }
}