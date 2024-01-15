#pragma once

#include "Vec2.h"
#include "SFML/Graphics.hpp"
#include <vector>

class Animation
{
	std::vector<sf::Sprite> m_frames;
	float                   m_animationLength; // length in seconds
	float                   m_elapsedTime;
	float                   m_frameDelta;

public:

	Animation();

	Animation(const std::vector<sf::Sprite>& frames, float length);


	void update(float delta);

	[[nodiscard]] const sf::Sprite& getCurrentFrame() const;


};