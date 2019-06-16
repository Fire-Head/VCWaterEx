#pragma once
#include "..\address.h"
#include "CVector.h"


class CWaterCreatures
{
public:
	static void UpdateAll(void);
	static bool IsSpaceForMoreWaterCreatures(void);
	static void CreateOne(CVector const&pos, int id);
};