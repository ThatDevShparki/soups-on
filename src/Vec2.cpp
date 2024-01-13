#include "Vec2.h"

#include <cmath>

Vec2::Vec2() = default;

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{
}

Vec2::Vec2(const sf::Vector2f& v)
	: x(v.x), y(v.y)
{
}


float Vec2::theta() const
{
	return atanf(x / y);
}


bool Vec2::operator ==(const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator !=(const Vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}


Vec2 Vec2::operator +(const Vec2& rhs) const
{
	return { x + rhs.x, y + rhs.y };
}

Vec2 Vec2::operator -(const Vec2& rhs) const
{
	return { x - rhs.x, y - rhs.y };
}

Vec2 Vec2::operator *(float rhs) const
{
	return { x * rhs, y * rhs };
}

Vec2 Vec2::operator /(float rhs) const
{
	return { x / rhs, y / rhs };
}


void Vec2::operator +=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator *=(float rhs)
{
	x *= rhs;
	y *= rhs;
}

void Vec2::operator /=(float rhs)
{
	x /= rhs;
	y /= rhs;
}


float Vec2::mag() const
{
	return sqrtf(powf(x, 2) + powf(y, 2));
}

Vec2 Vec2::norm() const
{
	return *this / mag();
}

float Vec2::dist(const Vec2& rhs)
{
	return (rhs - *this).mag();
}


std::string Vec2::toString() const
{
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vec2::operator sf::Vector2f() const
{
	return { x, y };
}

