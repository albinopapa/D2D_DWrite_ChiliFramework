#include "Vec2.h"
#include <cmath>

Vec2::Vec2( float X, float Y )
	:
	D2D1_POINT_2F( { X, Y } )
{
}

Vec2 Vec2::operator+( const Vec2& rhs ) const
{
	return Vec2( x + rhs.x,y + rhs.y );
}

Vec2& Vec2::operator+=( const Vec2& rhs )
{
	return *this = *this + rhs;
}

Vec2 Vec2::operator*( float rhs ) const
{
	return Vec2( x * rhs,y * rhs );
}

Vec2& Vec2::operator*=( float rhs )
{
	return *this = *this * rhs;
}

Vec2 Vec2::operator-( const Vec2& rhs ) const
{
	return Vec2( x - rhs.x,y - rhs.y );
}

Vec2& Vec2::operator-=( const Vec2& rhs )
{
	return *this = *this - rhs;
}

float Vec2::GetLength() const
{
	return std::sqrt( GetLengthSq() );
}

float Vec2::GetLengthSq() const
{
	return x * x + y * y;
}

Vec2& Vec2::Normalize()
{
	const float len = GetLength();
	if( len != 0.0f )
	{
		return *this * ( 1.0f / len );
	}
}

Vec2 Vec2::Normalize() const
{
	return Vec2( *this ).Normalize();
}
