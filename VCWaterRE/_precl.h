#pragma once
#include "game\game.h"

#define NAKED                            __declspec(naked)
#define NAK                              NAKED

#define ARRLEN(a)                        (sizeof(a)/sizeof(a[0]))
#define _ARRAY_SIZE(array)               (sizeof(array) / sizeof(array[0]))
#define ARRAY_ELEMENT_SIZE(array)        (sizeof(array[0]))
#define ARRAY_SIZE                       _ARRAY_SIZE

#define WRAPPER __declspec(naked)
#define EAXJMP(a)                        { _asm mov eax, a _asm jmp eax }
#define VARJMP(a)                        { _asm jmp a }

#define BIT(num) (1<<(num))

#define PI ((float)3.1415)
// 6.28f
#define _2PI (PI*2)

#define DEG2RAD(x) ((x) * PI / 180.0f)
#define RAD2DEG(x) ((x) * 180.0f / PI)

#define PERCENT(x, p) ( ( Float(x) * ( Float(p) / 100.0f ) ) )
#define LIMIT_ANGLE(v, min, max) ( ( v >= max ) ? ( v - max ) : ( ( v < min ) ? ( v + max ) : (v) ) )

#define clamp(v, a, b) (max(min(v, b), a))

#ifndef max
    #define max(a, b)                    (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
    #define min(a, b)                    (((a) > (b)) ? (b) : (a))
#endif

#define STRCONVERT(x)                    #x
#define STR(x)                           STRCONVERT(x)
#define STRINGIFY                        STRCONVERT
#define TOSTRING                         STR
