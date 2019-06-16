#include <iostream>
#include <Windows.h>

#define MAX_INI 1024

#ifndef INIREADER_H
#define INIREADER_H
class CIniReader
{
public:
    CIniReader(char* szFileName); 
	void SetIni(char* szFileName); 
    int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
    float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
    bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
    char* ReadString(char* szSection, char* szKey, const char* szDefaultValue);
    void WriteInteger(char* szSection, char* szKey, int iValue);
    void WriteFloat(char* szSection, char* szKey, float fltValue);
    void WriteBoolean(char* szSection, char* szKey, bool bolValue);
    void WriteString(char* szSection, char* szKey, char* szValue);
	bool SectionExists(char* szSection);
	
    char* GetIniPath();
	bool IniExists();
private:
    char m_szFileName[MAX_INI];
};
#endif//INIREADER_H