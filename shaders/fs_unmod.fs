#version 330 core

in vec3 frag_color;
out vec4 fin_color;

void main(){
    fin_color= vec4(frag_color,1.0);
}