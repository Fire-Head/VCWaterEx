#pragma once
#include "..\address.h"
#include "CVector.h"

class COcclusion
{
public:
	static bool IsAABoxOccluded(CVector pos, float fSizeX, float fSizeY, float fSizeZ);
};