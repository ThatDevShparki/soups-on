#include "AssetManager.h"

#include <fstream>
#include <iostream>


void AssetManager::addTexture(const std::string& tag, const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Could not load texture from path: " + path << std::endl;
		exit(-1);
	}
	m_textures[tag] = texture;
	std::cout << "Loaded texture " + tag + " from " + path << std::endl;
}

void AssetManager::addFont(const std::string& tag, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cerr << "Could not load font from path: " + path << std::endl;
		exit(-1);
	}
	m_fonts[tag] = font;
	std::cout << "Loaded font " + tag + " from " + path << std::endl;
}


const TextureMap& AssetManager::textures() const
{
	return m_textures;
}

const FontMap& AssetManager::fonts() const
{
	return m_fonts;
}