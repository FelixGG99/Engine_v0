#pragma once
#ifndef MODEL_H
#define MODEL_H


#include "vec.h"
#include "Shader.h"
#include "mesh.h"
#include "TextureFromFile.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model {

	// Model data
	std::vector<Texture> textures_loaded;
	std::vector<Mesh>meshes;
	std::string directory;

	// Extract Model from File Functions
	void loadModel(const std::string &path);
	void processNode(const aiNode *node, const aiScene *scene);
	Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
	std::vector<Texture>loadMaterialTextures(const aiMaterial *mat, const aiTextureType &type, const std::string &typeName);

public:
	// Render Function
	void Draw(const Shader &shader) const;
	
	// Constructors
	Model(const char*path) { loadModel(path); }
	Model() {}
};

#endif // !MODEL_H
