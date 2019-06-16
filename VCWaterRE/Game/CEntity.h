#pragma once
#include "CPlaceable.h"
#include "RenderWare.h"

class CEntity : public CPlaceable
{
protected:
	DUMMY(CEntity);
public:
	    union {
        RwObject *m_pRwObject;
        RpAtomic *m_pRwAtomic;
        RpClump  *m_pRwClump;
    };

    unsigned char m_nType : 3;
    unsigned char m_nState : 5;

    struct {
        unsigned char bUseCollision : 1;
        unsigned char bEntUFlag02 : 1;
        unsigned char bIsStatic : 1;
        unsigned char bEntUFlag04 : 1;
        unsigned char bEntUFlag05 : 1;
        unsigned char bEntUFlag06 : 1;
        unsigned char bEntUFlag07 : 1;
        unsigned char bRecordCollisions : 1;

        unsigned char bEntUFlag09 : 1;
        unsigned char bExplosionProof : 1;
        unsigned char bIsVisible : 1;
        unsigned char bHasCollided : 1;
        unsigned char bRenderScorched : 1;
        unsigned char bEntUFlag14 : 1;
        unsigned char bUseLevelSectors : 1;
        unsigned char bIsBigBuilding : 1;

        unsigned char bEntUFlag17 : 1;
        unsigned char bBulletProof : 1;
        unsigned char bFireProof : 1;
        unsigned char bCollisionProof : 1;
        unsigned char bMeleeProof : 1;
        unsigned char bImmuneToNonPlayerDamage : 1;
        unsigned char bEntUFlag23 : 1;
        unsigned char bRemoveFromWorld : 1;

        unsigned char bEntUFlag25 : 1;
        unsigned char bImBeingRendered : 1;
        unsigned char bEntUFlag27 : 1;
        unsigned char bEntUFlag28 : 1;
        unsigned char bEntUFlag29 : 1;
        unsigned char bEntUFlag30 : 1;
        unsigned char bEntUFlag31 : 1;
        unsigned char bEntUFlag32 : 1;

        unsigned char bEntUFlag33 : 1;
        unsigned char bDontCastShadowsOn : 1;
        unsigned char bEntUFlag35 : 1;
        unsigned char bIsStaticWaitingForCollision : 1;
        unsigned char bEntUFlag37 : 1;
        unsigned char bEntUFlag38 : 1;
        unsigned char bEntUFlag39 : 1;
        unsigned char bEntUFlag40 : 1;
    } m_nFlags;
private:
    char _pad56[2];
public:
    short m_nScanCode;
    short m_nRandomSeed;
    short m_nModelIndex;
    char m_nLevel;
    unsigned char m_nInterior;
    class CReference *m_pFirstRef;
	
	CEntity();
	virtual ~CEntity();
};

VALIDATE_SIZE(CEntity, 0x64);