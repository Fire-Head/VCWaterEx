#pragma once
#include <Windows.h>
#include <math.h> 
#include "RenderWare.h"
#include "..\address.h"
#include "types.h"

#define SQR(x) ( (x) * (x) )

class CVector2D
{
public:
	Float x;
	Float y;
	
	CVector2D()
	{
		;
	}
	
	CVector2D(Float fX, Float fY) : x(fX), y(fY)
	{
		;
	}

	CVector2D(CVector2D &refVector) : x(refVector.x), y(refVector.y)
	{
		;
	}

	Float MagnitudeSqr() const
	{
		return SQR(x) + SQR(y);
	}
	
	Float Magnitude() const
	{
		return sqrt(MagnitudeSqr());
	}

	void Normalise();
	
	// operator =
	inline CVector2D const& operator = (CVector2D const &refRight)
	{
		x = refRight.x;
		y = refRight.y;
		return *this;
	}
	
	inline CVector2D const& operator = (Float fRight)
	{
		x = fRight;
		y = fRight;
		return *this;
	}
	
	// operator +=
	inline CVector2D const& operator += (CVector2D const &refRight)
	{
		x += refRight.x;
		y += refRight.y;
		return *this;
	}
	
	inline CVector2D const& operator += (Float fRight)
	{
		x += fRight;
		y += fRight;
		return *this;
	}
	
	// operator -=
	inline CVector2D const& operator -= (CVector2D const &refRight)
	{
		x -= refRight.x;
		y -= refRight.y;
		return *this;
	}
	
	inline CVector2D const& operator -= (Float fRight)
	{
		x -= fRight;
		y -= fRight;
		return *this;
	}
	
	// operator *=
	inline CVector2D const& operator *= (CVector2D const &refRight)
	{
		x *= refRight.x;
		y *= refRight.y;
		return *this;
	}
	
	inline CVector2D const& operator *= (Float fRight)
	{
		x *= fRight;
		y *= fRight;
		return *this;
	}
	
	// operator /=
	inline CVector2D const& operator /= (CVector2D const &refRight)
	{
		x /= refRight.x;
		y /= refRight.y;
		return *this;
	}
	
	inline CVector2D const& operator /= (Float fRight)
	{
		x /= fRight;
		y /= fRight;
		return *this;
	}
};

// operator +
CVector2D operator + (CVector2D const &refLeft, CVector2D const &refRight);
CVector2D operator + (CVector2D const &refLeft, Float fRight);
CVector2D operator + (Float fLeft, CVector2D const &refRight);

// operator -
CVector2D operator - (CVector2D const &refLeft, CVector2D const &refRight);
CVector2D operator - (CVector2D const &refLeft, Float fRight);
CVector2D operator - (Float fLeft, CVector2D const &refRight);

// operator *
CVector2D operator * (CVector2D const &refLeft, CVector2D const &refRight);
CVector2D operator * (CVector2D const &refLeft, Float fRight);
CVector2D operator * (Float fLeft, CVector2D const &refRight);

// operator /
CVector2D operator / (CVector2D const &refLeft, CVector2D const &refRight);
CVector2D operator / (CVector2D const &refLeft, Float fRight);
CVector2D operator / (Float fLeft, CVector2D const &refRight);

Float DotProduct(CVector2D const &refA, CVector2D const &refB);
Float CrossProduct(CVector2D const &refA, CVector2D const &refB);