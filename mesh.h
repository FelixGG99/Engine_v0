#pragma once
#ifndef MESH_H
#define MESH_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "vec.h"
#include "Shader.h"
#include<string>
#include<vector>

struct Vertex {  //definition of a vertex type
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct Texture {  //definition of a Texture type
	unsigned int ID;
	std::string type; ///type of the texture (specular, diffuse, etc)
	std::string path;
};

class Mesh {  //class definition of a mesh object

	//Mesh Data
	std::vector<Vertex>vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture>textures;

	//Render Data
	unsigned int VAO, VBO, EBO;

	//Build Render Data
	void SetupMesh();

public:
	//Constructors
	Mesh() {};
	Mesh(const std::vector<Vertex>&v, const std::vector<unsigned int>&i, const std::vector<Texture>&t) {
		vertices = v;	indices = i;	textures = t;
		SetupMesh();
	}

	//Render Function
	void Draw(const Shader &shader) const;
};

#endif // !MESH_H