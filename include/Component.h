#pragma once

#include "Vec2.h"

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2  pos    = { 0.0f, 0.0f };
	Vec2  vel    = { 0.0f, 0.0f };
	Vec2  acc    = { 0.0f, 0.0f };
	Vec2  scale  = { 0.0f, 0.0f };
	float rot    = 0.0f;
	float maxVel = 0.0f;
	float maxAcc = 0.0f;

	CTransform() = default;

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
	Vec2 size     = { 0.0f, 0.0f };
	Vec2 halfSize = { 0.0f, 0.0f };

	CBoundingBox() = default;

	CBoundingBox(const Vec2& size, const Vec2& halfSize)
		: size(size), halfSize(halfSize)
	{
	}
};