#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

// variable for set texture
uniform int setTexture;

void main()
{
     if(setTexture == 0){
        FragColor = texture(texture1, TexCoord);
     }else if(setTexture == 1){
        FragColor = texture(texture2, TexCoord);
     }else if(setTexture == 2){
        FragColor = texture(texture3, TexCoord);
     }
}