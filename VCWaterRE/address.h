#pragma once
#include ".\game\types.h"

//#if 1
#define VALIDATE_SIZE(struc, size) static_assert(sizeof(struc) == size, "Invalid structure size of " #struc)
#define VALIDATE_OFFSET(struc, member, offset) \
	static_assert(offsetof(struc, member) == offset, "The offset of " #member " in " #struc " is not " #offset "...")
//#endif

struct tDummy { };
static const tDummy dummy_func;

#define FUNCVMT(index) (*(void ***)this)[index]

//#define DUMMY(c) c(tDummy vmt)
#define DUMMY(c) c(tDummy) {}
#define DUMMYB(c, b) c(tDummy vmt) : b(vmt) {}
#define DUMMYCTOR(b) b(dummy_func)


#define ADDR_RSGLOBAL                       0x9B48D8                     
#define ADDR_RWENGINEINSTANCE               0x7870C0          
#define ADDR_SCENE                          0x8100B8
