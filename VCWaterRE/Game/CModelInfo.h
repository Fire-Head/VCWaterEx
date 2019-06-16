#pragma once
#include "..\address.h"
#include "CVector.h"


class CSphere {
public:
    CVector m_vecCenter;
    float   m_fRadius;
};

class CBox {
public:
    CVector m_vecMin;
    CVector m_vecMax;
};

class CColBox : public CBox
{
};
class CColSphere : public CSphere
{
};

class CColModel {
public:
    CColSphere m_colSphere;
    CBox m_boundBox;
    unsigned short m_nNumSpheres;
    unsigned short m_nNumBoxes;
    unsigned short m_nNumTriangles;
    unsigned char m_nNumLines;
    unsigned char m_nColStoreIndex;
    bool m_bLoaded;
private:
    char _pad31[3];
public:
    CColSphere *m_pSpheres;
    void *m_pLines;
    CColBox *m_pBoxes;
    void *m_pVertices;
    void *m_pTriangles;
    void *m_pTrianglePlanes;
};
VALIDATE_SIZE(CColModel, 0x4C);

class CBaseModelInfo
{
	int vtable;
public:
    char           m_szName[21];
    unsigned char  m_nType; // see ModelInfoType
    unsigned char  m_nNum2dEffects;
    bool           m_bDoWeOwnTheColModel;
    CColModel     *m_pColModel;
    short          m_n2dEffectIndex;
    short          m_nObjectDataIndex;
    unsigned short m_nRefCount;
    short          m_nTxdIndex;

    // vtable
};

VALIDATE_SIZE(CBaseModelInfo, 0x28);

class CModelInfo {
public:
    // variables
    static CBaseModelInfo **ms_modelInfoPtrs;       // [6500];
};