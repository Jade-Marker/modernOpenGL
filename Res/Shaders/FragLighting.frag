#version 450 core

//Code for this shader is based off of code from these tutorials: https://learnopengl.com/Lighting/Basic-Lighting https://learnopengl.com/Lighting/Materials

in vec2 vs_textureCoord;

in vec3 vs_DiffuseLight;
in vec3 vs_AmbientLight;
in vec3 vs_SpecularLight;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{  
    color = texture(u_Texture, vs_textureCoord) * vec4((vs_DiffuseLight + vs_AmbientLight + vs_SpecularLight), 1.0f);
}