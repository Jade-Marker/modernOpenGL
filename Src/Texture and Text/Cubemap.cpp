#include "Cubemap.h"
#include <vector>

//Code for this class is based off of code from this tutorial: https://learnopengl.com/Advanced-OpenGL/Cubemaps

Cubemap::Cubemap(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back)
{
	_textureType = GL_TEXTURE_CUBE_MAP;
	GenerateTexture(GL_CLAMP_TO_EDGE);

	std::vector<std::string> texturePaths;
	texturePaths.push_back(right);
	texturePaths.push_back(left);
	texturePaths.push_back(bottom);
	texturePaths.push_back(top);
	texturePaths.push_back(front);
	texturePaths.push_back(back);

	for (int i = 0; i < texturePaths.size(); i++)
	{
		_textureType = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		LoadImage(texturePaths[i]);
	}

	_textureType = GL_TEXTURE_CUBE_MAP;
}
