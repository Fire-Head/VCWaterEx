#pragma once
#include <Windows.h>
#include "RenderWare.h"

class CRGBA
{
public:
	union{
	    unsigned int colorInt;
		struct{
		    unsigned char r;
	        unsigned char g;
	        unsigned char b;
	        unsigned char a;
		};
		struct{
			unsigned char red;
			unsigned char green; 
			unsigned char blue;
			unsigned char alpha;
		};
		
#ifdef RWCORE_H
		struct 
		{
			RwRGBA rwRGBA;
		};
#endif
	};

#ifdef RWCORE_H
		operator RwRGBA &(void)
		{
			return rwRGBA;
		}
		
		operator RwRGBA *(void)
		{
			return &rwRGBA;
		}
		
		operator RwRGBA (void) const
		{
			return rwRGBA;
		}
#endif

	CRGBA() : r(0), g(0), b(0), a(0){}
	CRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a){}
};
