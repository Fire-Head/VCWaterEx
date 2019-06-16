#pragma once

#include "RenderWare.h"
#include "..\address.h"

class CTexDictionary
{
public:
	// class variables
	RwTexDictionary *m_pRwDictionary;
	unsigned __int16 m_dwRefsCount;
	__int16 m_dwParentIndex;
	unsigned __int32 hash;
};

class CTxdStore
{
public:
	static int AddTxdSlot(char  const* name);
	static void RemoveTxdSlot(int index);
	static int FindTxdSlot(char  const* name);
	static bool LoadTxd(int index, char  const* filename);

	static void SetCurrentTxd(int index);
	static void PushCurrentTxd();
	static void PopCurrentTxd();
	static CTexDictionary *AddRef(int index);
};