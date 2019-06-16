#include "settings.h"
#include "Errors.h"
#include "inc/IniReader.h"
#include "Tweak.h"
#include "CWaterLevelEx.h"

const char *aWaterSwitch[] =
{
	"PC",
	"FH",
	"PS2",
	"GTA3",
};

void onSwitchChange();
SETTWEAKPATH("");
TWEAKSWITCH(g_Switch, WATER_ORIGINAL, WATER_MAX, aWaterSwitch, onSwitchChange);
TWEAKFLOAT(fHugeSectorNearDist,             500.0f, 0.0f,  100000.0f, 10.0f);
TWEAKFLOAT(fTransparentFarDist,             70.0f,  0.0f,  100000.0f, 10.0f);
TWEAKFLOAT(fTransparentFarDistInBoat,       120.0f, 0.0f,  100000.0f, 10.0f);


void onSwitchChange()
{
	CWaterLevelEx::Patch(g_Switch == WATER_ORIGINAL);
}

CSettings::CSettings()
{
	Reset();
}


void CSettings::Load(Bool bReload)
{
	CIniReader Ini("");

	CTweakVars::Get().ReadINI(&Ini);
	
	if ( bReload )
		ApplyInGame();
	else
		Apply();
}

void CSettings::Save()
{
	CIniReader Ini("");
	
	CTweakVars::Get().WriteINI(&Ini);
}

void CSettings::Apply()
{
	CWaterLevelEx::Patch(g_Switch == WATER_ORIGINAL);
	
	//CParticleEx::SetParticleSwitch((eParticleSwitch)m_nParticleSwitch);
	//CParticleArray::ReSize(m_nParticleLimit);
}

void CSettings::ApplyInGame()
{
	CWaterLevelEx::Patch(g_Switch == WATER_ORIGINAL);
}

void CSettings::Reset()
{
	CTweakVars::Get().Reset();
	
	CWaterLevelEx::Patch(g_Switch == WATER_ORIGINAL);
}

CSettings &CSettings::Get()
{
	static CSettings settings;
	
	return settings;
}
