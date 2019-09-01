#include "TextureFromFile.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned int TextureFromFile(const char* filename, const std::string &directory) {
	std::string path = directory + '/' + filename;

	unsigned int texID;
	glGenTextures(1, &texID);

	int w, h, nrComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &nrComponents, 0);

	if (data) {

		GLenum format;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Texture located at ";
		std::cout << path.c_str();
		std::cout << " could not be loaded.\n";
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

unsigned int TextureFromFile(const char* filename) {
	std::string path = filename;

	unsigned int texID;
	glGenTextures(1, &texID);

	int w, h, nrComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &nrComponents, 0);

	if (data) {

		GLenum format;
		if (nrComponents == 1) format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Texture located at ";
		std::cout << path.c_str();
		std::cout << " could not be loaded.\n";
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return texID;
}