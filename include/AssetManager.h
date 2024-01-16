#pragma once

#include "SFML/Graphics.hpp"
#include "Map.h"
#include "Animation.h"
#include <string>
#include <map>

typedef std::map<std::string, sf::Texture>             TextureMap;
typedef std::map<std::string, std::vector<sf::Sprite>> SpriteMap;
typedef std::map<std::string, sf::Font>                FontMap;
typedef std::map<std::string, Map>                     MapMap;
typedef std::map<std::string, Animation>               AnimationMap;

class AssetManager
{
	TextureMap   m_textures;
	SpriteMap    m_sprites;
	FontMap      m_fonts;
	MapMap       m_maps;
	MapMap       m_collisions;
	AnimationMap m_animations;

	void init(const std::string& manifestPath);

public:

	AssetManager();
	explicit AssetManager(const std::string& manifestPath);

	void addTexture(const std::string& tag, const std::string& path);
	void addSprites(
		const std::string& tag,
		size_t width,
		size_t height,
		const std::string& path
	);
	void addFont(const std::string& tag, const std::string& path);
	void addMap(const std::string& tag, const std::vector<std::string>& paths);
	void addCollisions(
		const std::string& tag, const std::vector<std::string>& paths
	);
	void addAnimation(
		const std::string& tag,
		unsigned int width,
		unsigned int height,
		float length,
		const std::string& path
	);

	[[nodiscard]] const TextureMap& textures() const;
	[[nodiscard]] const SpriteMap& sprites() const;
	[[nodiscard]] const FontMap& fonts() const;
	[[nodiscard]] const MapMap& maps() const;
	[[nodiscard]] const MapMap& collisions() const;
	[[nodiscard]] const AnimationMap& animations() const;

	[[nodiscard]] const sf::Texture& getTexture(const std::string& tag) const;
	[[nodiscard]] const std::vector<sf::Sprite>& getSprites(
		const std::string& tag
	) const;
	[[nodiscard]] const sf::Sprite& getSprite(
		const std::string& tag, size_t index
	) const;
	[[nodiscard]] const sf::Font& getFont(const std::string& tag) const;
	[[nodiscard]] const Map& getMap(const std::string& tag) const;
	[[nodiscard]] const Map& getCollisions(const std::string& tag) const;
	[[nodiscard]] const Animation& getAnimation(const std::string& tag) const;
};