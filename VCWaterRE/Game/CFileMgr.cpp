#include "CFileMgr.h"

int CFileMgr::LoadTextFile(const char *path, unsigned char *buf, int size, const char *mode)
{
	return ((int (__cdecl *)(const char *, unsigned char *, int, const char *))0x48DFB0)(path, buf, size, mode);
}

void CFileMgr::SetDir(char const *dir)
{
	((void (__cdecl *)(char const *))0x48E030)(dir);
}


int CFileMgr::Read(int file, char *buf, int size)
{
	return ((int (__cdecl *)(int, char *, int))0x48DF50)(file, buf, size);
}

int CFileMgr::Write(int file, char *buf, int size)
{
	return ((int (__cdecl *)(int, char *, int))0x48DF30)(file, buf, size);
}

int CFileMgr::OpenFile(const char *path, const char *mode)
{
	return ((int (__cdecl *)(const char *, const char *))0x48DF90)(path, mode);
}

int CFileMgr::CloseFile(int file)
{
	return ((int (__cdecl *)(int))0x48DEA0)(file);
}

