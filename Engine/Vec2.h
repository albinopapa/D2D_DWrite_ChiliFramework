#pragma once

#include <d2d1_1.h>

class Vec2 : public D2D1_POINT_2F
{
public:
	Vec2() = default;
	Vec2( float X, float Y );

	Vec2 operator+( const Vec2& rhs ) const;
	Vec2 operator*( float rhs ) const;
	Vec2 operator-( const Vec2& rhs ) const;

	Vec2& operator+=( const Vec2& rhs );
	Vec2& operator-=( const Vec2& rhs );
	Vec2& operator*=( float rhs );

	float GetLength() const;
	float GetLengthSq() const;

	Vec2& Normalize();
	Vec2 Normalize() const;
};