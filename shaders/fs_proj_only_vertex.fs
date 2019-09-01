#version 330 core
out vec4 fin_color;
uniform vec3 color;

void main(){
    fin_color = vec4(color,0.2);
}