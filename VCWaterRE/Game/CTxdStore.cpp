#include "CTxdStore.h"

int CTxdStore::AddTxdSlot(char  const* name)
{
	return ((int (__cdecl *)(char  const*))0x580F00)(name);
}

void CTxdStore::RemoveTxdSlot(int index)
{
	((void (__cdecl *)(int))0x580E90)(index);
}


int CTxdStore::FindTxdSlot(char  const* name)
{
	return ((int (__cdecl *)(char  const*))0x580D70)(name);
}

bool CTxdStore::LoadTxd(int index, char  const* filename)
{
	return ((bool (__cdecl *)(int, char  const*))0x580CD0)(index, filename);
}

void CTxdStore::SetCurrentTxd(int index)
{
	((void (__cdecl *)(int))0x580AD0)(index);
}

void CTxdStore::PushCurrentTxd()
{
	((void (__cdecl *)())0x580AC0)();
}

void CTxdStore::PopCurrentTxd()
{
	((void (__cdecl *)())0x580AA0)();
}

CTexDictionary *CTxdStore::AddRef(int index)
{
	return ((CTexDictionary *(__cdecl *)(int))0x580A60)(index);
}
