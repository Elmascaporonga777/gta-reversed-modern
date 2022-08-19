/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Rect.h"
#include "Vector.h"

class CObject;
class CPed;
class CEntryExit;

class CEntryExit {
public:
    char    m_szName[8];
    CRect   m_recEntrance;
    float   m_fEntranceZ;
    float   m_fEntranceAngle;
    CVector m_vecExitPos;
    float   m_fExitAngle;
    struct {
        uint16 bUnknownInterior : 1;     // 0x1
        uint16 bUnknownPairing : 1;      // 0x2
        uint16 bCreateLinkedPair : 1;    // 0x4
        uint16 bRewardInterior : 1;      // 0x8
        uint16 bUsedRewardEntrance : 1;  // 0x10
        uint16 bCarsAndAircraft : 1;     // 0x20
        uint16 bBikesAndMotorcycles : 1; // 0x40
        uint16 bDisableOnFoot : 1;       // 0x80

        uint16 bAcceptNpcGroup : 1;      // 0x100
        uint16 bFoodDateFlag : 1;        // 0x200
        uint16 bUnknownBurglary : 1;     // 0x400
        uint16 bDisableExit : 1;         // 0x800
        uint16 bBurglaryAccess : 1;      // 0x1000
        uint16 bEnteredWithoutExit : 1;  // 0x2000
        uint16 bEnableAccess : 1;        // 0x4000
        uint16 bDeleteEnex : 1;          // 0x8000
    } m_nFlags;
    uint8       m_nArea;
    uint8       m_nSkyColor;
    uint8       m_nTimeOn;
    uint8       m_nTimeOff;
    uint8       m_nNumberOfPeds;
    CEntryExit* m_pLink;

    static bool& ms_bWarping;
    static CObject*& ms_pDoor;
    static CEntryExit*& ms_spawnPoint;

public:
    static void InjectHooks();

    void GenerateAmbientPeds(const CVector& posn);
    CEntryExit* GetEntryExitToDisplayNameOf();
    void GetPositionRelativeToOutsideWorld(CVector& outPos);
    bool IsInArea(const CVector& position);
    void RequestAmbientPeds();
    void RequestObjectsInFrustum() const;
    bool TransitionFinished(CPed* ped);
    bool TransitionStarted(CPed* ped);
    static void WarpGangWithPlayer(CPed* ped);
    void ProcessStealableObjects(CPed* ped);
    void FindValidTeleportPoint(CVector* point);
    bool HasNameSet() const;

    // NOTSA

    // These 5 were probably inlined
    CVector2D GetPosition2D() const;
    CVector GetPosition() const;
    auto GetLinkedOrThis() { return m_pLink ? m_pLink : this; }
    auto GetLinkedOrThis() const { return m_pLink ? m_pLink : this; }
    bool IsVisibleByTime() const;

    // Most likely didn't exist in this form.
    CMatrix GetRectEntranceMatrix() const;

    // This possibly existed in some form.
    // `IsInArea` has some transformation code Which is very similar to the one in `CEnExManager::AddOne`
    CVector TransformEntrancePoint(const CVector& point) const;
};

VALIDATE_SIZE(CEntryExit, 0x3C);
