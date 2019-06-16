#pragma once
#include "types.h"
#include "..\address.h"
#include "CRGBA.h"
#include "RenderWare.h"

#pragma pack(push, 1)
struct tParticleSystemData
{
	Int32 m_Type;
	Char          m_aName[20];
	Float         m_fCreateRange;
	Float         m_fDefaultInitialRadius;
	Float         m_fExpansionRate;
	UInt16        m_nZRotationInitialAngle;
	Int16         m_nZRotationAngleChangeAmount;
	UInt16        m_nZRotationChangeTime;
	UInt16        m_nZRadiusChangeTime;
	Float         m_fInitialZRadius;
	Float         m_fZRadiusChangeAmount;
	UInt16        m_nFadeToBlackTime;
	UInt8         m_nFadeToBlackInitialIntensity;
	char _pad0[1];
	Int16         m_nFadeToBlackAmount;
	UInt8         m_nFadeAlphaInitialIntensity;
	char _pad1[1];
	UInt16        m_nFadeAlphaTime;
	Int16         m_nFadeAlphaAmount;
	UInt8         m_nStartAnimationFrame;
	UInt8         m_nFinalAnimationFrame;
	UInt16        m_nAnimationSpeed;
	UInt16        m_nRotationSpeed;
	char _pad2[2];
	Float         m_fGravitationalAcceleration;
	Int32         m_nFrictionDecceleration;
	Int32         m_nLifeSpan;
	Float         m_fPositionRandomError;
	Float         m_fVelocityRandomError;
	Float         m_fExpansionRateError;
	Int32         m_nRotationRateError;
	UInt32        m_nLifeSpanErrorShape;
	Float         m_fTrailLengthMultiplier;
	UInt32        Flags;
	CRGBA         m_RenderColouring;
	UInt8         m_InitialColorVariation;
	CRGBA         m_FadeDestinationColor;
	char _pad3[3];
	UInt32        m_ColorFadeTime;
	
	//
	CVector2D     m_vecTextureStretch;
	Float         m_fWindFactor;
	//
	
	RwRaster      **m_ppRaster;
	class CParticle     *m_pParticles;
};
#pragma pack(pop)

VALIDATE_SIZE(tParticleSystemData, 0x94);

class cParticleSystemMgr
{
public:
	tParticleSystemData m_aParticles[83];
};

extern cParticleSystemMgr &mod_ParticleSystemManager;
