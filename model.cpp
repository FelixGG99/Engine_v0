#include "model.h"


void Model::Draw(const Shader &shader) const {

	// Draw every Mesh object conforming the Model object
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(const std::string &path) {

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	/*	Useful Assimp Importer Flags:
		aiProcess_Triangulate		:	if the mesh is not entirely conformed by triangles, transform every primitive in the mesh into a triangle
		aiProcess_FlipUVs			:	flip the coordinates in the y axis when necessary during texture processing so they are shown in a correct way
		aiProcess_GenNormals		:	generate normals if the model doesn't contain them
		aiProcess_SplitLargeMeshes	:	splits large meshes into smaller sub-meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.
		aiProcess_OptimizeMeshes	:	joins several meshes into one larger mesh, reducing drawing calls for optimization.
	*/

	//check if the scene is null, flags are incomplete or is not pointing to a root node
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {  
		std::cout << "ERROR: ASSIMP: " << importer.GetErrorString() << "\n"; ///print error
		return;
	}

	//determine the directory path where resources and files are stored (local directory, not absolute)
	directory = path.substr(0, path.find_last_of('/'));
	//process data from the scene recursively checking every node composing it
	processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode *node, const aiScene *scene) {

	// process every mesh in the node (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// visit every children of the current node (if any)
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {

	// vectors conforming the current Mesh object
	std::vector<Vertex>vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture>textures;

	//placeholder to temporarily store the vertex being extracted from the mesh
	Vertex placeholder;

	// extract the vertices' data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		// extract position of current vertex
		placeholder.Position.e[0] = mesh->mVertices[i].x;
		placeholder.Position.e[1] = mesh->mVertices[i].y;
		placeholder.Position.e[2] = mesh->mVertices[i].z;
		//extract normal of current vertex
		placeholder.Normal.e[0] = mesh->mNormals[i].x;
		placeholder.Normal.e[1] = mesh->mNormals[i].y;
		placeholder.Normal.e[2] = mesh->mNormals[i].z;

		//extract texture coords of current vertex (if any)
		if (mesh->mTextureCoords[0]) {  ///assimp allows the model to have up to 8 different texture coordinates per vertex but here only the first one is used (indexed on 0)
			placeholder.TexCoords.e[0] = mesh->mTextureCoords[0][i].x;
			placeholder.TexCoords.e[1] = mesh->mTextureCoords[0][i].y;
		}
		else placeholder.TexCoords = vec2(0.0f, 0.0f);
		
		// push the current vertex into the vertices vector
		vertices.push_back(placeholder);
	}

	// extract the indices (faces) 
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i]; /// extract a face from the mesh
		for (unsigned int j = 0; j < face.mNumIndices; j++) ///from the face, extract every index it contains...
			indices.push_back(face.mIndices[j]); ///...and store it in the indices vertex of the Mesh object
	}

	// extract the textures
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture>diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(),diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture>specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType &type, const std::string &typeName) {
	
	std::vector<Texture>textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		// get which texture to use
		mat->GetTexture(type, i, &str);

		//optimization
		unsigned char skip = 0;
		for(unsigned int j=0;j<textures_loaded.size();j++){ // if required texture is already loaded, just store which one to use and skip loading it
			if (!std::strcmp(textures_loaded[j].path.c_str(), str.C_Str())) {
				textures.push_back(textures_loaded[j]);
				skip = 1;
				break;
			}
		}
		if (skip) continue;

		// it is has not been loaded, load it and store it for further use
		Texture tex;
		tex.ID = TextureFromFile(str.C_Str(), this->directory);
		tex.type = typeName;
		tex.path = str.C_Str();
		textures.push_back(tex);
		textures_loaded.push_back(tex);
	}
	return textures;
}
