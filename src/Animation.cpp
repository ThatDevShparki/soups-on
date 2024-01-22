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
	while (m_elapsedTime >= m_animationLength)
	{
		m_elapsedTime -= m_animationLength;
	}
}

sf::Sprite& Animation::getCurrentFrame()
{
	return m_frames.at(int(m_elapsedTime / m_frameDelta));
}