#pragma once

#include "Vec2.h"

class RectF : public D2D1_RECT_F
{
public:
	RectF() = default;
	RectF( float Left, float Top, float Right, float Bottom );
	RectF( const Vec2& topLeft, const Vec2& bottomRight );
	RectF( const Vec2& topLeft, float width, float height );
	bool IsOverlappingWith( const RectF& other ) const;
	static RectF FromCenter( const Vec2& center, float halfWidth, float halfHeight );

};
