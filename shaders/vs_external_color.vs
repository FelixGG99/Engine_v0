#version 330 core

layout (location = 0) in vec3 pos;

uniform vec3 color;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 frag_color;

void main(){
    frag_color=color;
    gl_Position = projection*view*model*vec4(pos,1.0);
}