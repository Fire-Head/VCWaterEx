#include "CMatrix.h"

CMatrix::~CMatrix()
{
	((void (__thiscall *)(CMatrix *))0x4DFAE0)(this);
}

CMatrix Invert(CMatrix const& in)
{
	return ((CMatrix (__cdecl *)(CMatrix const&))0x4DE9C0)(in);
}
