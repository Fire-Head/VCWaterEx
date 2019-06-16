#pragma once
#include "CPhysical.h"
#include "CPed.h"
#include "CEntity.h"


class CPlayerPed;
class CCopPed;

enum eCarWeapon {

};

enum eCarLock {
    CARLOCK_NOT_USED,
    CARLOCK_UNLOCKED,
    CARLOCK_LOCKED,
    CARLOCK_LOCKOUT_PLAYER_ONLY,
    CARLOCK_LOCKED_PLAYER_INSIDE,
    CARLOCK_COP_CAR,
    CARLOCK_FORCE_SHUT_DOORS,
    CARLOCK_SKIP_SHUT_DOORS
};

enum eVehicleType
{
  VEHICLETYPE_CAR = 0x0,
  VEHICLETYPE_BOAT = 0x1,
  VEHICLETYPE_TRAIN = 0x2,
  VEHICLETYPE_HELI = 0x3,
  VEHICLETYPE_PLANE = 0x4,
  VEHICLETYPE_BIKE = 0x5,
};

enum eVehicleApperance {
    VEHICLE_APPEARANCE_AUTOMOBILE = 1,
    VEHICLE_APPEARANCE_BIKE,
    VEHICLE_APPEARANCE_HELI,
    VEHICLE_APPEARANCE_BOAT,
    VEHICLE_APPEARANCE_PLANE,
};

enum eVehicleLightsFlags {

};

enum eVehicleCreatedBy {
    RANDOM_VEHICLE = 1,
    MISSION_VEHICLE = 2,
    PARKED_VEHICLE = 3,
    PERMANENT_VEHICLE = 4
};

enum eBombState {
    BOMB_DETONATED = 0,
    BOMB_READY = 3,
    BOMB_NOT_SET = 8,
    BOMB_SET = 11
};

#if 0
enum eOrdnanceType;
enum eFlightModel;
enum eDoors;
enum eBikeWheelSpecial;
#else
typedef int eOrdnanceType;
typedef int eFlightModel;
typedef int eBikeWheelSpecial;
#endif

class CWeapon;
class CPed;
typedef int tWheelState;

class CAutoPilot
{
	char unk[0x74];
};

class CVehicle : public CPhysical
{
protected:
	DUMMYB(CVehicle, CPhysical);
public:
    void *m_pHandlingData;
    void *m_pFlyingHandling;
    CAutoPilot m_autoPilot;
    CVehicle *m_pVehicleToRam;
    unsigned char m_nPrimaryColor;
    unsigned char m_nSecondaryColor;
    char m_anExtras[2];
    short m_wWantedStarsOnEnter;
    short m_wMissionValue;
    void *m_pDriver;
    void *m_passengers[8];
    unsigned char m_nNumPassengers;
    unsigned char m_nNumGettingIn;
    unsigned char m_nGettingInFlags;
    unsigned char m_nGettingOutFlags;
    unsigned char m_nMaxPassengers;
    char __f01CD[3];
    int field_1D4;
    CVector field_1D8;
    CEntity *m_pEntityWeAreOn;
    class CFire *m_pFire;
    float m_fSteerAngle;
    float m_fGasPedal;
    float m_fBreakPedal;
    unsigned char m_nCreatedBy; // see eVehicleCreatedBy
    struct {
        unsigned char b01 : 1;
        unsigned char b02 : 1;
        unsigned char b03 : 1;
        unsigned char bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
        unsigned char bIsEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
        unsigned char bIsHandbrakeOn : 1;
        unsigned char bLightsOn : 1; // Are the lights switched on ?
        unsigned char b08 : 1;
        unsigned char bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
        unsigned char bHideOccupants : 1;
        unsigned char bIsBus : 1; // Is this vehicle a bus
        unsigned char bIsBig : 1; // Is this vehicle a big
        unsigned char bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
        unsigned char b14 : 1;
        unsigned char b15 : 1;
        unsigned char b16 : 1;
        unsigned char b17 : 1;
        unsigned char bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
        unsigned char b19 : 1;
        unsigned char b20 : 1;
        unsigned char b21 : 1;
        unsigned char bSpawnCops : 1;
        unsigned char b23 : 1;
        unsigned char bUsePlayerColModel : 1;
        unsigned char bIsLawEnforcementVeh : 1;
        unsigned char b26 : 1;
        unsigned char bHasDriver : 1;
        unsigned char b28 : 1;
        unsigned char b29 : 1;
        unsigned char b30 : 1;
        unsigned char b31 : 1;
        unsigned char b32 : 1;
        unsigned char b33 : 1;
        unsigned char bCarTiresInvulnerable : 1;
        unsigned char b35 : 1;
        unsigned char b36 : 1;
        unsigned char b37 : 1;
        unsigned char b38 : 1;
        unsigned char b39 : 1;
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
        unsigned char b55 : 1;
        unsigned char b56 : 1;
        unsigned char b57 : 1;
        unsigned char b58 : 1;
        unsigned char b59 : 1;
        unsigned char b60 : 1;
        unsigned char b61 : 1;
        unsigned char b62 : 1;
        unsigned char b63 : 1;
        unsigned char b64 : 1;
    } m_nVehicleFlags;
    unsigned char m_nAmmoInClip;
    char field_201;
    float m_fHealth;
    unsigned char m_nCurrentGear;
    char __f0205[3];
    int field_20C;
    int field_210;
    int m_nTimeTillWeNeedThisCar;
    int field_218;
    int m_nTimeOfDeath;
    short field_220;
    short m_wBombTimer; // goes down with each frame
    int field_224;
    int field_228;
    int field_22C;
    unsigned int m_nLockStatus;
    unsigned char m_nLastWeaponDamage;
    char __f0231[3];
    CEntity *pLastDamEntity;
    unsigned char m_nRadioStation;
    char field_23D;
    char field_23E;
    unsigned int m_bHornEnabled;
    char field_244;
    unsigned char m_bSirenOrAlarm;
    unsigned char m_nSirenExtra;
    char field_247;

class CStoredCollPoly {
public:
    CVector m_aVertices[3]; // triangle vertices
    Bool m_bIsActual;
private:
    char _pad[3];
};

    CStoredCollPoly m_frontCollPoly; // poly which is under front part of car
    CStoredCollPoly m_rearCollPoly; // poly which is under rear part of car
    float m_fSteerRatio;
    unsigned int m_nVehicleClass; // see enum eVehicleType
};


VALIDATE_SIZE(CVehicle, 0x2A0);