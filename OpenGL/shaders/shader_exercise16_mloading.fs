#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
    // Lee el color de la textura difusa
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    // Lee el color de la textura especular
    vec4 specularColor = texture(texture_specular1, TexCoords);
    // Lee el color de la textura normal
    vec4 normalColor = texture(texture_normal1, TexCoords);
    // Lee el color de la textura de altura
    vec4 heightColor = texture(texture_height1, TexCoords);

    // Combina las texturas de alguna manera; aquí solo para ilustrar, puedes ajustar según sea necesario
    vec4 finalColor = diffuseColor;
    finalColor.rgb += specularColor.rgb * 0.2; // Por ejemplo, agrega un poco del color especular
    finalColor.rgb += normalColor.rgb * 0.1;   // Agrega un poco del color normal
    finalColor.rgb += heightColor.rgb * 0.05;  // Agrega un poco del color de altura

    FragColor = finalColor;
}