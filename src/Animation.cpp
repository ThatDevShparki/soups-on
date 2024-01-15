#include "Animation.h"


Animation::Animation() = default;

Animation::Animation(
	const std::vector<sf::Sprite>& frames,
	float length
)
	: m_frames(frames), m_animationLength(length), m_frameDelta(
	length / float(frames.size()))
{
}

void Animation::update(float delta)
{
	m_elapsedTime += delta;
	m_elapsedTime = std::remainder(m_animationLength, m_elapsedTime);
}

const sf::Sprite& Animation::getCurrentFrame() const
{
	return m_frames[int(m_elapsedTime / m_frameDelta) - 1];
}