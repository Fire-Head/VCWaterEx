#pragma once

#include "RenderWare.h"
#include "..\address.h"

class CSprite
{
public:
	static float &m_f2DNearScreenZ;
	static float &m_f2DFarScreenZ;
	
	static bool CalcScreenCoors(RwV3d const &posn, RwV3d *out, float *w, float *h, bool Unk);
	static void InitSpriteBuffer2D();
	static void FlushSpriteBuffer();
	static void RenderBufferedOneXLUSprite2D(float x, float y, float halfWidth, float halfHeight, RwRGBA const& color, short alpha, unsigned char intensity);
	static void RenderBufferedOneXLUSprite2D_Rotate_Dimension(float x, float y, float halfWidth, float halfHeight, RwRGBA const& color, short alpha, float rhw, unsigned char intensity);
	static void RenderBufferedOneXLUSprite_Rotate_Dimension(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, float rotate, unsigned char aspect);
	static void RenderBufferedOneXLUSprite(float x, float y, float z, float halfWidth, float halfHeight, unsigned char red, unsigned char green, unsigned char blue, short alpha, float rhw, unsigned char aspect);

};