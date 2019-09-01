#include "mesh.h"

void Mesh::SetupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	//Vertex Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	//Vertex TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::Draw(const Shader &shader) const {

	/* In the fragment shader, as many uniform samplers must be declared as textures we want to use, type explicitly defined:
		uniform sampler2D texture_diffuse1;
		uniform sampler2D texture_diffuse2;
		...
		uniform sampler2D texture_diffuseN;
		uniform sampler2D texture_specular1;
		uniform sampler2D texture_specular2;
		...
		uniform sampler2D texture_specularN;
	*/

	unsigned int diffuseNr = 1;		//number of the next available uniform to store a diffuse texture into
	unsigned int specularNr = 1;	//number of the next available uniform to store a specular texture into

	// bind every texture of the mesh
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); ///activate the current texture unit where the texture is going to be stored

		std::string number;
		if (textures[i].type == "texture_diffuse") number = std::to_string(diffuseNr++);
		else if (textures[i].type == "texture_specular") number = std::to_string(specularNr++);

		shader.setInt((textures[i].type + number).c_str(), i); ///indicate to the uniform sampler which texture unit to use
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);	///bind the texture to the texture unit currently active
	}
	glActiveTexture(GL_TEXTURE0); ///return active texture to default after setting up the tex units

	// draw mesh
	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
