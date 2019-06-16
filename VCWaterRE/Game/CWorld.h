#pragma once
#include "..\address.h"
#include "CVector.h"
#include "CEntity.h"
#include "CColPoint.h"


class CStoredCollPoly {
public:
    CVector m_aVertices[3]; // triangle vertices
    Bool m_bIsActual;
private:
    char _pad[3];
};

class CWorld
{
public:
	static float FindGroundZFor3DCoord(float x, float y, float z, bool* bOut);
	
	static bool ProcessVerticalLine(CVector const& pos, float a2, CColPoint& point, CEntity*& entity, bool a5, bool a6, bool a7, bool a8, bool a9, bool a10, CStoredCollPoly* a11);
	
};