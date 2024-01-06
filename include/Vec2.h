#pragma once


class Vec2
{
public:

	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float x, float y);

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
};