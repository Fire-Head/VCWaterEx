#pragma once
#include "CEntity.h"
#include "CPlaceable.h"
#include "CVector.h"
#include "CColPoint.h"
#include "..\address.h"

class CEntryInfoList {
public:
    void *m_pLastEntry;
};
//#pragma pack(push, 4)
class CPhysical : public CEntity
{
protected:
	DUMMYB(CPhysical, CEntity);
public:
	 unsigned int m_nAudioEntityId;
    float fUnknownX;
    float fUnknownY;
    CVector m_vecMoveSpeed;
    CVector m_vecTurnSpeed;
    CVector m_vecFrictionMoveForce;
    CVector m_vecFrictionTurnForce;
    CVector m_vecForce;
    CVector m_vecTorque;
    float m_fMass;
    float m_fTurnMass;
    float m_fVelocityFrequency;
    float m_fAirResistance;
    float m_fElasticity;
    float m_fBuoyancyConstant;
    CVector m_vecCentreOfMass;
    CEntryInfoList m_collisionList;
    void *m_pMovingListNode;
    unsigned char uCollideExtra;                        
    unsigned char uCollideInfo;
    unsigned char m_nNumCollisionRecords;
    char field_E7;
    CEntity *m_apCollisionRecords[6];
    float m_fTotSpeed;
    float m_fCollisionPower;
    CPhysical *m_pPhysColliding;
    CVector m_vecCollisionPower;
    unsigned short m_wComponentCol;
    unsigned char m_nMoveFlags;
    unsigned char m_nCollFlags;
    unsigned char m_nLastCollType;
    unsigned char m_nZoneLevel;
    char field_11E[2];
	
	CPhysical();
	virtual ~CPhysical();

	void ApplyMoveForce(float fX, float fY, float fZ)
	{
		((void (__thiscall *)(CPhysical*, float, float, float))0x4BADC0)(this, fX, fY, fZ);
	}
};
//#pragma pack(pop)

VALIDATE_SIZE(CPhysical, 0x120);