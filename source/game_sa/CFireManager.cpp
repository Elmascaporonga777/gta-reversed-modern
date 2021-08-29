#include "StdInc.h"

CFireManager& gFireManager = *reinterpret_cast<CFireManager*>(0xB71F80);

void CFireManager::InjectHooks() {
    ReversibleHooks::Install("CFireManager", "CFireManager", 0x539DA0, &CFireManager::Constructor);
    ReversibleHooks::Install("CFireManager", "~CFireManager", 0x538BB0, &CFireManager::Destructor);
    ReversibleHooks::Install("CFireManager", "Init", 0x538BC0, &CFireManager::Init);
    ReversibleHooks::Install("CFireManager", "Shutdown", 0x539DD0, &CFireManager::Shutdown);
    ReversibleHooks::Install("CFireManager", "GetNumOfNonScriptFires", 0x538F10, &CFireManager::GetNumOfNonScriptFires);
    ReversibleHooks::Install("CFireManager", "FindNearestFire", 0x538F40, &CFireManager::FindNearestFire);
    ReversibleHooks::Install("CFireManager", "PlentyFiresAvailable", 0x539340, &CFireManager::PlentyFiresAvailable);
    ReversibleHooks::Install("CFireManager", "ExtinguishPoint", 0x539450, &CFireManager::ExtinguishPoint);
    ReversibleHooks::Install("CFireManager", "ExtinguishPointWithWater", 0x5394C0, &CFireManager::ExtinguishPointWithWater);
    ReversibleHooks::Install("CFireManager", "IsScriptFireExtinguished", 0x5396E0, &CFireManager::IsScriptFireExtinguished);
    ReversibleHooks::Install("CFireManager", "RemoveScriptFire", 0x539700, &CFireManager::RemoveScriptFire);
    ReversibleHooks::Install("CFireManager", "RemoveAllScriptFires", 0x539720, &CFireManager::RemoveAllScriptFires);
    ReversibleHooks::Install("CFireManager", "ClearAllScriptFireFlags", 0x5397A0, &CFireManager::ClearAllScriptFireFlags);
    ReversibleHooks::Install("CFireManager", "SetScriptFireAudio", 0x5397B0, &CFireManager::SetScriptFireAudio);
    ReversibleHooks::Install("CFireManager", "GetScriptFireCoords", 0x5397E0, &CFireManager::GetScriptFireCoords);
    ReversibleHooks::Install("CFireManager", "GetNumFiresInRange", 0x5397F0, &CFireManager::GetNumFiresInRange);
    ReversibleHooks::Install("CFireManager", "GetNumFiresInArea", 0x539860, &CFireManager::GetNumFiresInArea);
    ReversibleHooks::Install("CFireManager", "DestroyAllFxSystems", 0x539D10, &CFireManager::DestroyAllFxSystems);
    ReversibleHooks::Install("CFireManager", "CreateAllFxSystems", 0x539D50, &CFireManager::CreateAllFxSystems);
    ReversibleHooks::Install("CFireManager", "GetNextFreeFire", 0x539E50, &CFireManager::GetNextFreeFire);
    ReversibleHooks::Install("CFireManager", "StartFire_NoTarget", 0x539F00, static_cast<CFire*(CFireManager::*)(CVector, float, uint8, CEntity*, uint32, int8, uint8)>(&CFireManager::StartFire));
    ReversibleHooks::Install("CFireManager", "StartFire", 0x53A050, static_cast<CFire *(CFireManager::*)(CEntity*, CEntity*, float, uint8, uint32, int8)>(&CFireManager::StartFire));
    ReversibleHooks::Install("CFireManager", "StartScriptFire", 0x53A270, &CFireManager::StartScriptFire);
    ReversibleHooks::Install("CFireManager", "Update", 0x53AF00, &CFireManager::Update);
}

// 0x539DA0
CFireManager::CFireManager() {
    Init();
}

// 0x538BB0
CFireManager::~CFireManager() {
    // NOP
}

CFireManager* CFireManager::Destructor() {
    this->CFireManager::~CFireManager();
    return this;
}

CFireManager* CFireManager::Constructor() {
    this->CFireManager::CFireManager();
    return this;
}

// 0x538BC0
void CFireManager::Init() {
    for (CFire& fire : m_aFires) {
        fire.Initialise();
    }
    m_nMaxFireGenerationsAllowed = 1'000'000 - 1;
}

// 0x539DD0
void CFireManager::Shutdown() {
    for (CFire& fire : m_aFires) {
        fire.m_nFlags.bCreatedByScript = false;
        if (fire.IsActive()) {
            fire.Extinguish();
        }
    }
}

// 0x538F10
uint32 CFireManager::GetNumOfNonScriptFires() {
    uint32 c = 0;
    for (CFire& fire : m_aFires)
        if (fire.m_nFlags.bActive && !fire.m_nFlags.bCreatedByScript)
            c++;
    return c;
}

// NOTSA
uint32 CFireManager::GetNumOfFires() {
    uint32 c = 0;
    for (CFire& fire : m_aFires) {
        if (fire.IsActive())
            c++;
    }
    return c;
}

// 0x538F40
CFire* CFireManager::FindNearestFire(CVector const& point, bool bCheckIsBeingExtinguished, bool bCheckWasCreatedByScript) {
    float fNearestDist2DSq = std::numeric_limits<float>::max(); // Izzotop :thinking
    CFire* pNearestFire{};
    for (CFire& fire : m_aFires) {
        if (!fire.IsActive())
            continue;

        if (bCheckWasCreatedByScript && fire.IsScript())
            continue;

        if (bCheckIsBeingExtinguished && fire.IsBeingExtinguished())
            continue;

        if (fire.m_pEntityTarget && fire.m_pEntityTarget->IsPed())
            continue;

        const float fDist2DSq = (fire.m_vecPosition - point).SquaredMagnitude2D();
        if (fDist2DSq < fNearestDist2DSq) {
            fNearestDist2DSq = fDist2DSq;
            pNearestFire = &fire;
        }
    }
    return pNearestFire;
}

// 0x539340
bool CFireManager::PlentyFiresAvailable() {
    uint32 c = 0;
    for (CFire& fire : m_aFires) {
        if (fire.m_nFlags.bActive) 
            c++;
        if (c >= 6)
            return true;
    }
    return false;
}

// 0x539450
void CFireManager::ExtinguishPoint(CVector point, float fRadius) {
    for (CFire& fire : m_aFires) {
        if (!fire.IsActive())
            continue;

        if ((fire.m_vecPosition - point).SquaredMagnitude() > fRadius * fRadius)
            continue;

        fire.m_nFlags.bCreatedByScript = false;
        fire.Extinguish();
    }
}

// 0x5394C0
bool CFireManager::ExtinguishPointWithWater(CVector point, float fRadius, float fWaterStrength) {
    bool bSuccess = false;
    for (CFire& fire : m_aFires) {
        if (!fire.IsActive())
            continue;

        if ((fire.m_vecPosition - point).SquaredMagnitude() > fRadius * fRadius)
            continue;

        fire.ExtinguishWithWater(fWaterStrength);
        bSuccess = true;
    }
    return bSuccess;
}

// 0x5396E0
bool CFireManager::IsScriptFireExtinguished(int16 id) {
    return !Get(id).IsActive();
}

// 0x539700
void CFireManager::RemoveScriptFire(uint16_t fireID) {
    CFire& fire = Get(fireID);
    if (fire.m_nFlags.bCreatedByScript) {
        fire.m_nFlags.bCreatedByScript = false;
        fire.Extinguish();
    }
}

// 0x539720
void CFireManager::RemoveAllScriptFires() {
    for (CFire& fire : m_aFires) {
        if (fire.m_nFlags.bCreatedByScript) {
            fire.m_nFlags.bCreatedByScript = false;
            fire.Extinguish();
        }
    }
}

// 0x5397A0
void CFireManager::ClearAllScriptFireFlags() {
    for (CFire& fire : m_aFires) {
        fire.m_nFlags.bCreatedByScript = false;
    }
}

// 0x5397B0
void CFireManager::SetScriptFireAudio(int16 fireID, bool bFlag) {
    Get(fireID).m_nFlags.bMakesNoise = bFlag;
}

// 0x5397E0
const CVector& CFireManager::GetScriptFireCoords(int16 fireID) {
    return Get(fireID).m_vecPosition;
}

// 0x5397F0
uint32 CFireManager::GetNumFiresInRange(const CVector& point, float fRadius) {
    uint32 c = 0;
    for (CFire& fire : m_aFires) {
        if (fire.IsActive() && !fire.IsScript()) {
            if ((fire.m_vecPosition - point).Magnitude2D() <= fRadius) {
                c++;
            }
        }
    }
    return c;
}

// 0x539860
uint32 CFireManager::GetNumFiresInArea(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    CBoundingBox boundingBox{
        {minX, minY, minZ},
        {maxX, maxY, maxZ}
    };
    uint32 c = 0;
    for (CFire& fire : m_aFires) {
        if (fire.IsActive() && !fire.IsScript()) {
            if (boundingBox.IsPointWithin(fire.m_vecPosition)) {
                c++;
            }
        }
    }
    return c;
}

// 0x539D10
void CFireManager::DestroyAllFxSystems() {
    for (CFire& fire : m_aFires) {
        if (fire.IsActive()) {
            fire.DestroyFx();
        }
    }
}

// 0x539D50
void CFireManager::CreateAllFxSystems() {
    for (CFire& fire : m_aFires) {
        if (fire.IsActive()) {
            fire.CreateFxSysForStrength(fire.m_vecPosition, nullptr);
        }
    }
}

// 0x539E50
CFire* CFireManager::GetNextFreeFire(bool bMayExtinguish) {
    for (CFire& fire : m_aFires) {
        if (!fire.IsActive() && !fire.IsScript()) {
            return &fire;
        }
    }

    if (!bMayExtinguish)
        return nullptr;

    // At this point there are no inactive fires in the pool 
    // So try to extinguish a script / first generation fire
    for (CFire& fire : m_aFires) {
        if (fire.IsFirstGen() || fire.IsScript()) {
            fire.m_nFlags.bCreatedByScript = false;
            fire.Extinguish();
            return &fire;
        }
    }
    return nullptr;
}

// 0x539F00
CFire* CFireManager::StartFire(CVector pos, float size, uint8 unused, CEntity* creator, uint32 nTimeToBurn, int8_t nGenerations, uint8 unused_) {
    CFire* fire = GetNextFreeFire(false);
    if (!fire)
        return nullptr;

    fire->Start(creator, pos, nTimeToBurn, std::min<uint8>((uint8)m_nMaxFireGenerationsAllowed, nGenerations));
    return fire;
}

// 0x53A050
CFire* CFireManager::StartFire(CEntity* target, CEntity* creator, float size, uint8 unused, uint32 lifetime, int8_t numGenerations) {
    /* Do few checks, and clear `m_pFire` if `target` */
    switch (target->m_nType) {
    case eEntityType::ENTITY_TYPE_PED: {
        auto pedTarget = static_cast<CPed*>(target);
        if (!pedTarget->IsPedInControl())
            return nullptr;
        if (pedTarget->m_pFire)
            return nullptr;
        if (pedTarget->physicalFlags.bFireProof)
            return nullptr;
        break;
    }
    case eEntityType::ENTITY_TYPE_VEHICLE: {
        auto pVehTarget = static_cast<CVehicle*>(target);
        if (pVehTarget->m_pFire)
            return nullptr;

        if (pVehTarget->IsAutomobile()) {
            if (static_cast<CAutomobile*>(target)->m_damageManager.GetEngineStatus() >= 225)
                return nullptr;
        }
        if (pVehTarget->physicalFlags.bFireProof)
            return nullptr;
        break;
    }
    case eEntityType::ENTITY_TYPE_OBJECT: {
        if (static_cast<CObject*>(target)->m_pFire)
            return nullptr;
        break;
    }
    }

    if (auto fire = GetNextFreeFire(false))
        fire->Start(creator, target, lifetime, std::min<uint8>((uint8)m_nMaxFireGenerationsAllowed, numGenerations));

    return nullptr;
}

// 0x53A270
int32 CFireManager::StartScriptFire(const CVector& pos, CEntity* pTarget, float _fUnused, uint8 _nUnused, int8_t nGenerations, int32 nStrength) {
    if (pTarget) {
        /* Extinguish current fire (if any) of target */
        const auto StopFire = [](CFire* fire) {
            fire->Extinguish();
            fire->m_nFlags.bCreatedByScript = false;
        };
        switch (pTarget->m_nType) {
        case eEntityType::ENTITY_TYPE_PED: {
            auto pPedTarget = static_cast<CPed*>(pTarget);
            if (pPedTarget->m_pFire)
                StopFire(pPedTarget->m_pFire);
            break;
        }
        case eEntityType::ENTITY_TYPE_VEHICLE: {
            auto pVehTarget = static_cast<CVehicle*>(pTarget);
            if (pVehTarget->m_pFire)
                StopFire(pVehTarget->m_pFire);

            /* Set engine status for automobiles */
            if (pVehTarget->IsAutomobile()) {
                auto& dmgMgr = static_cast<CAutomobile*>(pVehTarget)->m_damageManager;
                if (dmgMgr.GetEngineStatus() >= 225)
                    dmgMgr.SetEngineStatus(215);
            }
            break;
        }
        }
    }

    if (auto fire = GetNextFreeFire(true)) {
        fire->Start(pos, (float)nStrength, pTarget, std::min<uint8>((uint8)m_nMaxFireGenerationsAllowed, nGenerations));
        return CTheScripts::GetNewUniqueScriptThingIndex(GetIndexOf(fire), 5);
    }
    return -1;
}

// 0x53AF00
void CFireManager::Update() {
    if (CReplay::Mode == 1)
        return;

    for (CFire& fire : m_aFires) {
        fire.ProcessFire();
    }

    if (CGameLogic::LaRiotsActiveHere() && (CTimer::m_snTimeInMilliseconds / 500u != CTimer::m_snPreviousTimeInMilliseconds / 500u)) {
        const float fRandomAngleRad = CGeneral::GetRandomNumberInRange(0.0f, 2 * rwPI);
        const float fRandomDir = CGeneral::GetRandomNumberInRange(35.0f, 60.0f);
        CVector point = TheCamera.GetPosition() + CVector{
            std::sin(fRandomAngleRad) * fRandomDir,
            std::cos(fRandomAngleRad) * fRandomDir,
            10.0f
        };

        CEntity* pHitEntity{};
        bool bHit{};
        point.z = CWorld::FindGroundZFor3DCoord(point.x, point.y, point.z, &bHit, &pHitEntity);

        CVector pointToCamDirNorm = (TheCamera.GetPosition() - point);
        pointToCamDirNorm.Normalise();

        // In the PC version 0.2f and "riot_smoke" is accessed through an array
        // On the Android version it's inlined
        // Android values used here, and on the PC version the array access isn't
        // a 100% clear to me.
        // TODO: Use the array here
        if (DotProduct(TheCamera.GetForward(), pointToCamDirNorm) > 0.2f || rand() < RAND_MAX / 2) {
            auto pFx = g_fxMan.CreateFxSystem("riot_smoke", &point, nullptr, true);
            if (pFx)
                pFx->PlayAndKill();
        } else {
            StartFire(point, CGeneral::GetRandomNumberInRange(0.5f, 2.5f), true, nullptr, 30000u, 5, 1);
        }
    }

    auto nFires = (int32)GetNumOfFires();
    bool firesVisited[60] = {false}; // Lookup table to see if a fire's strength was already included into a group of fires
    while (nFires > 0) {
        // Repeat until there are no active fires left 
        // Find strongest un-visited fire, and sum of the strength of all fires within 6.0 units of it 
        // Based on this strength possibly create a shadow (if combined strength > 4), and coronas (combined strength > 6)

        // Find strongest fire, which hasn't yet been visited
        CFire* pStrongest{};
        for (size_t i = 0; i < 60; i++) {
            CFire& fire = Get(i);
            if (firesVisited[i] || !fire.IsActive())
                continue;
            if (!pStrongest || pStrongest->m_fStrength < fire.m_fStrength)
                pStrongest = &fire;
        }

        // Sum up strengths of all fires (that haven't yet been visited) within 6.0 units range
        float fCombinedStrength{};
        int32 nCombinedCeilStrength{};
        for (size_t i = 0; i < 60; i++) {
            CFire& fire = Get(i);
            if (firesVisited[i] || !fire.IsActive())
                continue;
            if ((fire.m_vecPosition - pStrongest->m_vecPosition).Magnitude2D() < 6.0f) {
                fCombinedStrength += fire.m_fStrength;
                nCombinedCeilStrength += (int32)std::ceil(fire.m_fStrength);
                nFires--;
                firesVisited[i] = true;

                // R* also does some weird vector point calculation,
                // which doesn't seem to end up being used. Probably old code left in?
                // <vector defined where `fCombinedStrength` is, lets name it averagedFirePos = {}>
                // averagedFirePos += fire.m_vecPosition * std::ceil(fire.m_fStrength);
            }
        }

        if (fCombinedStrength > 4.0f && nCombinedCeilStrength) {
            const CVector baseColor{ 40.0f, 32.0f, 20.0f }; // From addresses 0x8CCFF0, 0x8CCFF1, 0x8CCFF2 (These are int values originally)

            const float fDir = std::min(7.0f, fCombinedStrength - 6.0f + 3.0f);

            CVector shdwPos = pStrongest->m_vecPosition;
            shdwPos.z += 5.0f;
            const float fColorMult = CGeneral::GetRandomNumberInRange(0.6f, 1.0f);
            {
                const CVector shdwColor = baseColor * fColorMult;
                CShadows::StoreStaticShadow(
                    reinterpret_cast<uint32>(pStrongest),
                    2,
                    gpShadowExplosionTex,
                    &shdwPos,
                    fDir * -1.2f,
                    0.0f,
                    0.0f,
                    fDir * 1.2f,
                    0,
                    (uint8)shdwColor.x,
                    (uint8)shdwColor.y,
                    (uint8)shdwColor.z,
                    10.0f,
                    1.0f,
                    40.0f,
                    false,
                    0.0f
                );
            }
            if (fCombinedStrength > 6.0f) {
                // Create coronas in a _|_ like shape.
                // Keep in mind, the right line's end is always pointing towards the camera,
                // so what you see is more like |

                CVector point = pStrongest->m_vecPosition + CVector{ 0.0f, 0.0f, 2.6f };
                {
                    CVector camToPointDirNorm = TheCamera.GetPosition() - point;
                    camToPointDirNorm.Normalise();
                    point += camToPointDirNorm * 3.5f;
                }

                // Wrapper lambda for code readability
                const auto RegisterCorona = [&](auto idx, CVector pos, eCoronaFlareType flare = eCoronaFlareType::FLARETYPE_NONE) {
                    const CVector crnaColor = baseColor * (fColorMult * 0.8f);
                    CCoronas::RegisterCorona(
                        reinterpret_cast<uint32>(pStrongest),
                        nullptr,
                        (uint8)crnaColor.x, // r
                        (uint8)crnaColor.y, // g
                        (uint8)crnaColor.z, // b
                        0xFF,                       // a
                        pos,
                        fDir * 0.5f,
                        70.0f,
                        eCoronaType::CORONATYPE_HEADLIGHT,
                        flare,
                        false,
                        false,
                        0,
                        0.0f,
                        false,
                        1.5f,
                        0,
                        15.0f,
                        false,
                        false
                    );
                };

                // Each corona only differs in position, and ID
                RegisterCorona(reinterpret_cast<uint32>(pStrongest), point, eCoronaFlareType::FLARETYPE_HEADLIGHTS);

                point.z += 2.0f;
                RegisterCorona(reinterpret_cast<uint32>(pStrongest) + 1, point);
                point.z -= 2.0f;

                CVector camRightNorm = TheCamera.GetRight();
                camRightNorm.z = 0.0f;
                camRightNorm.Normalise();

                point += camRightNorm * 2.0f;
                RegisterCorona(reinterpret_cast<uint32>(pStrongest) + 2, point);

                point -= camRightNorm * 4.0f;
                RegisterCorona(reinterpret_cast<uint32>(pStrongest) + 3, point);
            }
        }

    }
}
