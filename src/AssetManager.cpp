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
		else if (head == "background")
		{
			std::string n, t, f;
			file >> n >> t >> f;
			addBackground(n, t, std::stof(f));
		}
		else if (head == "map")
		{
			std::string n, p;
			file >> n >> p;
			addMap(n, { "assets/" + p });
		}
		else if (head == "collisions")
		{
			std::string n, p;
			file >> n >> p;
			addCollisions(n, { "assets/" + p });
		}
		else if (head == "animation")
		{
			std::string n, w, h, l, p;
			file >> n >> w >> h >> l >> p;
			addAnimation(n, std::stoi(w), std::stoi(h), std::stof(l), "assets/" + p);
		}
		else if (head == "music")
		{
			std::string n, p;
			file >> n >> p;
			addSound(n, "assets/" + p);
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

	for (int j = 0; j < size.y; j += int(height))
	{
		for (int i = 0; i < size.x; i += int(width))
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

void AssetManager::addBackground(
	const std::string& tag,
	const std::string& textureTag,
	float factor
)
{
	auto& texture = m_textures.at(textureTag);

	if (m_backgrounds.count(tag) > 0)
	{
		m_backgrounds.at(tag).addLayer(texture, factor);
	}
	else
	{
		m_backgrounds[tag] = Background({ texture }, { factor });
	}

	std::cout << "Loaded background " + tag + " with texture " + textureTag
			  << std::endl;
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

void AssetManager::addCollisions(
	const std::string& tag,
	const std::vector<std::string>& paths
)
{
	if (m_collisions.count(tag) > 0)
	{
		for (const auto& p: paths)
		{
			m_collisions[tag].addLayer(p);
		}
	}
	else
	{
		m_collisions[tag] = Map(paths);
	}

	std::cout << "Update collisions for " + tag << std::endl;
}

void AssetManager::addAnimation(
	const std::string& tag,
	unsigned int width,
	unsigned int height,
	float length,
	const std::string& path
)
{
	addSprites(tag, width, height, path);
	m_animations[tag] = Animation(getSprites(tag), length);
	std::cout
		<< "Loaded animation (" + std::to_string(length) + "s) " + tag + " from " +
		   path << std::endl;
}

void AssetManager::addSound(const std::string& tag, const std::string& path)
{
	sf::SoundBuffer soundBuffer;
	if (!soundBuffer.loadFromFile(path))
	{
		std::cerr << "Unable to open music from file " + path << std::endl;
		throw std::invalid_argument("Unable to open music file");
	}
	m_sounds[tag] = soundBuffer;
	std::cout << "Loaded sound " + tag + " from " + path << std::endl;
}


const TextureMap& AssetManager::textures() const
{
	return m_textures;
}

const SpriteMap& AssetManager::sprites() const
{
	return m_sprites;
}

const BackgroundMap& AssetManager::backgrounds() const
{
	return m_backgrounds;
}

const FontMap& AssetManager::fonts() const
{
	return m_fonts;
}

const MapMap& AssetManager::maps() const
{
	return m_maps;
}

const MapMap& AssetManager::collisions() const
{
	return m_collisions;
}

const AnimationMap& AssetManager::animations() const
{
	return m_animations;
}

const SoundBufferMap& AssetManager::sounds() const
{
	return m_sounds;
}


const sf::Texture& AssetManager::getTexture(const std::string& tag) const
{
	return m_textures.at(tag);
}

const std::vector<sf::Sprite>& AssetManager::getSprites(const std::string& tag) const
{
	return m_sprites.at(tag);
}

const sf::Sprite& AssetManager::getSprite(const std::string& tag, size_t index) const
{
	return getSprites(tag)[index];
}

const Background& AssetManager::getBackground(const std::string& tag) const
{
	return m_backgrounds.at(tag);
}

const sf::Font& AssetManager::getFont(const std::string& tag) const
{
	return m_fonts.at(tag);
}

const Map& AssetManager::getMap(const std::string& tag) const
{
	return m_maps.at(tag);
}

const Map& AssetManager::getCollisions(const std::string& tag) const
{
	return m_collisions.at(tag);
}

const Animation& AssetManager::getAnimation(const std::string& tag) const
{
	return m_animations.at(tag);
}

const sf::SoundBuffer& AssetManager::getSound(const std::string& tag) const
{
	return m_sounds.at(tag);
}
