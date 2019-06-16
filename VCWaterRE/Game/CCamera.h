#pragma once
#include "..\address.h"
#include "types.h"
#include "CVector.h"
#include "RenderWare.h"
#include "CEntity.h"
#include "CPlaceable.h"
#include "CCam.h"
#include "CDraw.h"


#define MAX_NUM_OF_SPLINETYPES (4)

struct tCamPathSplines {
	float *m_arr_PathData;//    FLOAT m_arr_PathData[MAXPATHLENGTH = 800];    
};

struct tQueuedMode {
	short Mode;
private:
    char _pad2[2];
public:
	float Duration;
	short MinZoom;
	short MaxZoom;
};

//#pragma pack(push, 1)
class CCamera : public CPlaceable
{
public:
	Bool    m_bAboveGroundTrainNodesLoaded;
	Bool    m_bBelowGroundTrainNodesLoaded;
	Bool    m_bCamDirectlyBehind;
	Bool    m_bCamDirectlyInFront;
	Bool    m_bCameraJustRestored;
	Bool    m_bcutsceneFinished;
	Bool    m_bCullZoneChecksOn;
	Bool    m_bFirstPersonBeingUsed; // To indicate if the m_bFirstPersonBeingUsed viewer is being used.
	Bool    m_bJustJumpedOutOf1stPersonBecauseOfTarget;
	Bool    m_bIdleOn;
	Bool    m_bInATunnelAndABigVehicle;
	Bool    m_bInitialNodeFound;
	Bool    m_bInitialNoNodeStaticsSet;
	Bool    m_bIgnoreFadingStuffForMusic;
	Bool    m_bPlayerIsInGarage;
	Bool    m_bPlayerWasOnBike;
	Bool    m_bJustCameOutOfGarage;
	Bool    m_bJustInitalised;	//Just so the speed thingy doesn't go mad right at the start
	Bool	m_bJust_Switched;	//Variable to indicate that we have jumped somewhere
	Bool    m_bLookingAtPlayer;
	Bool    m_bLookingAtVector;
	Bool    m_bMoveCamToAvoidGeom;
	Bool    m_bObbeCinematicPedCamOn;
	Bool    m_bObbeCinematicCarCamOn;
	Bool    m_bRestoreByJumpCut;
	Bool    m_bUseNearClipScript;
	Bool    m_bStartInterScript;
	Bool	m_bStartingSpline;
	Bool    m_bTargetJustBeenOnTrain;	//this variable is needed to be able to restore the camera
	Bool    m_bTargetJustCameOffTrain;
	Bool    m_bUseSpecialFovTrain;
	Bool    m_bUseTransitionBeta;
	Bool    m_bUseScriptZoomValuePed;
	Bool    m_bUseScriptZoomValueCar;
	Bool    m_bWaitForInterpolToFinish;
	Bool    m_bItsOkToLookJustAtThePlayer;	 //Used when interpolating
	Bool    m_bWantsToSwitchWidescreenOff;
	Bool    m_WideScreenOn;
	Bool    m_1rstPersonRunCloseToAWall;
	Bool    m_bHeadBob;
	Bool    m_bVehicleSuspenHigh;
	Bool    m_bEnable1rstPersonCamCntrlsScript;

	Bool    m_bAllow1rstPersonWeaponsCamera;
	Bool    m_bFailedCullZoneTestPreviously;
	Bool    m_FadeTargetIsSplashScreen;	//used as hack for fading 
	Bool    WorldViewerBeingUsed;	// To indicate if the world viewer is being used.                                      
	unsigned char   ActiveCam;				 // Which one at the moment (0 or 1)
											// Their is a fudge at the end when the renderware matrix will receive either
											// the active camera or the worldviewer camera
	unsigned int    m_uiCamShakeStart;          // When did the camera shake start.
	unsigned int    m_uiFirstPersonCamLastInputTime;
	unsigned int    m_uiLongestTimeInMill;
	unsigned int    m_uiNumberOfTrainCamNodes;

	unsigned char     m_uiTransitionJUSTStarted;  // This is the first frame of a transition.
	unsigned char     m_uiTransitionState;        // 0:one mode 1:transition
	unsigned int    m_uiTimeLastChange;
	unsigned int    m_uiTimeWeLeftIdle_StillNoInput;
	unsigned int    m_uiTimeWeEnteredIdle;
	unsigned int    m_uiTimeTransitionStart;    // When was the transition started ?
	unsigned int    m_uiTransitionDuration;     // How long does the transition take ?
	unsigned int    m_uiTransitionDurationTargetCoors;
	int     m_BlurBlue;
	int     m_BlurGreen;
	int     m_BlurRed;
	int     m_BlurType;
	int     m_iWorkOutSpeedThisNumFrames; 
	int     m_iNumFramesSoFar;				//counter
	int     m_iCurrentTrainCamNode;			//variable indicating which camera node we are at for the train
	int     m_motionBlur;					//to indicate that we are fading

	int     m_imotionBlurAddAlpha;
	int     m_iCheckCullZoneThisNumFrames;
	int     m_iZoneCullFrameNumWereAt;
	int     WhoIsInControlOfTheCamera;		//to discern between obbe and scripts
	float   CamFrontXNorm, CamFrontYNorm;
	float   CarZoomIndicator;
	float   CarZoomValue;
	float   CarZoomValueSmooth;
	float   DistanceToWater;
	float   FOVDuringInter;
	float   LODDistMultiplier;				 // This takes into account the FOV and the standard LOD multiplier Smaller aperture->bigger LOD multipliers.
	float   GenerationDistMultiplier;		// This takes into account the FOV but noy the standard LOD multiplier

	float   m_fAlphaSpeedAtStartInter;
	float   m_fAlphaWhenInterPol;
	float   m_fAlphaDuringInterPol;
	float   m_fBetaDuringInterPol;
	float   m_fBetaSpeedAtStartInter;
	float   m_fBetaWhenInterPol;
	float   m_fFOVWhenInterPol;
	float   m_fFOVSpeedAtStartInter;
	float   m_fStartingBetaForInterPol;
	float   m_fStartingAlphaForInterPol;
	float   m_PedOrientForBehindOrInFront;

	float   m_CameraAverageSpeed;		//this is an average depending on how many frames we work it out
	float   m_CameraSpeedSoFar;		 //this is a running total
	float   m_fCamShakeForce;			 // How severe is the camera shake.
	float m_fCarZoomValueScript;
	float   m_fFovForTrain;
	float   m_fFOV_Wide_Screen;
	float   m_fNearClipScript;
	float   m_fOldBetaDiff;						 // Needed for interpolation between 2 modes
	float   m_fPedZoomValue;
	float   m_fPedZoomValueSmooth;
	float   m_fPedZoomValueScript;
	float   m_fPositionAlongSpline;				//Variable used to indicate how far along the spline we are 0-1 for started to completed respectively
	float   m_ScreenReductionPercentage;
	float   m_ScreenReductionSpeed;
	float   m_AlphaForPlayerAnim1rstPerson;
	float   Orientation;            // The orientation of the camera. Used for peds walking.
	float   PedZoomIndicator;
	float   PlayerExhaustion;       // How tired is player (inaccurate sniping) 0.0f-1.0f
									// The following things are used by the sound code to
									// play reverb depending on the surroundings. From a point
									// in front of the camera the disance is measured to the
									// nearest obstacle (building)
	float   SoundDistUp;			//, SoundDistLeft, SoundDistRight;     // These ones are buffered and should be used by the audio
	float   SoundDistUpAsRead;		//, SoundDistLeftAsRead, SoundDistRightAsRead;
	float   SoundDistUpAsReadOld;	 //, SoundDistLeftAsReadOld, SoundDistRightAsReadOld;
									 // Very rough distance to the nearest water for the sound to use
								  // Front vector X&Y normalised to 1. Used by loads of stuff.


	float   m_fAvoidTheGeometryProbsTimer;
	short   m_nAvoidTheGeometryProbsDirn;

	float   m_fWideScreenReductionAmount;	//0 for not reduced 1 for fully reduced (Variable for Les)
	float   m_fStartingFOVForInterPol;

	CCam Cams[3];                // The actual cameras (usually only one of the two is active)
								 // And to complicate this we have a third camera, this camera is 
								 // used for debugging when we want to have a look at the world.
								 // We can't change the camera mode because other objects depend on their

	void *pToGarageWeAreIn;
	void *pToGarageWeAreInForHackAvoidFirstPerson;
	tQueuedMode m_PlayerMode;

	// The higher priority player camera mode. This one is used
	// for the sniper mode and rocket launcher mode.
	// This one overwrites the m_PlayerMode above.
	tQueuedMode PlayerWeaponMode;
	CVector m_PreviousCameraPosition;		//needed to work out speed
	CVector m_RealPreviousCameraPosition;	// This cane be used by stuff outside the camera code. The one above is the same as the current coordinates outwidth the camera code.
											 // an active camera for range finding etc
	CVector m_cvecAimingTargetCoors;        // Coors to look at with Gordons aiming thing
											// The player camera that is waiting to be used
											// This camera can replace the default camera where this is
											// needed (in tricky situations like tunnels for instance)
	CVector m_vecFixedModeVector;
	CVector m_vecFixedModeSource;
	CVector m_vecFixedModeUpOffSet;
	CVector m_vecCutSceneOffset;

	CVector m_cvecStartingSourceForInterPol;
	CVector m_cvecStartingTargetForInterPol;
	CVector m_cvecStartingUpForInterPol;
	CVector m_cvecSourceSpeedAtStartInter;
	CVector m_cvecTargetSpeedAtStartInter;
	CVector m_cvecUpSpeedAtStartInter;
	CVector m_vecSourceWhenInterPol;
	CVector m_vecTargetWhenInterPol;
	CVector m_vecUpWhenInterPol;
	CVector m_vecClearGeometryVec;
	CVector m_vecGameCamPos;

	CVector SourceDuringInter, TargetDuringInter, UpDuringInter;
	// RenderWare camera pointer
	RwCamera* m_pRwCamera;
						   
	CEntity *pTargetEntity;
	tCamPathSplines m_arrPathArray[MAX_NUM_OF_SPLINETYPES]; //These only get created when the script calls the load splines function
	CMatrix m_cameraMatrix;
	Bool    m_bGarageFixedCamPositionSet;
	Bool    m_vecDoingSpecialInterPolation;
	Bool    m_bScriptParametersSetForInterPol;
	Bool    m_bFading;	//to indicate that we are fading 
	Bool    m_bMusicFading;
	CMatrix m_viewMatrix;
	CVector m_vecFrustumNormals[4];
	CVector m_vecOldSourceForInter;
	CVector m_vecOldFrontForInter;
	CVector m_vecOldUpForInter;
	float   m_vecOldFOVForInter;
	float   m_fFLOATingFade;	//variable representing the FLOAT version of CDraw::Fade. Necessary to stop loss of precision
	float   m_fFLOATingFadeMusic;
	float   m_fTimeToFadeOut;
	float   m_fTimeToFadeMusic;
	float   m_fFractionInterToStopMoving;
	float   m_fFractionInterToStopCatchUp;
	float   m_fFractionInterToStopMovingTarget;
	float   m_fFractionInterToStopCatchUpTarget;

	float   m_fGaitSwayBuffer;
	float   m_fScriptPercentageInterToStopMoving;
	float   m_fScriptPercentageInterToCatchUp;
	unsigned int  m_fScriptTimeForInterPolation;

	short   m_iFadingDirection;
	int     m_iModeObbeCamIsInForCar;
	short   m_iModeToGoTo;
	short   m_iMusicFadingDirection;
	short   m_iTypeOfSwitch;

	unsigned int  m_uiFadeTimeStarted;
	unsigned int  m_uiFadeTimeStartedMusic;
	
	int GetLookDirection()
	{
		return ((int (__thiscall*)(CCamera*))0x46F6CE)(this);
	}

	CMatrix& GetInverseMatrix()
	{
		return m_cameraMatrix;
	}
	
	bool IsSphereVisible(const CVector &center, float radius, const CMatrix *mat) // thanks to aap
	{
		RwV3d c;
		c = *(RwV3d*)&center;
		RwV3dTransformPoints(&c, &c, 1, &mat->rwMatrix);
		if(c.y + radius < CDraw::ms_fNearClipZ) return false;
		if(c.y - radius > CDraw::ms_fFarClipZ) return false;
		if(c.x*m_vecFrustumNormals[0].x + c.y*m_vecFrustumNormals[0].y > radius) return false;
		if(c.x*m_vecFrustumNormals[1].x + c.y*m_vecFrustumNormals[1].y > radius) return false;
		if(c.y*m_vecFrustumNormals[2].y + c.z*m_vecFrustumNormals[2].z > radius) return false;
		if(c.y*m_vecFrustumNormals[3].y + c.z*m_vecFrustumNormals[3].z > radius) return false;
		return true;
	}

};
//#pragma pack(pop)

extern CCamera& TheCamera;


VALIDATE_SIZE(CCamera, 0x958);