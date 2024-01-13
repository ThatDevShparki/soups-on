#pragma once

#include <string>
#include "SFML/Graphics.hpp"

class Vec2
{
public:

	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float x, float y);
	explicit Vec2(const sf::Vector2f& v);

	[[nodiscard]] float theta() const;

	[[nodiscard]] bool operator ==(const Vec2& rhs) const;
	[[nodiscard]] bool operator !=(const Vec2& rhs) const;

	[[nodiscard]] Vec2 operator +(const Vec2& rhs) const;
	[[nodiscard]] Vec2 operator -(const Vec2& rhs) const;
	[[nodiscard]] Vec2 operator *(float rhs) const;
	[[nodiscard]] Vec2 operator /(float rhs) const;

	void operator +=(const Vec2& rhs);
	void operator -=(const Vec2& rhs);
	void operator *=(float rhs);
	void operator /=(float rhs);

	[[nodiscard]] float mag() const;
	[[nodiscard]] Vec2 norm() const;
	[[nodiscard]] float dist(const Vec2& rhs);

	[[nodiscard]] std::string toString() const;

	[[nodiscard]] explicit operator sf::Vector2f() const;
};