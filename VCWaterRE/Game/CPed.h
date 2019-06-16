#pragma once
#include <windows.h>
#include "CPhysical.h"
#include "CVector2D.h"
#include "RenderWare.h"
#include "..\address.h"



class CWeapon {
public:
    int m_Type;
    int m_nState;
    unsigned int m_nAmmoInClip;
    unsigned int m_nTotalAmmo;
    unsigned int m_nNextShotTime;
    bool m_bAddRotOffset;
private:
    char _pad15[3];
};

VALIDATE_SIZE(CWeapon, 0x18);

struct LimbOrientation
{
public:
    float m_fYaw;
    float m_fPitch;
};

VALIDATE_SIZE(LimbOrientation, 0x8);

struct LimbMovementInfo
{
    float maxYaw, minYaw;
    float yawD;
    float maxPitch, minPitch;
    float pitchD;
};
VALIDATE_SIZE(LimbMovementInfo, 0x18);


#pragma pack(push, 1)
struct struct_4C50C0
{
  DWORD field_0;
  DWORD field_4;
};
#pragma pack(pop)


class CPedIK
{
public:
   class CPed * m_pPed;
   LimbOrientation m_aLimbOrien[2];
   float m_fSlopePitch;
   float m_fSlopePitchLimitMult;
   float m_fSlopeRoll;
   float m_fBodyRoll;
	union
    {
        unsigned int m_nFlags;
        struct
        {
            unsigned int bGunReachedTarget : 1;
            unsigned int bTorsoUsed : 1;
            unsigned int bUseArm : 1;
            unsigned int bSlopePitch : 1;
        };
    };
};

VALIDATE_SIZE(CPedIK, 0x28);



class CPathNode {
public:
    short m_wPathData1;
    short m_wPathData2;
    short m_wPosX;
    short m_wPosY;
    short m_wPosZ;
    char field_A[2];
    short field_C;
    char field_E[6];
};

VALIDATE_SIZE(CPathNode, 0x14);

class CPed : public CPhysical
{

	class CStoredCollPoly {
public:
    CVector m_aVertices[3]; // triangle vertices
    Bool m_bIsActual;
private:
    char _pad[3];
};
protected:
	DUMMYB(CPed, CPhysical);

public:
    CStoredCollPoly m_polyColliding;
    float m_fCollisionSpeed;
    struct {
        unsigned char bIsStanding : 1;
        unsigned char bWasStanding : 1;
        unsigned char b03 : 1;
        unsigned char bIsPointingGunAt : 1;
        unsigned char bIsLooking : 1;
        unsigned char b06 : 1;
        unsigned char bIsRestoringLook : 1;
        unsigned char bIsAimingGun : 1;

        unsigned char bIsRestoringGun : 1;
        unsigned char bCanPointGunAtTarget : 1;
        unsigned char bIsTalking : 1;
        unsigned char bIsInTheAir : 1;
        unsigned char bIsLanding : 1;
        unsigned char bIsRunning : 1;
        unsigned char b15 : 1;
        unsigned char b16 : 1;

        unsigned char bCanPedEnterSeekedCar : 1;
        unsigned char bRespondsToThreats : 1;
        unsigned char bRenderPedInCar : 1;
        unsigned char b20 : 1;
        unsigned char bUpdateAnimHeading : 1;
        unsigned char bRemoveHead : 1;
        unsigned char bFiringWeapon : 1;
        unsigned char b24 : 1;

        unsigned char b25a : 1;
        unsigned char b25b : 1;
        unsigned char bPedIsBleeding : 1;
        unsigned char bStopAndShoot : 1;
        unsigned char bIsPedDieAnimPlaying : 1;
        unsigned char b30 : 1;
        unsigned char b31 : 1;
        unsigned char b32 : 1;

        unsigned char bKindaStayInSamePlace : 1;
        unsigned char bBeingChasedByPolice : 1;
        unsigned char bNotAllowedToDuck : 1;
        unsigned char bCrouchWhenShooting : 1;
        unsigned char bIsDucking : 1;
        unsigned char bGetUpAnimStarted : 1;
        unsigned char bDoBloodyFootprints : 1;
        unsigned char b40 : 1;

        unsigned char b41 : 1;
        unsigned char b42 : 1;
        unsigned char b43 : 1;
        unsigned char b44 : 1;
        unsigned char b45 : 1;
        unsigned char b46 : 1;
        unsigned char b47 : 1;
        unsigned char b48 : 1;

        unsigned char b49 : 1;
        unsigned char b50 : 1;
        unsigned char b51 : 1;
        unsigned char b52 : 1;
        unsigned char b53 : 1;
        unsigned char b54 : 1;
        unsigned char bIsBeingJacked : 1;
        unsigned char bFadeOut : 1;

        unsigned char b57 : 1;
        unsigned char b58 : 1;
        unsigned char b59 : 1;
        unsigned char bClearObjective : 1;
        unsigned char b61 : 1;
        unsigned char b62 : 1;
        unsigned char b63 : 1;
        unsigned char b64 : 1;

        unsigned char bShakeFist : 1;
        unsigned char bNoCriticalHits : 1;
        unsigned char b67 : 1;
        unsigned char bHasAlreadyBeenRecorded : 1;
        unsigned char bFallenDown : 1;
        unsigned char bUpdateMatricesRequired : 1;
        unsigned char b71 : 1;
        unsigned char b72 : 1;

        unsigned char b73 : 1;
        unsigned char b74 : 1;
        unsigned char b75 : 1;
        unsigned char b76 : 1;
        unsigned char b77 : 1;
        unsigned char bMiamiViceCop : 1;
        unsigned char bMoneyHasBeenGivenByScript : 1;
        unsigned char bHasBeenPhotographed : 1;

        unsigned char bIsDrowning : 1;
        unsigned char bDrownsInWater : 1;
        unsigned char bStayBehindIfLeaderTooFar : 1;
        unsigned char bUsingExitCarTimer : 1;
        unsigned char bScriptPedIsPlayerAlly : 1;
        unsigned char bKnockedUpIntoAir : 1;
        unsigned char bDeadPedInFrontOfCar : 1;
        unsigned char bStayInCarOnJack : 1;

        unsigned char b89 : 1;
        unsigned char bDoomAim : 1;
        unsigned char bCanBeShotInVehicle : 1;
        unsigned char b92 : 1;
        unsigned char b93 : 1;
        unsigned char b94 : 1;
        unsigned char b95 : 1;
        unsigned char b96 : 1;

        unsigned char b97 : 1;
        unsigned char bCrouchWhenScared : 1;
        unsigned char b99 : 1;
        unsigned char b100 : 1;
        unsigned char b101 : 1;
        unsigned char b102 : 1;
        unsigned char bPedWasSetOutOfCollision : 1;
        unsigned char bGangMemberReturnsFire : 1;
    } m_nPedFlags;
private:
    char _pad159[3];
public:
    unsigned char m_nGangFlags;
private:
    char _pad15D[3];
public:
    unsigned char m_nPedStatus; //1 - normal ped, can be removed, 2 - script ped
private:
    char _pad161[3];
public:
    int m_dwObjective;
    int m_dwPrevObjective;
    CEntity *m_pObjectiveEntity;
    void *m_pObjectiveVehicle;
    CVector m_vecObjective;
    float m_fObjectiveAngle;
    void *m_pGangLeader;
    int m_dwPedFormation;
    unsigned int m_dwFearFlags;
    CEntity *m_pThreatEntity;
    CVector2D m_EventOrThreatPos;
    unsigned int m_dwEventType;
    CEntity *m_pEventEntity;
    float m_fAngleToEvent;
    void *m_apFrames[18];
    RpAtomic *m_pCurWeaponAtomic;
    int m_dwAnimGroupId;
    void *m_pVehicleAnim;
    CVector2D m_vecAnimMoveDelta;
    CVector m_vecOffsetSeek;
    CPedIK m_pedIK;
    CVector2D m_ActionPos;
    int m_dwActionTimer;
    int m_dwAction;
    int m_dwLastAction;
    int m_dwMoveState;
    int m_dwStoredMoveState;
    int m_dwPrevActionState;
    int m_dwWaitState;
    int m_dwWaitTimer;
    void *m_apPathNodesStates[8];
    unsigned short m_wPathNodes;
    short m_wCurPathNode;
    CEntity *m_pPathRelEntity;
    CEntity *m_pNextNodeEntity;
    int m_dwPathNodeTimer;
    CPathNode m_aPathNodeStates[8];
    void *m_pCurNodeState;
    char m_nPathState;
private:
    char _pad335[3];
public:
    void *m_pNextPathNode;
    void *m_pLastPathNode;
    CVector m_vecPathNextNode;
    float m_fPathNextNodeDir;
    unsigned int m_dwPathNodeType;
    float m_fHealth;
    float m_fArmour;
    int m_dwShadowUpdateTimer;
    short wRouteLastPoint;
    unsigned short wRoutePoints;
    short wRoutePos;
    unsigned short wRouteType;
    short wRouteCurDir;
private:
    char _pad36A[2];
public:
    float m_fMovedX;
    float m_fMovedY;
    float m_fRotationCur;
    float m_fRotationDest;
    float m_fHeadingRate;
    unsigned short m_nEnterType; //shows how the ped enters the vehicle (from the driver side or passenger side, from the back, etc : 11 - passenger side 15 - driver side)
    unsigned short m_nWalkAroundType;
    CPhysical *m_pCurPhysSurface;
    CVector m_vecOffsetFromPhysSurface;
    CEntity *m_pCurSurface;
    CVector vecSeekVehicle;
    CEntity *m_pSeekTarget;
    void *m_pVehicle;
    Bool m_bInVehicle;
private:
    char _pad3AD[3];
public:
    float m_fSeatPrecisionX;
    float m_fSeatPrecisionY;
    void *m_pFromVehicle;
    void *m_pSeat;
    unsigned int m_dwSeatType; //specify seat type, is -1 when not sitting
    Bool m_bHasPhone;
private:
    char _pad3C5;
public:
    short m_wPhoneId;
    int m_dwLookingForPhone;
    int m_dwPhoneTalkTimer;
    void *m_pLastAccident;
    int m_dwPedType;
    void *m_pPedStats;
    float m_fFleeFromPosX;
    float m_fFleeFromPosY;
    CEntity *m_pFleeFrom;
    int m_dwFleeTimer;
    CEntity *m_pThreatEx;
    CEntity *m_pLastThreatAt;
    int m_dwLastThreatTimer;
    void *m_pVehicleColliding;
    unsigned char m_nStateUnused;
private:
    char _pad3FD[3];
public:
    int m_dwTimerUnused;
    void *m_pTargetUnused;
    CWeapon m_aWeapons[10];
    int m_dwAtchStoredWep;
    int m_dwStoredGiveWep;
    int m_dwStoredGiveAmmo;
    unsigned char m_nActiveWeapon;
    unsigned char m_nWepSkills;
    unsigned char m_nWepAccuracy;
    unsigned char m_nBodyPart;
    CEntity *m_pPointGunAt;
    CVector m_vecHitLastPos;
    int m_dwHitCounter;
    int m_dwLastHitState; //state after a hit, 2 - just received a hit, 24 - recovered after a hit, 8 - just hit someone, this is not always accurate though  
    unsigned char m_nFightFlags1; //FF - looking behind while hitting or getting hit, 00 - normal mode (not sure exactly)
    unsigned char m_nFightFlags2; //flag 01 - you are hitting, flags FE - normal position (?)
    unsigned char m_nFightFlags3; 
    unsigned char m_nBleedCounter;
    void *m_pPedFire;
    void *m_pPedFight;
    float m_fLookDirection;
    int m_dwWepModelID;
    int m_dwLeaveCarTimer;
    int m_dwGetUpTimer;
    int m_dwLookTimer;
    int m_dwStandardTimer;
    int m_dwAttackTimer;
    int m_dwLastHitTime;
    int m_dwHitRecoverTimer;
    int m_dwObjectiveTimer;
    int m_dwDuckTimer;
    int m_dwDuckAndCoverTimer;
    int m_dwBloodyTimer;
    int m_dwShotTime;
    int m_dwShotTimeAdd;
    unsigned char m_nPanicCounter;
    unsigned char m_nDeadBleeding;
    unsigned char m_nBodyPartBleeding;
private:
    char _pad56B;
public:
    CPed *m_apNearPeds[10];
    unsigned short m_nNumNearPeds;
    unsigned short m_nPedMoney;
    unsigned char m_nLastDamWep;
private:
    char _pad599[3];
public:
    CEntity *m_pLastDamEntity;
    CEntity *m_pAttachedTo;
    CVector m_vecAttachOffset;
    unsigned short m_nAttachType;
private:
    char _pad5B2[2];
public:
    float m_fAttachRot;
    unsigned int m_dwAttachWepAmmo;
    unsigned int m_dwThreatFlags;
    int m_dwThreatCheck;
    int m_dwLastThreatCheck;
    unsigned int m_dwSayType;
    int m_dwSayTimer;
    int m_dwTalkTimerLast;
    int m_dwTalkTimer;
    short m_wTalkTypeLast;
    unsigned short m_wTalkType;
    Bool m_bCanPedTalk;
private:
    char _pad5DD[3];
public:
    int m_dwPedLastComment;
    CVector m_vecSeekPosEx;
    float m_fSeekExAngle;
	
	CPed() { }
	virtual ~CPed();
	Bool IsPlayer()
	{
		return ((Bool (__thiscall *)(CPed *))0x4F4930)(this);
	}
};

VALIDATE_SIZE(CPed, 0x5F4);