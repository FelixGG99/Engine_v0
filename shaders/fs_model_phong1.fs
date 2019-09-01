#version 330 core

in vec3 frag_pos;
in vec3 Normal;
in vec2 TexCoords;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 pos_vec;
uniform vec3 light_color;
uniform vec3 light_pos;
out vec4 frag_color;
const float ambientLightStrength = 0.1;
const float specularLightStrength = 0.7;
const int shininess = 64;

void main(){

	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(light_pos - frag_pos);

	float diff = max(dot(light_dir,norm),0.0);
	vec3 diffuse = diff*light_color;

	vec3 reflected_light = reflect(-light_dir,norm); //diff_direction goes from obj to light source, negated because we need it from the source to the obj in reflect()

	vec3 view_dir = normalize(pos_vec - frag_pos); 
	float spec = pow(max(dot(view_dir, reflected_light), 0.0), shininess);
	vec3 specular = specularLightStrength * spec * light_color;

	vec3 ambient = ambientLightStrength*light_color;

    frag_color = texture(texture_diffuse1,TexCoords)*vec4(ambient+diffuse+specular,1.0);
}