#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

// variable to control texture swapping
uniform bool swapTextures;

void main()
{
    if (swapTextures) {
        if (gl_PrimitiveID < 2) {  // Grupo 1 (primeros 2 triángulos)
            FragColor = texture(texture2, TexCoord);
        } else {  // Grupo 2 (resto de los triángulos)
            FragColor = texture(texture1, TexCoord); 
        }
    } else {
        if (gl_PrimitiveID < 2) {  // Grupo 1 (primeros 2 triángulos)
            FragColor = texture(texture1, TexCoord);
        } else {  // Grupo 2 (resto de los triángulos)
            FragColor = texture(texture2, TexCoord); 
        }
    }
}