#pragma once
#include "..\address.h"
#include "CVehicle.h"
#include "CVector2D.h"



class CBoat : public CVehicle
{
protected:
	DUMMYB(CBoat, CVehicle);
public:
class CDoor {
public:
    float fAngleInPosOne;
    float fAngleInPosTwo;
    unsigned char nAxisDirection;
    unsigned char nAxis;
    unsigned char nState;
    unsigned char __f000B;
    float fAngle;
    float fPrevAngle;
    float fVelAngle;
    CVector vecVelocity;
};

	float m_fPropRotation;
    float m_fPropSpeed;
    int field_2A8;
    void *m_aBoatNodes[8];
    CDoor m_boatDoor;
    void *m_pBoatHandling;
    union
    {
        struct
        {
            unsigned char bUnknownBoatFlagsA : 1; 
            unsigned char bIsBoatInWater : 1; 
            unsigned char bUnknownBoatFlagsB : 6; 

        } m_nBoatFlags;
    };
    char field_2F5;
    char gap_2F6[2];
    float m_fForcedZRotation;
    int m_nAttackPlayerTime;
    int field_300;
    float m_fBurningTimer;
    void *m_pWhoDestroyedMe;
    float field_30C;
    int field_310;
    float m_fBoatGasPedal;
    float m_fBoatBrakePedal;
    float m_fBoatSteeringLeftRight;
    unsigned char m_nPadNumber;
    char field_321[3];
    char gap_324[4];
    float field_328;
    float field_32C; // x
    float field_330; // y
    float field_334; // z
    float field_338;
    short field_33C;
    unsigned short m_nNumWaterTrailPoints;
    CVector2D m_avecWakePoints[32];
    float m_afWakePointLifeTime[32];

    static float &WAKE_LIFETIME; // 150.0
    static float &MIN_WAKE_INTERVAL; // 2.0
	
	static CBoat **apFrameWakeGeneratingBoats;
	
	static void FillBoatList(void);
};