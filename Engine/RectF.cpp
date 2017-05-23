#include "RectF.h"

RectF::RectF( float Left, float Top, float Right, float Bottom )
	:
	D2D1_RECT_F( { Left, Top, Right, Bottom } )
{
}

RectF::RectF(const Vec2 & topLeft, const Vec2 & bottomRight) :
	RectF( topLeft.x, topLeft.y, bottomRight.x, bottomRight.y )
{
}

RectF::RectF(const Vec2 & topLeft, float width, float height) :
	RectF( topLeft, topLeft + Vec2( width, height ) )
{
}

bool RectF::IsOverlappingWith(const RectF & other) const
{
	return 
		right > other.left && left < other.right &&
		top < other.bottom && bottom > other.top;
}

RectF RectF::FromCenter(const Vec2 & center, float halfWidth, float halfHeight)
{
	const Vec2 half(halfWidth, halfHeight);
	return RectF(center - half, center + half);
}
