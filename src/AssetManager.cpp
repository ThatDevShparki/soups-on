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
		else if (head == "sprites")
		{
			std::string n, w, h, p;
			file >> n >> w >> h >> p;
			addSprites(n, std::stoi(w), std::stoi(h), "assets/" + p);
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

void AssetManager::addSprites(
	const std::string& tag,
	size_t width,
	size_t height,
	const std::string& path
)
{
	addTexture(tag, path);

	std::vector<sf::Sprite> sprites;
	auto& texture = getTexture(tag);
	auto size = texture.getSize();

	if (size.x % width != 0 || size.y % height != 0)
	{
		std::cerr << "Sizes for sprites do not align with texture size" << std::endl;
		exit(-1);
	}

	for (int j = 0; j < size.y; j += int(width))
	{
		for (int i = 0; i < size.x; i += int(height))
		{
			sprites.emplace_back(
				texture,
				sf::IntRect(i, j, int(width), int(height))
			);
		}
	}

	m_sprites[tag] = sprites;
	std::cout << "Loaded " + std::to_string(sprites.size()) + " sprites " + tag +
				 " from " + path << std::endl;
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

const SpriteMap& AssetManager::sprites() const
{
	return m_sprites;
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

const sf::Sprite& AssetManager::getSprite(const std::string& tag, size_t index) const
{
	return m_sprites.at(tag)[index];
}

const sf::Font& AssetManager::getFont(const std::string& tag) const
{
	return m_fonts.at(tag);
}

const Map& AssetManager::getMap(const std::string& tag) const
{
	return m_maps.at(tag);
}