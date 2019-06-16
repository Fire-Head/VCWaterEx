#pragma once
#include <stdio.h>

enum eAssertType
{
	ASSTYPE_ASSERT = 0,
	ASSTYPE_ERROR,
	ASSTYPE_FATAL_ERROR,
};

extern void FH_Assert(eAssertType as, const char *file, int line, const char *func, const char *expr, const char *msg = "", ...);

#define DebuggerBreak()	__asm int 3
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)

#define VALIDATE_OFFSET(struc, member, offset) static_assert(offsetof(struc, member) == offset, "The offset of " #member " in " #struc " is not " #offset "...")


#define ASSERT(_expr, ...)    do {if(!(_expr)){FH_Assert(ASSTYPE_ASSERT, __FILE__,__LINE__,__FUNCTION__,#_expr,__VA_ARGS__);}}while(false);
#define VERIFY(_expr, ...)    do {if(!(_expr)){FH_Assert(ASSTYPE_ASSERT, _MSG_VERIFY, __FILE__,__LINE__,__FUNCTION__,#_expr,__VA_ARGS__);}}while(false);
#define CHECK(_expr, ...)     do {if((_expr)){FH_Assert(ASSTYPE_ERROR, __FILE__,__LINE__,__FUNCTION__,#_expr,__VA_ARGS__);}}while(false);
#define FATAL(...)            do {FH_Assert(ASSTYPE_FATAL_ERROR,__FILE__,__LINE__,__FUNCTION__,NULL,__VA_ARGS__);}while(false);
//#define ERROR(...)            do {FH_Assert(ASSTYPE_ERROR,__FILE__,__LINE__,__FUNCTION__,NULL,__VA_ARGS__);}while(false);
