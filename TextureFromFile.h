#pragma once

#ifndef TEXFROMFILE_H
#define TEXFROMFILE_H

//MAKE SURE THE DIRECTIVE #define STB_IMAGE_IMPLEMENTATION is defined somewhere in a single .cpp file before including stb_image.h!

#include <glad/glad.h>
#include <vector>
#include <iostream>
unsigned int TextureFromFile(const char* filename, const std::string &directory);
unsigned int TextureFromFile(const char* filename);
#endif // !TEXFROMFILE_H
