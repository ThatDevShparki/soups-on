#pragma once

#include "Vec2.h"
#include "SFML/Graphics.hpp"
#include <vector>

class Animation
{
	std::vector<sf::Sprite> m_frames;
	float                   m_animationLength = 0; // length in seconds
	float                   m_elapsedTime     = 0;
	float                   m_frameDelta      = 0;

public:

	Animation();

	Animation(const std::vector<sf::Sprite>& frames, float length);


	void update(float delta);

	[[nodiscard]] sf::Sprite& getCurrentFrame();

};