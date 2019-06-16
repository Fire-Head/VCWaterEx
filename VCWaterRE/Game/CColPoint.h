#pragma once
#include "..\address.h"
#include "CVector.h"

class CColPoint
{
public:
    CVector  m_vecPosition;
    float field_C;
    CVector m_vecNormal;
    float field_1C;
    unsigned char m_nSurfaceTypeA;
    unsigned char m_nPieceTypeA;
    unsigned char m_nSurfaceTypeB;
    unsigned char m_nPieceTypeB;
    int field_24;
};