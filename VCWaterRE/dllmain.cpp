#include <windows.h>
#include <stdint.h>
#include "debugmenu_public.h"
#include "inc/CPatch.h"
#include "settings.h"
#include "Tweak.h"
#include "utils.h"
#include "Errors.h"
#include "CWaterLevelEx.h"

typedef void (*tCB)();


template<typename T>
inline static void CALL(int address, void *func, T &CB)
{
	if ( *(int*)(address + 1) + (address + 5) != (int)func)
	{
		CB = reinterpret_cast <T>(*(int*)(address + 1) + (address + 5));
		CPatch::RedirectCall(address, func);
	}
}

void SaveSet()
{
	CSettings::Get().Save();
}

void ReLoadSet()
{
	CSettings::Get().Load(true);
}

void ResetSet()
{
	CSettings::Get().Reset();
}


DebugMenuAPI gDebugMenuAPI;


template <void (*CallBack)(), void (*PreCallBack)(), void (*PostCallBack)()> void QUICK_Button()
{
	if ( PreCallBack != NULL )
		PreCallBack();

	CallBack();

	if ( PostCallBack != NULL )
		PostCallBack();

	DMAudio.PlayFrontEndSound(179, 0);
}

void CheckAsi()
{
	CSettings::Get().Load();

	if(DebugMenuLoad())
	{
		CTweakVars::Get().AddDBG();

		DebugMenuAddCmd(PROJECT_NAME"|Settings", "Save Settings", QUICK_Button<SaveSet, NULL, NULL>);
		DebugMenuAddCmd(PROJECT_NAME"|Settings", "ReLoad Settings", QUICK_Button<ReLoadSet, NULL, NULL>);
		DebugMenuAddCmd(PROJECT_NAME"|Settings", "Reset Settings", QUICK_Button<ResetSet, NULL, NULL>);
	}
}

typedef int (*tRsEventHandler)(int, int);
static tRsEventHandler AsiCheckCB;

int __cdecl onAsiCheck(int a, int b)
{
	CheckAsi();

	return AsiCheckCB(a, b);
}


struct CText
{
	unsigned short *Get(char const *);
};

WRAPPER unsigned short *CText::Get(char const *) { EAXJMP(0x584F30); }

CText &TheText = *(CText*)0x94B220;

WRAPPER void _SetHelpMessage(unsigned short *, bool, bool) { EAXJMP(0x55BFC0); }

void NoSeaBedCheat(void)
{
	_SetHelpMessage(TheText.Get("CHEAT1"), true, false);
	CWaterLevelEx::NoSeaBedCheat();
}

void RenderWaterLayersCheat(void)
{
	_SetHelpMessage(TheText.Get("CHEAT1"), true, false);
	CWaterLevelEx::RenderWaterLayersCheat();
}


namespace Cheat
{
	typedef void (*tCB)();
	template<typename T>
	inline static void CALL(int address, void *func, T &CB)
	{
		if ( *(int*)(address + 1) + (address + 5) != (int)func)
		{
			CB = reinterpret_cast <T>(*(int*)(address + 1) + (address + 5));
			CPatch::RedirectCall(address, func);
		}
	}

	struct tCheat
	{
		char String[30];
		tCB Func;
	};
		
	typedef int (*tstrncmp)(const char * str1, const char * str2);
	static std::list<tCheat>CheatsList;
	static tstrncmp AddToPCCheatStringCB;
	
	int __cdecl onAddToPCCheatString(const char * str1, const char * str2)
	{
		for(auto i = CheatsList.begin(); i != CheatsList.end(); ++i)
		{
			if ( !strncmp((*i).String, ((char *)0xA10942), strlen((*i).String)) )
				(*i).Func();
		}
		return AddToPCCheatStringCB(str1, str2);
	}

	void RegisterCheat(char *cheatstring, tCB func)
	{	
		tCheat cheat;
		
		_snprintf(cheat.String, sizeof(cheat.String), cheatstring);
		strrev(cheat.String);
		cheat.Func = func;
		CheatsList.push_back(cheat);
		CALL(0x4ABD54, onAddToPCCheatString, AddToPCCheatStringCB);
	}
};


int PedSetInCarCB(void *pAsoc, CPed *pPed)
{
	return ((int (__cdecl *)(void *, CPed *))0x4FFF30)(pAsoc, pPed);
}

void _PedSetInCarCB(void *pAsoc, CPed *pPed)
{
	PedSetInCarCB(pAsoc, pPed);
	
	if ( pPed->IsPlayer() )
		CWaterLevelEx::AllocateBoatWakeArray();
}


void InitRW()
{
	RwEngineInstance = *(void **)ADDR_RWENGINEINSTANCE;
};


typedef void (*tCallBack)();
typedef void (*tRemoveTxdSlot)(int index);

static tCallBack InitialiseRenderWareCB;
void __cdecl onInitialiseRenderWare()
{
	InitRW();
	InitialiseRenderWareCB();
}

static tRemoveTxdSlot ShutdownRenderWareCB;
void __cdecl onShutdownRenderWare(int index)
{
	ShutdownRenderWareCB(index);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		if ( !IsVersion_1_0() )
			FATAL("Unsupported game version.");
		
		if ( !IsAsiLoaderUsed() )
			FATAL("ASI Loader is missing.");
		
		CWaterLevelEx::Patch(g_Switch == WATER_ORIGINAL);

		CALL(0x5FFAFE, onAsiCheck, AsiCheckCB);

		
		// CWaterLevel::Initialise
		CPatch::RedirectCall(0x4A4D2E, CWaterLevelEx::Initialise);
		
		// CWaterLevel::Shutdown
		CPatch::RedirectCall(0x4A4ABC, CWaterLevelEx::Shutdown);
		
		// CWaterLevel::RenderWater
		CPatch::RedirectCall(0x4A6594, CWaterLevelEx::RenderWater);
		
		// CWaterLevel::RenderTransparentWater
		CPatch::RedirectCall(0x4A65AE, CWaterLevelEx::RenderTransparentWater);
		
		// CWaterLevel::GetWaterLevel
		{
			static int aGetWaterLevel[] = 
			{
			
				0x428769, 0x4402C7, 0x440401, 0x440544, 0x44058E, 0x455ABE, 
				0x47EE06, 0x485B5E, 0x4D286F, 0x4E2B86, 0x4E2D51, 0x4E2E9B, 
				0x4E2FE1, 0x4E3289, 0x4E3454, 0x4E359A, 0x509ABC, 0x509BB3, 
				0x509D25, 0x585C43, 0x58ECFA, 0x59A3DC, 0x59A56C, 0x5A169D, 
				0x5A34BA, 0x5A404A, 0x5A4873, 0x5A9F72, 0x5AA654, 0x5AA7D0, 
				0x5AACE4, 0x5B352C, 0x5CA00B, 0x614017, 0x638465, 
			};
			
			for ( int i = 0; i < ARRLEN(aGetWaterLevel); i++ )
				CPatch::RedirectCall(aGetWaterLevel[i], CWaterLevelEx::GetWaterLevel);
		}
		
		// CWaterLevel::GetWaterLevelNoWaves
		{
			static int aGetWaterLevelNoWaves[] = 
			{
			
				0x4695C0, 0x469965, 0x469DF4, 0x46A227, 0x475F15, 0x48105E, 
				0x4812B2, 0x5256BF, 0x5C5B0E, 
			};
	
			for ( int i = 0; i < ARRLEN(aGetWaterLevelNoWaves); i++ )
				CPatch::RedirectCall(aGetWaterLevelNoWaves[i], CWaterLevelEx::GetWaterLevelNoWaves);
		}
		
		// CWaterLevel::GetWaterNormal
		CPatch::RedirectCall(0x5AA805, CWaterLevelEx::GetWaterNormal);
		
		// CWaterLevel::CalcDistanceToWater
		CPatch::RedirectCall(0x46DD6F, CWaterLevelEx::CalcDistanceToWater);
		
		// CWaterLevel::GetGroundLevel
		CPatch::RedirectCall(0x53CD38, CWaterLevelEx::GetGroundLevel);
		
		// CWaterLevel::GetWaterDepth
		{
			static int aGetWaterDepth[] = 
			{
			
				0x63707E, 0x637385, 
			};
	
			for ( int i = 0; i < ARRLEN(aGetWaterDepth); i++ )
				CPatch::RedirectCall(aGetWaterDepth[i], CWaterLevelEx::GetWaterDepth);
		}
		
		// CWaterLevel::PreCalcWaterGeometry
		CPatch::RedirectCall(0x4A5E40, CWaterLevelEx::PreCalcWaterGeometry);
		
		CPatch::RedirectCall(0x517FA7, _PedSetInCarCB);
		
		////////////////////////////////////////////////////////////////////
		Cheat::RegisterCheat("SEABEDCHEAT", NoSeaBedCheat);
		Cheat::RegisterCheat("WATERLAYERSCHEAT", RenderWaterLayersCheat);
	
		CALL(0x4A51A8, onInitialiseRenderWare, InitialiseRenderWareCB);
		CALL(0x55C88A, onShutdownRenderWare, ShutdownRenderWareCB);
	}
	return TRUE;
}