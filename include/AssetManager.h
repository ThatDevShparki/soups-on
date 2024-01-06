#pragma once

#include "SFML/Graphics.hpp"
#include "Map.h"
#include <string>
#include <map>

typedef std::map<std::string, sf::Texture> TextureMap;
typedef std::map<std::string, sf::Font>    FontMap;
typedef std::map<std::string, Map>         MapMap;

class AssetManager
{
	TextureMap m_textures;
	FontMap    m_fonts;
	MapMap     m_maps;

	void init(const std::string& manifestPath);

public:

	AssetManager();
	explicit AssetManager(const std::string& manifestPath);

	void addTexture(const std::string& tag, const std::string& path);
	void addFont(const std::string& tag, const std::string& path);
	void addMap(const std::string& tag, const std::vector<std::string>& paths);

	[[nodiscard]] const TextureMap& textures() const;
	[[nodiscard]] const FontMap& fonts() const;
	[[nodiscard]] const MapMap& maps() const;

	[[nodiscard]] const sf::Texture& getTexture(const std::string& tag) const;
	[[nodiscard]] const sf::Font& getFont(const std::string& tag) const;
	[[nodiscard]] const Map& getMap(const std::string& tag) const;
};