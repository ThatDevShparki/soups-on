#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "Vec2.h"

typedef std::vector<float> FloatArray;

class Background : public sf::Drawable
{

	sf::View                 m_view;
	std::vector<sf::Texture> m_textures;
	std::vector<float>       m_factors;
	std::vector<float>       m_offsets;
	float                    m_size = 0;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateView(const sf::View& view);

public:

	Background();

	Background(
		const std::vector<sf::Texture>& textures,
		const std::vector<float>& factors
	);


	void update(float delta);
	void addLayer(const sf::Texture& texture, float factor);

	const sf::View& view() const;
	void onResizeView(const Vec2& size);
};