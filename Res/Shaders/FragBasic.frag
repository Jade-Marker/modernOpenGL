#version 450 core

in vec2 vs_textureCoord;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{  
    color = texture(u_Texture, vs_textureCoord);
}