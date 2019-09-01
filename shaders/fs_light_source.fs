#version 330 core

in vec3 frag_color;
out vec4 fin_color;
uniform vec3 light_color;

void main(){
    fin_color=vec4(light_color, 1.0);
}
