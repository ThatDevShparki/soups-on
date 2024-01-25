#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Vec2.h"
#include "Animation.h"

#include <tuple>


class Component
{
public:
	bool has = false;
};


class CTransform : public Component
{
public:

	Vec2  pos     = { 0.0f, 0.0f };
	Vec2  prevPos = { 0.0f, 0.0f };
	Vec2  vel     = { 0.0f, 0.0f };
	Vec2  acc     = { 0.0f, 0.0f };
	Vec2  scale   = { 0.0f, 0.0f };
	float rot     = 0.0f;
	float maxVel  = 0.0f;
	float maxAcc  = 0.0f;

	CTransform() = default;

	explicit CTransform(const Vec2& pos)
		: pos(pos)
	{
	};

	CTransform(
		const Vec2& pos,
		const Vec2& vel,
		const Vec2& acc,
		const Vec2& scale,
		float rot,
		float maxVel,
		float maxAcc
	)
		: pos(pos)
		, vel(vel)
		, acc(acc)
		, scale(scale)
		, rot(rot)
		, maxVel(maxVel)
		, maxAcc(maxAcc)
	{
	};
};

class CBoundingBox : public Component
{
public:

	Vec2 size     = { 0.0f, 0.0f };
	Vec2 halfSize = { 0.0f, 0.0f };

	CBoundingBox() = default;

	explicit CBoundingBox(const Vec2& size)
		: size(size)
	{
		halfSize = { size.x / 2, size.y / 2 };
	}
};

class CShape : public Component
{
public:
	sf::RectangleShape shape;
	sf::Color          color;

	CShape() = default;

	CShape(const Vec2& size, const sf::Color& color)
		: shape({ size.x, size.y }), color(color)
	{
	}
};

class CInput : public Component
{
public:
	bool up    = false;
	bool down  = false;
	bool left  = false;
	bool right = false;

	CInput() = default;
};

class CSprite : public Component
{
public:
	sf::Sprite sprite;

	CSprite() = default;

	explicit CSprite(const sf::Sprite& sprite)
		: sprite(sprite)
	{
	}
};

class CState : public Component
{
public:
	bool jumping    = false;
	bool running    = false;
	bool facingLeft = false;

	CState() = default;

	bool operator ==(CState rhs) const
	{
		return (jumping == rhs.jumping) && (running == rhs.running) &&
			   (facingLeft == rhs.facingLeft);
	}

	bool operator !=(CState rhs) const
	{
		return !(rhs == *this);
	}
};

class CPrevState : public CState
{
public:
	CPrevState() = default;

	explicit CPrevState(CState state)
	{
		jumping    = state.jumping;
		running    = state.running;
		facingLeft = state.facingLeft;
	}
};

class CAnimation : public Component
{
public:
	Animation animation;

	CAnimation() = default;

	explicit CAnimation(const Animation& animation)
		: animation(animation)
	{
	}
};

class CSound : public Component
{
public:
	sf::SoundBuffer buffer;
	sf::Sound       sound;
	float           volume = 50.0f;
	bool            paused = false;

	CSound() = default;

	explicit CSound(const sf::SoundBuffer& buffer)
		: buffer(buffer), sound(sf::Sound(buffer))
	{
	}
};