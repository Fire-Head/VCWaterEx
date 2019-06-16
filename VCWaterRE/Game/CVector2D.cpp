#include "CVector2D.h"


void CVector2D::Normalise()
{
	/*
	Float fScal = MagnitudeSqr();
	Float fNorm = 1.0f / sqrt(fScal);
	x *= fNorm;
	y *= fNorm;
	*/
	Float fScal = MagnitudeSqr();
	
	if ( fScal > 0.0f )
	{
		Float fNorm = 1.0f / sqrt(fScal);
		x *= fNorm;
		y *= fNorm;
	}
	else
	{
		x = 1.0f;
	}
}

// operator +
CVector2D operator + (CVector2D const &refLeft, CVector2D const &refRight)
{
	return CVector2D(refLeft.x + refRight.x, refLeft.y + refRight.y);
}

CVector2D operator + (CVector2D const &refLeft, Float fRight)
{
	return CVector2D(refLeft.x + fRight, refLeft.y + fRight);
}

CVector2D operator + (Float fLeft, CVector2D const &refRight)
{
	return CVector2D(fLeft + refRight.x, fLeft + refRight.y);
}

// operator -
CVector2D operator - (CVector2D const &refLeft, CVector2D const &refRight)
{
	return CVector2D(refLeft.x - refRight.x, refLeft.y - refRight.y);
}

CVector2D operator - (CVector2D const &refLeft, Float fRight)
{
	return CVector2D(refLeft.x - fRight, refLeft.y - fRight);
}

CVector2D operator - (Float fLeft, CVector2D const &refRight)
{
	return CVector2D(fLeft - refRight.x, fLeft - refRight.y);
}

// operator *
CVector2D operator * (CVector2D const &refLeft, CVector2D const &refRight)
{
	return CVector2D(refLeft.x * refRight.x, refLeft.y * refRight.y);
}

CVector2D operator * (CVector2D const &refLeft, Float fRight)
{
	return CVector2D(refLeft.x * fRight, refLeft.y * fRight);
}

CVector2D operator * (Float fLeft, CVector2D const &refRight)
{
	return CVector2D(fLeft * refRight.x, fLeft * refRight.y);
}

// operator /
CVector2D operator / (CVector2D const &refLeft, CVector2D const &refRight)
{
	return CVector2D(refLeft.x / refRight.x, refLeft.y / refRight.y);
}

CVector2D operator / (CVector2D const &refLeft, Float fRight)
{
	return CVector2D(refLeft.x / fRight, refLeft.y / fRight);
}

CVector2D operator / (Float fLeft, CVector2D const &refRight)
{
	return CVector2D(fLeft / refRight.x, fLeft / refRight.y);
}

Float DotProduct(CVector2D const &refA, CVector2D const &refB)
{
	return refA.x * refB.x + refA.y * refB.y;
}

Float CrossProduct(CVector2D const &refA, CVector2D const &refB)
{
	return refA.x * refB.y - refA.y * refB.x;
}
