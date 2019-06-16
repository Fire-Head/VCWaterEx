#pragma once
#include <windows.h>
#include "..\address.h"
#include <stdio.h>
#include <stdarg.h>

#include "./RW34/include/rwplcore.h"
#include "./RW34/include/rwcore.h"
#include "./RW34/include/rpworld.h"
#include "./RW34/include/rpmatfx.h"
#include "./RW34/include/rpskin.h"
#include "./RW34/include/rphanim.h"
#include "./RW34/include/rtbmp.h"

#include "./RW34/include/rtpng.h"
/*
#include "./RW33/include/rwplcore.h"
#include "./RW33/include/rwcore.h"
#include "./RW33/include/rtquat.h"
#include "./RW33/include/rpworld.h"
#include "./RW33/include/rperror.h"
#include "./RW33/include/rpcriter.h"
#include "./RW33/include/rphanim.h"
#include "./RW33/include/rpmatfx.h"
#include "./RW33/include/rpskin.h"
*/

struct GlobalScene
{
    RpWorld         *world;
    RwCamera        *camera;
};

#define Scene (*(GlobalScene *)ADDR_SCENE)

typedef RwV3d *(*rwVectorsMultFn) (RwV3d * pointsOut,
                                   const RwV3d * pointsIn,
                                   RwInt32 numPoints,
                                   const RwMatrix * matrix);
								   
//
typedef struct PsGlobalType PsGlobalType;
typedef struct RsInputDevice RsInputDevice;
typedef enum RsEventStatus RsEventStatus;
typedef struct RsMouseStatus RsMouseStatus;
typedef struct RsPadButtonStatus RsPadButtonStatus;
typedef struct RsKeyStatus RsKeyStatus;
typedef enum RsKeyCodes RsKeyCodes;
typedef enum RsEvent RsEvent;
typedef enum RsPadButtons RsPadButtons;
typedef struct RsGlobalType RsGlobalType;
typedef enum RsInputDeviceType RsInputDeviceType;

struct PsGlobalType
{
  HWND window;
  HINSTANCE instance;
  RwBool fullscreen;
  RwV2d lastMousePos;

  DWORD field_14;
  DWORD diInterface;
  DWORD diMouse;
  DWORD diDevice1;
  DWORD diDevice2;
};

enum RsEventStatus
{
    rsEVENTERROR,
    rsEVENTPROCESSED,
    rsEVENTNOTPROCESSED
};

typedef RsEventStatus (*RsInputEventHandler)(RsEvent event, void *param);

struct RsInputDevice
{
  RsInputDeviceType inputDeviceType;
  RwBool used;
  RsInputEventHandler inputEventHandler;
};

struct RsGlobalType
{
  //LPCSTR appName;
  const RwChar *appName;
  RwInt32 maximumWidth;
  RwInt32 maximumHeight;
  RwInt32 w;
  RwInt32 h;
  RwInt32 maxFPS;
  RwBool quit;
  void *ps;		/* platform specific data */  //PsGlobalType
  RsInputDevice keyboard;
  RsInputDevice mouse;
  RsInputDevice pad;
};

enum RsInputDeviceType
{
    rsKEYBOARD,
    rsMOUSE,
    rsPAD
};

struct RsMouseStatus
{
    RwV2d   pos;
    RwV2d   delta;
    RwBool  shift;
    RwBool  control;
#if defined(macintosh)
    RwBool  alt;
#endif /* defined(macintosh) */
};

struct RsPadButtonStatus
{
    RwInt32     padID;
    RwUInt32    padButtons;
};

struct RsKeyStatus
{
    RwInt32     keyCharCode;
};


enum RsKeyCodes
{
  rsESC = 0x3E8,
  rsF1 = 0x3E9,
  rsF2 = 0x3EA,
  rsF3 = 0x3EB,
  rsF4 = 0x3EC,
  rsF5 = 0x3ED,
  rsF6 = 0x3EE,
  rsF7 = 0x3EF,
  rsF8 = 0x3F0,
  rsF9 = 0x3F1,
  rsF10 = 0x3F2,
  rsF11 = 0x3F3,
  rsF12 = 0x3F4,
  rsINS = 0x3F5,
  rsDEL = 0x3F6,
  rsHOME = 0x3F7,
  rsEND = 0x3F8,
  rsPGUP = 0x3F9,
  rsPGDN = 0x3FA,
  rsUP = 0x3FB,
  rsDOWN = 0x3FC,
  rsLEFT = 0x3FD,
  rsRIGHT = 0x3FE,
  rsDIVIDE = 0x3FF,
  rsTIMES = 0x400,
  rsPLUS = 0x401,
  rsMINUS = 0x402,
  rsPADDEL = 0x403,
  rsPADEND = 0x404,
  rsPADDOWN = 0x405,
  rsPADPGDN = 0x406,
  rsPADLEFT = 0x407,
  rsPAD5 = 0x408,
  rsNUMLOCK = 0x409,
  rsPADRIGHT = 0x40A,
  rsPADHOME = 0x40B,
  rsPADUP = 0x40C,
  rsPADPGUP = 0x40D,
  rsPADINS = 0x40E,
  rsPADENTER = 0x40F,
  rsSCROLL = 0x410,
  rsPAUSE = 0x411,
  rsBACKSP = 0x412,
  rsTAB = 0x413,
  rsCAPSLK = 0x414,
  rsENTER = 0x415,
  rsLSHIFT = 0x416,
  rsRSHIFT = 0x417,
  rsSHIFT = 0x418,
  rsLCTRL = 0x419,
  rsRCTRL = 0x41A,
  rsLALT = 0x41B,
  rsRALT = 0x41C,
  rsLWIN = 0x41D,
  rsRWIN = 0x41E,
  rsAPPS = 0x41F,
  rsNULL = 0x420,
};



enum RsEvent
{
  rsCAMERASIZE = 0x0,
  rsCOMMANDLINE = 0x1,
  rsFILELOAD = 0x2,
  rsINITDEBUG = 0x3,
  rsINPUTDEVICEATTACH = 0x4,
  rsLEFTBUTTONDOWN = 0x5,
  rsLEFTBUTTONUP = 0x6,
  rsMOUSEMOVE = 0x7,
  rsMOUSEWHEELMOVE = 0x8,
  rsPLUGINATTACH = 0x9,
  rsREGISTERIMAGELOADER = 0xA,
  rsRIGHTBUTTONDOWN = 0xB,
  rsRIGHTBUTTONUP = 0xC,
  _rs_13 = 0xD,
  _rs_14 = 0xE,
  _rs_15 = 0xF,
  _rs_16 = 0x10,
  _rs_17 = 0x11,
  _rs_18 = 0x12,
  _rs_19 = 0x13,
  _rs_20 = 0x14,
  rsRWINITIALISE = 0x15,
  rsRWTERMINATE = 0x16,
  rsSELECTDEVICE = 0x17,
  rsINITIALISE = 0x18,
  rsTERMINATE = 0x19,
  rsIDLE = 0x1A,
  rsFRONTENDIDLE = 0x1B,
  rsKEYDOWN = 0x1C,
  rsKEYUP = 0x1D,
  rsQUITAPP = 0x1E,
  rsPADBUTTONDOWN = 0x1F,
  rsPADBUTTONUP = 0x20,
  rsPADANALOGUELEFT = 0x21,
  rsPADANALOGUELEFTRESET = 0x22,
  rsPADANALOGUERIGHT = 0x23,
  rsPADANALOGUERIGHTRESET = 0x24,
  rsPREINITCOMMANDLINE = 0x25,
  rsACTIVATE = 0x26,
};

enum RsPadButtons
{
    rsPADDPADLEFT   = 0x00000001,
    rsPADDPADRIGHT  = 0x00000002,
    rsPADDPADUP     = 0x00000004,
    rsPADDPADDOWN   = 0x00000008,
    rsPADSTART      = 0x00000010,
    rsPADSELECT     = 0x00000020,
    rsPADBUTTON1    = 0x00000040,
    rsPADBUTTON2    = 0x00000080,
    rsPADBUTTON3    = 0x00000100,
    rsPADBUTTON4    = 0x00000200,
    rsPADBUTTON5    = 0x00000400,
    rsPADBUTTON6    = 0x00000800,
    rsPADBUTTON7    = 0x00001000,
    rsPADBUTTON8    = 0x00002000,
    rsPADBUTTONA1   = 0x00004000,
    rsPADBUTTONA2   = 0x00008000
};
