#include "Tweak.h"
#include <stdint.h>
#include "debugmenu_public.h"
#include "Errors.h"
#include "utils.h"

void CTweakVars::Add(CTweakVar *var)
{
	m_TweakList.push_back(var);
}

void CTweakVars::Reset()
{
	for(auto i = m_TweakList.begin(); i != m_TweakList.end(); ++i)
		(*i)->Reset();
}

void CTweakVars::ReadINI(CIniReader *pIni)
{
	for(auto i = m_TweakList.begin(); i != m_TweakList.end(); ++i)
		(*i)->ReadINI(pIni);
}

void CTweakVars::WriteINI(CIniReader *pIni)
{
	for(auto i = m_TweakList.begin(); i != m_TweakList.end(); ++i)
		(*i)->WriteINI(pIni);
}

void CTweakVars::AddDBG(const std::string &path)
{
	for(auto i = m_TweakList.begin(); i != m_TweakList.end(); ++i)
		(*i)->AddDBG(path);
}

//////////////////////////////////////////////////////////////////////////////////////
static char* GetIniSection(const std::string &path)
{
	if ( path.empty() )
		return "MAIN";
	
	return(char *) path.c_str();
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakSwitch::CTweakSwitch(UInt32 *pInt, const std::string &strName, UInt32 nDefVal,
			UInt32 nMax, const char **aStr, void (*pFunc)(), const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nMax(nMax),
	m_aStr(aStr),
	m_pFunc(pFunc)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakSwitch::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakSwitch::ReadINI(CIniReader *pIni)
{
	std::string val = pIni->ReadString(GetIniSection(m_path), (char *)m_strVarName.c_str(), (char *)m_aStr[m_nDefVal]);

	*m_pIntVar = m_nMax + 1;	// max+1 const for unknown value, yes it's stupid
	
	for ( Int32 i = 0; i < m_nMax; i++)
	{
		if ( !strcmp(ToUpper(val).c_str(), m_aStr[i]) )
		{
			*m_pIntVar = i;
			break;
		}
	}
	
	if ( *m_pIntVar == m_nMax + 1 )
		FATAL("Invalid value: %s", val.c_str());
}

void CTweakSwitch::WriteINI(CIniReader *pIni)
{
	pIni->WriteString(GetIniSection(m_path), (char *)m_strVarName.c_str(), (char *)m_aStr[*m_pIntVar]);
}

void CTweakSwitch::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuEntry *e = DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(),
		(uint32_t *)m_pIntVar, m_pFunc, 1, 0, m_nMax - 1, m_aStr);
	DebugMenuEntrySetWrap(e, true);
}

//////////////////////////////////////////////////////////////////////////////////////
CTweakBool::CTweakBool(Bool *pBool, const std::string &strName, Bool bDefVal, const std::string &strPath) :
	m_pBoolVar(pBool),
	m_bDefVal(bDefVal),
	m_strVarName(strName),
	m_path(strPath)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakBool::Reset()
{
	*m_pBoolVar = m_bDefVal;
}

void CTweakBool::ReadINI(CIniReader *pIni)
{
	*m_pBoolVar = pIni->ReadBoolean(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_bDefVal);
}

void CTweakBool::WriteINI(CIniReader *pIni)
{
	pIni->WriteBoolean(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pBoolVar);
}

void CTweakBool::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVarBool8(mpath.c_str(), m_strVarName.c_str(), (int8_t *)m_pBoolVar, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakFloat::CTweakFloat(Float *pFloat, const std::string &strName, Float fDefVal,
			Float fLower, Float fUpper, Float fStep, const std::string &strPath) :
	m_pFloatVar(pFloat),
	m_fDefVal(fDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_fLoawerBound(fLower),
	m_fUpperBound(fUpper),
	m_fStep(fStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakFloat::Reset()
{
	*m_pFloatVar = m_fDefVal;
}

void CTweakFloat::ReadINI(CIniReader *pIni)
{
	*m_pFloatVar = pIni->ReadFloat(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_fDefVal);
}

void CTweakFloat::WriteINI(CIniReader *pIni)
{
	pIni->WriteFloat(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pFloatVar);
}

void CTweakFloat::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (float *)m_pFloatVar, NULL, m_fStep, m_fLoawerBound, m_fUpperBound);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakInt8::CTweakInt8(Int8 *pInt, const std::string &strName, Int8 nDefVal,
			Int8 nLower, Int8 nUpper, Int8 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakInt8::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakInt8::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakInt8::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakInt8::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (int8_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakUInt8::CTweakUInt8(UInt8 *pInt, const std::string &strName, UInt8 nDefVal,
			UInt8 nLower, UInt8 nUpper, UInt8 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakUInt8::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakUInt8::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakUInt8::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakUInt8::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (uint8_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakInt16::CTweakInt16(Int16 *pInt, const std::string &strName, Int16 nDefVal,
			Int16 nLower, Int16 nUpper, Int16 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakInt16::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakInt16::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakInt16::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakInt16::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (int16_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakUInt16::CTweakUInt16(UInt16 *pInt, const std::string &strName, UInt16 nDefVal,
			UInt16 nLower, UInt16 nUpper, UInt16 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakUInt16::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakUInt16::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakUInt16::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakUInt16::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (uint16_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakInt32::CTweakInt32(Int32 *pInt, const std::string &strName, Int32 nDefVal,
			Int32 nLower, Int32 nUpper, Int32 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakInt32::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakInt32::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakInt32::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakInt32::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (int32_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////

CTweakUInt32::CTweakUInt32(UInt32 *pInt, const std::string &strName, UInt32 nDefVal,
			UInt32 nLower, UInt32 nUpper, UInt32 nStep, const std::string &strPath) :
	m_pIntVar(pInt),
	m_nDefVal(nDefVal),
	m_strVarName(strName),
	m_path(strPath),

	m_nLoawerBound(nLower),
	m_nUpperBound(nUpper),
	m_nStep(nStep)
{
	CTweakVars::Get().Add(this);
}
	
void CTweakUInt32::Reset()
{
	*m_pIntVar = m_nDefVal;
}

void CTweakUInt32::ReadINI(CIniReader *pIni)
{
	*m_pIntVar = pIni->ReadInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), m_nDefVal);
}

void CTweakUInt32::WriteINI(CIniReader *pIni)
{
	pIni->WriteInteger(GetIniSection(m_path), (char *)m_strVarName.c_str(), *m_pIntVar);
}

void CTweakUInt32::AddDBG(const std::string &path)
{
	std::string mpath = std::string(PROJECT_NAME);

	if ( !path.empty() )
		mpath += std::string("|") + path;
	
	if ( !m_path.empty() )
		mpath += std::string("|") + m_path;
	
	DebugMenuAddVar(mpath.c_str(), m_strVarName.c_str(), (uint32_t *)m_pIntVar, NULL, m_nStep, m_nLoawerBound, m_nUpperBound, NULL);
}