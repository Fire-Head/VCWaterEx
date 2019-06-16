#include "CShadows.h"

// Converted from static void __cdecl CShadows::AddPermanentShadow(unsigned char ShadowType, RwTexture *pTexture, CVector *pPosn, float fX1, float fY1, float fX2, float fY2, short nTransparency, unsigned char nRed, unsigned char nGreen, unsigned char nBlue, float fZDistance, unsigned int nTime, float fScale) 0x512FD0
void CShadows::AddPermanentShadow(unsigned char ShadowType, RwTexture* pTexture, CVector* pPosn, float fX1, float fY1, float fX2, float fY2, short nTransparency, unsigned char nRed, unsigned char nGreen, unsigned char nBlue, float fZDistance, unsigned int nTime, float fScale)
{
	((void (__cdecl *)(unsigned char, RwTexture*, CVector*, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, unsigned int, float))0x56EC50)(ShadowType, pTexture, pPosn, fX1, fY1, fX2, fY2, nTransparency, nRed, nGreen, nBlue, fZDistance, nTime, fScale);
}
