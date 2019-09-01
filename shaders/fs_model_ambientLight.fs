#version 330 core

in vec3 frag_pos;
in vec3 Normal;
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 light_color;
uniform vec3 light_pos;
out vec4 fin_color;

void main(){
    const float ambientLightStrength=0.1;
    vec3 final_light=ambientLightStrength*light_color;

    fin_color=vec4(final_light,1.0)*texture(texture_diffuse1,TexCoords);
}