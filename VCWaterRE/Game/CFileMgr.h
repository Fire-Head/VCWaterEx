#pragma once
#include <windows.h>
#include "..\address.h"

class CFileMgr
{
public:
	static int LoadTextFile(char const *path, unsigned char *buf, int size, char  const* mode);
	static void SetDir(char const *dir);
	
	static int Read(int file, char *buf, int size);
	static int Write(int file, char *buf, int size);
	
	static int OpenFile(char const *path, char const *mode);
	static int CloseFile(int file);
};