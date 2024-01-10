#include "AssetManager.h"

#include <fstream>
#include <iostream>


AssetManager::AssetManager() = default;

AssetManager::AssetManager(const std::string& manifestPath)
{
	init(manifestPath);
}


void AssetManager::init(const std::string& manifestPath)
{
	std::ifstream file(manifestPath);
	if (!file)
	{
		std::cerr << "Could not load manifest from path: " + manifestPath
				  << std::endl;
		exit(-1);
	}

	std::string head;
	while (file >> head)
	{
		if (head == "font")
		{
			std::string n, p;
			file >> n >> p;
			addFont(n, "assets/" + p);
		}
		else if (head == "texture")
		{
			std::string n, p;
			file >> n >> p;
			addTexture(n, "assets/" + p);
		}
		else if (head == "map")
		{
			std::string n, p;
			file >> n >> p;
			addMap(n, { "assets/" + p });
		}
	}
	std::cout << "Loaded all assets from manifest " + manifestPath << std::endl;
}


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

void AssetManager::addMap(
	const std::string& tag, const std::vector<std::string>& paths
)
{
	if (m_maps.count(tag) > 0)
	{
		for (const auto& p: paths)
		{
			m_maps[tag].addLayer(p);
		}
	}
	else
	{
		m_maps[tag] = Map(paths);
	}

	std::cout << "Loaded map " + tag + ": " << std::endl;
}


const TextureMap& AssetManager::textures() const
{
	return m_textures;
}

const FontMap& AssetManager::fonts() const
{
	return m_fonts;
}

const MapMap& AssetManager::maps() const
{
	return m_maps;
}


const sf::Texture& AssetManager::getTexture(const std::string& tag) const
{
	return m_textures.at(tag);
}

const sf::Font& AssetManager::getFont(const std::string& tag) const
{
	return m_fonts.at(tag);
}

const Map& AssetManager::getMap(const std::string& tag) const
{
	return m_maps.at(tag);
}