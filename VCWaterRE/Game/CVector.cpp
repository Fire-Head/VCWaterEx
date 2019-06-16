#include "CVector.h"

CVector CrossProduct(const CVector& first, const CVector& second)
{
    return CVector(second.z * first.y - first.z * second.y,
					first.z * second.x - second.z * first.x,
					first.x * second.y - second.x * first.y);
}

Float DotProduct(CVector const &refA, CVector const &refB)
{
	return refA.x * refB.x
			+ refA.y * refB.y
			+ refA.z * refB.z;
}

CVector::CVector()
{
	//((void(__thiscall *)(CVector *))AddressByVersion(0x436F40, 0x436F40, 0x436F40))(this);
}

CVector::CVector(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void CVector::Normalise()
{
	((void (__thiscall*)(CVector*))0x4DFEA0)(this);
}

float CVector::NormaliseAndMag()
{
	float fScal = MagnitudeSqr();
	if ( fScal > 0.0f )
	{
		float fNorm = 1.0f / sqrtf(fScal);
		x *= fNorm;
		y *= fNorm;
		z *= fNorm;
		return 1.0f / fNorm;
	}
	else
	{
		this->x = 1.0f;
		return 1.0f;
	}
}

void CVector::Normalise2D()
{
	Float fScal = Magnitude2DSqr();
	
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

float CVector::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

// operator +
CVector operator + (CVector const &refLeft, CVector const &refRight)
{
	return CVector(refLeft.x + refRight.x, refLeft.y + refRight.y, refLeft.z + refRight.z);
}

CVector operator + (CVector const &refLeft, Float fRight)
{
	return CVector(refLeft.x + fRight, refLeft.y + fRight, refLeft.z + fRight);
}

CVector operator + (Float fLeft, CVector const &refRight)
{
	return CVector(fLeft + refRight.x, fLeft + refRight.y, fLeft + refRight.z);
}

// operator -
CVector operator - (CVector const &refLeft, CVector const &refRight)
{
	return CVector(refLeft.x - refRight.x, refLeft.y - refRight.y, refLeft.z - refRight.z);
}

CVector operator - (CVector const &refLeft, Float fRight)
{
	return CVector(refLeft.x - fRight, refLeft.y - fRight, refLeft.z - fRight);
}

CVector operator - (Float fLeft, CVector const &refRight)
{
	return CVector(fLeft - refRight.x, fLeft - refRight.y, fLeft - refRight.z);
}

// operator *
CVector operator * (CVector const &refLeft, CVector const &refRight)
{
	return CVector(refLeft.x * refRight.x, refLeft.y * refRight.y, refLeft.z * refRight.z);
}

CVector operator * (CVector const &refLeft, Float fRight)
{
	return CVector(refLeft.x * fRight, refLeft.y * fRight, refLeft.z * fRight);
}

CVector operator * (Float fLeft, CVector const &refRight)
{
	return CVector(fLeft * refRight.x, fLeft * refRight.y, fLeft * refRight.z);
}

// operator /
CVector operator / (CVector const &refLeft, CVector const &refRight)
{
	return CVector(refLeft.x / refRight.x, refLeft.y / refRight.y, refLeft.z / refRight.z);
}

CVector operator / (CVector const &refLeft, Float fRight)
{
	return CVector(refLeft.x / fRight, refLeft.y / fRight, refLeft.z / fRight);
}

CVector operator / (Float fLeft, CVector const &refRight)
{
	return CVector(fLeft / refRight.x, fLeft / refRight.y, fLeft / refRight.z);
}