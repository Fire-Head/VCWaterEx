#pragma once
#include "..\address.h"

class CRect
{
public:
	union{	
		struct{
			float x1;
			float y2;
			float x2;
			float y1;
		};
		
		struct{
			float left;
			float bottom;
			float right;
			float top;
		};
	};
	CRect()
	{
		left = 1000000.0f;
		top = 1000000.0f;
		right = -1000000.0f;
		bottom = -1000000.0f;
	}
	
	CRect(float fLeft, float fTop, float fRight, float fBottom)
	{
		left = fLeft;
		top = fTop;
		right = fRight;
		bottom = fBottom;
	}
};