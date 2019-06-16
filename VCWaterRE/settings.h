#pragma once
#include "_precl.h"
#include "_config.h"
#include "tweak.h"


enum eWaterSwitch
{
	WATER_ORIGINAL = 0,
	WATER_FH,
	WATER_PS2,
	WATER_GTA3,
	WATER_MAX,
};

extern const char *aWaterSwitch[];

EXTERN_TWEAKSWITCH(g_Switch);

/*
EXTERN_TWEAKFLOAT(fEnvScale);
EXTERN_TWEAKFLOAT(fWave2InvLength);
EXTERN_TWEAKFLOAT(fWave2NormScale);
EXTERN_TWEAKFLOAT(fWave2Ampl);
EXTERN_TWEAKFLOAT(fRandomMoveDiv);
EXTERN_TWEAKFLOAT(fRandomDamp);
EXTERN_TWEAKFLOAT(fNormMult);
EXTERN_TWEAKFLOAT(fNormMultB);
EXTERN_TWEAKFLOAT(fNormalDirectionScalar1);
EXTERN_TWEAKFLOAT(fNormalDirectionScalar2);
EXTERN_TWEAKFLOAT(fSeaBedZ);
EXTERN_TWEAKFLOAT(fFlatWaterBlendRange);
EXTERN_TWEAKFLOAT(fStartBlendDistanceAdd);
EXTERN_TWEAKFLOAT(fMinWaterAlphaMult);
*/
EXTERN_TWEAKFLOAT(fHugeSectorNearDist);
EXTERN_TWEAKFLOAT(fTransparentFarDist);
EXTERN_TWEAKFLOAT(fTransparentFarDistInBoat);

class CSettings
{
public:
	CSettings();
	
	void Load(Bool bReload = false);
	void Save();
	void Reset();
	
	void Apply();
	void ApplyInGame();
	
	static CSettings &Get();
};
