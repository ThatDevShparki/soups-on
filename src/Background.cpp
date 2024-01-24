#include "Background.h"

Background::Background() = default;

Background::Background(
	const std::vector<sf::Texture>& textures,
	const std::vector<float>& factors
)
	: m_textures(textures), m_factors(factors)
{
	for (const auto& texture: m_textures)
	{
		float size = float(texture.getSize().y);
		if (size > m_size)
		{
			m_size = size;
		}
		m_offsets.push_back(0.0f);

		sf::Sprite sprite(texture);
		m_sprites.push_back(sprite);
	}
}


void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(m_view);
	for (const auto& sprite: m_sprites)
	{
		target.draw(sprite, states);
	}
//	for (unsigned int i = 0; i < m_size; i++)
//	{
//		auto& texture = m_textures[i];
//		auto& offset  = m_offsets[i];
//
//		sf::Sprite sprite(texture);
////		float      scale = m_size / float(texture.getSize().y);
//		sprite.setPosition(offset, 0);
////		sprite.setScale(scale, scale);
//
//		target.draw(sprite, states);
//	}
}

void Background::update(float delta)
{
	for (unsigned int i = 0; i < m_size; i++)
	{
		auto& offset = m_offsets[i];
		auto& factor = m_factors[i];
		m_offsets[i] = offset + factor * delta;
	}
}

void Background::addLayer(const sf::Texture& texture, float factor)
{
	float size = float(texture.getSize().y);
	if (size > m_size)
	{
		m_size = size;
	}
	m_factors.push_back(factor);
	m_offsets.push_back(0.0f);

	sf::Sprite sprite(texture);
	m_sprites.push_back(sprite);
}

const sf::View& Background::view() const
{
	return m_view;
}

void Background::onResizeView(const Vec2& size)
{
	float aspectRatio = size.y / size.x;
	float width       = m_size / aspectRatio;
	m_view.setSize({ width, m_size });
}