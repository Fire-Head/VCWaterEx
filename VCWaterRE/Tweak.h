#pragma once
#include "_precl.h"
#include <string>
#include <list>
#include "_config.h"
#include "Game/types.h"
#include "inc/singleton.h"
#include "inc/IniReader.h"


#define TWEAKPATH ___tw___TWEAKPATH

#define SETTWEAKPATH(path)	\
	static std::string ___tw___TWEAKPATH = path;

// interface
class CTweakVar
{
public:
	// reset variable to default value
	virtual void Reset() = 0;
	
	// read variable value from an .ini file
	virtual void ReadINI(CIniReader *pIni) = 0;
	
	// write variable value to the .ini file
	virtual void WriteINI(CIniReader *pIni) = 0;
	
	// add variable to aap's debugmenu
	virtual void AddDBG(const std::string &path) = 0;
};

class CTweakVars : public ISingleton<CTweakVars>
{
	std::list<CTweakVar *> m_TweakList;

public:
	void Reset();
	void Add(CTweakVar *var);
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path = "");
};

#define TWEAKSWITCH(name, value, max, astr, func)	\
	UInt32 name = value; \
	static CTweakSwitch ___tw___tweak##name(&name, STR(name), value, max, astr, func, TWEAKPATH);
	
#define EXTERN_TWEAKSWITCH(name) extern UInt32 name;
	
class CTweakSwitch : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	UInt32 *m_pIntVar;	
	UInt32 m_nDefVal;
	
	UInt32 m_nMax;
	const char **m_aStr;
	void (*m_pFunc)();
	
public:
	CTweakSwitch(UInt32 *pInt, const std::string &strName, UInt32 nDefVal,
			UInt32 nMax, const char **aStr, void (*pFunc)(), const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKBOOL(name, value)	\
	Bool name = value; \
	static CTweakBool ___tw___tweak##name(&name, STR(name), value, TWEAKPATH);
	
#define EXTERN_TWEAKBOOL(name) extern Bool name;

class CTweakBool : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	Bool *m_pBoolVar;	
	Bool m_bDefVal;

public:
	CTweakBool(Bool *pBool, const std::string &strName, Bool bDefVal, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path = "");
};

#define TWEAKFLOAT(name, value, lower, upper, step)	\
	Float name = value; \
	static CTweakFloat ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKFLOAT(name) extern Float name;

class CTweakFloat : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	Float *m_pFloatVar;	
	Float m_fDefVal;
	
	Float m_fLoawerBound;
	Float m_fUpperBound;
	Float m_fStep;
	
public:
	CTweakFloat(Float *pFloat, const std::string &strName, Float fDefVal,
			Float fLower, Float fUpper, Float fStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKINT8(name, value, lower, upper, step)	\
	Int8 name = value; \
	static CTweakInt8 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKINT8(name) extern Int8 name;
	
class CTweakInt8 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	Int8 *m_pIntVar;	
	Int8 m_nDefVal;
	
	Int8 m_nLoawerBound;
	Int8 m_nUpperBound;
	Int8 m_nStep;
	
public:
	CTweakInt8(Int8 *pInt, const std::string &strName, Int8 nDefVal,
			Int8 nLower, Int8 nUpper, Int8 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKUINT8(name, value, lower, upper, step)	\
	UInt8 name = value; \
	static CTweakUInt8 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKUINT8(name) extern UInt8 name;
	
class CTweakUInt8 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	UInt8 *m_pIntVar;	
	UInt8 m_nDefVal;
	
	UInt8 m_nLoawerBound;
	UInt8 m_nUpperBound;
	UInt8 m_nStep;
	
public:
	CTweakUInt8(UInt8 *pInt, const std::string &strName, UInt8 nDefVal,
			UInt8 nLower, UInt8 nUpper, UInt8 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKINT16(name, value, lower, upper, step)	\
	Int16 name = value; \
	static CTweakInt16 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKINT16(name) extern Int16 name;
	
class CTweakInt16 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	Int16 *m_pIntVar;	
	Int16 m_nDefVal;
	
	Int16 m_nLoawerBound;
	Int16 m_nUpperBound;
	Int16 m_nStep;
	
public:
	CTweakInt16(Int16 *pInt, const std::string &strName, Int16 nDefVal,
			Int16 nLower, Int16 nUpper, Int16 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKUINT16(name, value, lower, upper, step)	\
	UInt16 name = value; \
	static CTweakUInt16 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKUINT16(name) extern UInt16 name;
	
class CTweakUInt16 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	UInt16 *m_pIntVar;	
	UInt16 m_nDefVal;
	
	UInt16 m_nLoawerBound;
	UInt16 m_nUpperBound;
	UInt16 m_nStep;
	
public:
	CTweakUInt16(UInt16 *pInt, const std::string &strName, UInt16 nDefVal,
			UInt16 nLower, UInt16 nUpper, UInt16 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKINT32(name, value, lower, upper, step)	\
	Int32 name = value; \
	static CTweakInt32 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKINT32(name) extern Int32 name;
	
class CTweakInt32 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	Int32 *m_pIntVar;	
	Int32 m_nDefVal;
	
	Int32 m_nLoawerBound;
	Int32 m_nUpperBound;
	Int32 m_nStep;
	
public:
	CTweakInt32(Int32 *pInt, const std::string &strName, Int32 nDefVal,
			Int32 nLower, Int32 nUpper, Int32 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};

#define TWEAKUINT32(name, value, lower, upper, step)	\
	UInt32 name = value; \
	static CTweakUInt32 ___tw___tweak##name(&name, STR(name), value, lower, upper, step, TWEAKPATH);
	
#define EXTERN_TWEAKUINT32(name) extern UInt32 name;
	
class CTweakUInt32 : public CTweakVar
{
	std::string m_path;
	std::string m_strVarName;
	UInt32 *m_pIntVar;	
	UInt32 m_nDefVal;
	
	UInt32 m_nLoawerBound;
	UInt32 m_nUpperBound;
	UInt32 m_nStep;
	
public:
	CTweakUInt32(UInt32 *pInt, const std::string &strName, UInt32 nDefVal,
			UInt32 nLower, UInt32 nUpper, UInt32 nStep, const std::string &strPath);

	void Reset();
	void ReadINI(CIniReader *pIni);
	void WriteINI(CIniReader *pIni);
	void AddDBG(const std::string &path);
};