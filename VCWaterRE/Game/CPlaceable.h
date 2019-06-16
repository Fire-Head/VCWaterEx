#pragma once
#include <windows.h>
#include "CMatrix.h"
#include "..\address.h"

#pragma pack(push, 4)
class CPlaceable
{
public:
    CMatrix m_sCoords;
	
	CPlaceable()
	{
		((void (__thiscall *)(CPlaceable *))0x4BBAD0)(this);
	}
	
	void SetHeading(float heading)
	{
		((void (__thiscall *)(CPlaceable *, float))0x4BBA80)(this, heading);
	}
	
    ~CPlaceable() { }  //~CPlaceable();
};
#pragma pack(pop)