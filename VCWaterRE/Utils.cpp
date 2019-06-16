#include "Utils.h"
#include <Windows.h>
#include <fstream>
#include <algorithm>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;


std::string GetAsiPath()
{
	char			moduleName[MAX_PATH] = {'\0'};
	static char		dllPath[MAX_PATH];
	char* tempPointer = NULL;

	dllPath[0] = '\0';

	GetModuleFileName((HINSTANCE)&__ImageBase, moduleName, MAX_PATH);
	tempPointer = strrchr(moduleName, '.');
	*tempPointer = '\0';
	tempPointer = strrchr(moduleName, '\\');
	strncpy_s(dllPath, moduleName, (tempPointer - moduleName + 1));
	dllPath[tempPointer - moduleName + 1] = '\0';

	return dllPath;
}

std::string GetAsiName()
{
	char			moduleName[MAX_PATH] = {'\0'};
	GetModuleFileNameA((HINSTANCE)&__ImageBase, moduleName, MAX_PATH);
	
	const std::string moduleFileName = moduleName;
	
    return moduleFileName.substr(moduleFileName.find_last_of("/\\") + 1);
}

static void * FUCK_FUCK_FUCK_RwEngineInstance = *(void **)ADDR_RWENGINEINSTANCE;

bool IsVersion_1_0()
{
	return (*(unsigned int*)0x667BF5 == 0xB85548EC);
}

bool IsAsiLoaderUsed()
{
	return FUCK_FUCK_FUCK_RwEngineInstance == NULL;
	return true;
}

bool file_exists(const char *name)
{
    std::ifstream f(name);
    return f.good();
}

std::string ToUpper(const std::string &str)
{
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		return s;
}

std::string ToLower(const std::string &str)
{
		std::string s(str);
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		return s;
}