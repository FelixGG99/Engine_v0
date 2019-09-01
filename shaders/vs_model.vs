#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 atexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float model_scale;

out vec3 frag_pos;
out vec2 TexCoords;
out vec3 Normal;

void main(){
    TexCoords=atexCoords;
	frag_pos=vec3(model*vec4(pos,1.0));
    gl_Position=projection*view*vec4(frag_pos,1.0);
	
	Normal = mat3(transpose(inverse(model))) * normal;   // vertex's normal correction. expensive when in gpu, make in cpu
}