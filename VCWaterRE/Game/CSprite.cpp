#include "CSprite.h"


float &CSprite::m_f2DNearScreenZ = *(float*)0x97863C;
float &CSprite::m_f2DFarScreenZ = *(float*)0x97F33C;
	
	
bool CSprite::CalcScreenCoors(RwV3d const &posn, RwV3d *out, float *w, float *h, bool Unk)
{
	return ((bool (__cdecl *)(RwV3d const &, RwV3d *, float *, float *, bool))0x5778B0)(posn, out, w, h, Unk);
}

// Converted from static void __cdecl CSprite::InitSpriteBuffer2D() 0x51C5B0
void CSprite::InitSpriteBuffer2D()
{
	((void (__cdecl *)())0x577740)();
}

// Converted from static void __cdecl CSprite::FlushSpriteBuffer() 0x51C520
void CSprite::FlushSpriteBuffer()
{
	((void (__cdecl *)())0x577790)();
}

// Converted from static void __cdecl CSprite::RenderBufferedOneXLUSprite2D(float x, float y, float halfWidth, float halfHeight, RwRGBA const &color, short alpha, unsigned char intensity) 0x51E3C0 
void CSprite::RenderBufferedOneXLUSprite2D(float x, float y, float halfWidth, float halfHeight, RwRGBA const& color, short alpha, unsigned char intensity)
{
	((void (__cdecl *)(float, float, float, float, RwRGBA const&, short, unsigned char))0x575680)(x, y, halfWidth, halfHeight, color, alpha, intensity);
}

// Converted from static void __cdecl CSprite::RenderBufferedOneXLUSprite2D_Rotate_Dimension(float x, float y, float halfWidth, float halfHeight, RwRGBA const &color, short alpha, float rhw, unsigned char intensity) 0x51E490
void CSprite::RenderBufferedOneXLUSprite2D_Rotate_Dimension(float x, float y, float halfWidth, float halfHeight, RwRGBA const& color, short alpha, float rhw, unsigned char intensity)
{
	((void (__cdecl *)(float, float, float, float, RwRGBA const&, short, float, unsigned char))0x575440)(x, y, halfWidth, halfHeight, color, alpha, rhw, intensity);
}

// Converted from static void __cdecl CSprite::RenderBufferedOneXLUSprite_Rotate_Dimension(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, float rotate, unsigned char aspect) 0x51D5B0
void CSprite::RenderBufferedOneXLUSprite_Rotate_Dimension(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, float rotate, unsigned char aspect)
{
	((void (__cdecl *)(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, float, unsigned char))0x5761F0)(x, y, z, halfWidth, halfHeight, red, green, blue, alpha, rhw, rotate, aspect);
}

// Converted from static void __cdecl CSprite::RenderBufferedOneXLUSprite(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, unsigned char aspect) 0x51C5D0
void CSprite::RenderBufferedOneXLUSprite(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, unsigned char aspect)
{
	((void (__cdecl *)(float, float, float, float, float, unsigned char, unsigned char, unsigned char, short, float, unsigned char))0x577350)(x, y, z, halfWidth, halfHeight, red, green, blue, alpha, rhw, aspect);
}



