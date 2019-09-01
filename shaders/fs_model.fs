#version 330 core

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
out vec4 frag_color;

void main(){
    frag_color=texture(texture_diffuse1,TexCoords);
}