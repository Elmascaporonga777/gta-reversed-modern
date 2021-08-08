#include "StdInc.h"

CFireManager& gFireManager = *reinterpret_cast<CFireManager*>(0xB71F80);

void CFireManager::InjectHooks() {
    //ReversibleHooks::Install("CFireManager", "Constructor", 0x539DA0, &CFireManager::Constructor);
    //ReversibleHooks::Install("CFireManager", "Destructor", 0x538BB0, &CFireManager::Destructor);
    ReversibleHooks::Install("CFireManager", "Init", 0x538BC0, &CFireManager::Init);
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
    ReversibleHooks::Install("CFireManager", "Shutdown", 0x539DD0, &CFireManager::Shutdown);
    ReversibleHooks::Install("CFireManager", "GetNextFreeFire", 0x539E50, &CFireManager::GetNextFreeFire);
    ReversibleHooks::Install("CFireManager", "StartFire_NoTarget", 0x539F00, static_cast<CFire*(CFireManager::*)(CVector, float, uint8_t, CEntity*, uint32_t, uint8_t, uint8_t)>(&CFireManager::StartFire));
    ReversibleHooks::Install("CFireManager", "StartFire", 0x53A050, static_cast<CFire *(CFireManager::*)(CEntity*, CEntity*, float, uint8_t, uint32_t, uint8_t)>(&CFireManager::StartFire));
    ReversibleHooks::Install("CFireManager", "StartScriptFire", 0x53A270, &CFireManager::StartScriptFire);
    //ReversibleHooks::Install("CFireManager", "Update", 0x53AF00, &CFireManager::Update);
}

CFireManager::CFireManager() {
}

CFireManager::~CFireManager() {
}

CFireManager* CFireManager::Destructor() {
    this->CFireManager::~CFireManager();
    return this;
}

CFireManager* CFireManager::Constructor() {
    this->CFireManager::CFireManager();
    return this;
}

void CFireManager::Init() {
    for (auto& fire : m_aFires) {
        fire.Initialise();
    }
    m_nMaxFireGenerationsAllowed = 1'000'000 - 1;
}

uint32_t CFireManager::GetNumOfNonScriptFires() {
    uint32_t c = 0;
    for (auto& fire : m_aFires)
        if (fire.m_nFlags.bActive && !fire.m_nFlags.bCreatedByScript)
            c++;
    return c;
}

CFire * CFireManager::FindNearestFire(CVector const& point, bool bCheckIsBeingExtinguished, bool bCheckWasCreatedByScript) {
    float fNearestDist2DSq = std::numeric_limits<float>::max();
    CFire* pNearestFire;
    for (auto& fire : m_aFires) {
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

bool CFireManager::PlentyFiresAvailable() {
    uint32_t c = 0;
    for (auto& fire : m_aFires) {
        if (fire.m_nFlags.bActive) 
            c++;
        if (c >= 6)
            return true;
    }
    return false;
}

void CFireManager::ExtinguishPoint(CVector point, float fRadius) {
    for (auto& fire : m_aFires) {
        if (!fire.IsActive())
            continue;
        if ((fire.m_vecPosition - point).SquaredMagnitude() > fRadius * fRadius)
            continue;
        fire.m_nFlags.bCreatedByScript = false;
        fire.Extinguish();
    }
}

bool CFireManager::ExtinguishPointWithWater(CVector point, float fRadius, float fWaterStrength) {
    bool bSuccess = false;
    for (auto& fire : m_aFires) {
        if (!fire.IsActive())
            continue;
        if ((fire.m_vecPosition - point).SquaredMagnitude() > fRadius * fRadius)
            continue;
        fire.ExtinguishWithWater(fWaterStrength);
        bSuccess = true;
    }
    return bSuccess;
}

bool CFireManager::IsScriptFireExtinguished(short id) {
    return !Get(id).IsActive();
}

void CFireManager::RemoveScriptFire(uint16_t fireID) {
    CFire& fire = Get(fireID);
    if (fire.m_nFlags.bCreatedByScript) {
        fire.m_nFlags.bCreatedByScript = false;
        fire.Extinguish();
    }
}

void CFireManager::RemoveAllScriptFires() {
    for (auto& fire : m_aFires) {
        if (fire.m_nFlags.bCreatedByScript) {
            fire.m_nFlags.bCreatedByScript = false;
            fire.Extinguish();
        }
    }
}

void CFireManager::ClearAllScriptFireFlags() {
    for (auto& fire : m_aFires) {
        fire.m_nFlags.bCreatedByScript = false;
    }
}

void CFireManager::SetScriptFireAudio(short fireID, bool bFlag) {
    Get(fireID).m_nFlags.bMakesNoise = bFlag;
}

const CVector& CFireManager::GetScriptFireCoords(short fireID) {
    return Get(fireID).m_vecPosition;
}

uint32_t CFireManager::GetNumFiresInRange(const CVector& point, float fRadius) {
    uint32_t c = 0;
    for (auto& fire : m_aFires) {
        if (fire.IsActive() && !fire.IsScript()) {
            if ((fire.m_vecPosition - point).Magnitude2D() <= fRadius) {
                c++;
            }
        }
    }
    return c;
}

uint32_t CFireManager::GetNumFiresInArea(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    CBoundingBox boundingBox{
        {minX, minY, minZ},
        {maxX, maxY, maxZ}
    };
    uint32_t c = 0;
    for (auto& fire : m_aFires) {
        if (fire.IsActive() && !fire.IsScript()) {
            if (boundingBox.IsPointWithin(fire.m_vecPosition)) {
                c++;
            }
        }
    }
    return c;
}
 
void CFireManager::DestroyAllFxSystems() {
    for (auto& fire : m_aFires) {
        if (fire.IsActive()) {
            fire.DestroyFx();
        }
    }
}

void CFireManager::CreateAllFxSystems() {
    for (auto& fire : m_aFires) {
        if (fire.IsActive()) {
            fire.CreateFxSysForStrength(fire.m_vecPosition, nullptr);
        }
    }
}

void CFireManager::Shutdown() {
    for (auto& fire : m_aFires) {
        fire.m_nFlags.bCreatedByScript = false;
        if (fire.IsActive()) {
            fire.Extinguish();
        }
    }
}

CFire* CFireManager::GetNextFreeFire(bool bMayExtinguish) {
    for (auto& fire : m_aFires) {
        if (!fire.IsActive() && !fire.IsScript()) {
            return &fire;
        }
    }

    if (!bMayExtinguish)
        return nullptr;

    // At this point there are no inactive fires in the pool 
    // So try to extinguish a script / first generation fire
    for (auto& fire : m_aFires) {
        if (fire.IsFirstGen() || fire.IsScript()) {
            fire.m_nFlags.bCreatedByScript = false;
            fire.Extinguish();
            return &fire;
        }
    }
    return nullptr;
}

CFire * CFireManager::StartFire(CVector pos, float fStrength, uint8_t unused, CEntity * pCreator, uint32_t nTimeToBurn, uint8_t nGenerations, uint8_t unused_) {
    CFire* pFire = GetNextFreeFire(false);
    if (!pFire)
        return nullptr;
    pFire->Start(pCreator, pos, nTimeToBurn, std::min<uint8_t>((uint8_t)m_nMaxFireGenerationsAllowed, nGenerations));
    return pFire;
}

CFire * CFireManager::StartFire(CEntity * pTarget, CEntity * pCreator, float fStrength, uint8_t unused, uint32_t lifetime, uint8_t nGenerations) {
    /* Do few checks, and clear `m_pFire` if `pTarget` */
    switch (pTarget->m_nType) {
    case eEntityType::ENTITY_TYPE_PED: {
        auto pPedTarget = static_cast<CPed*>(pTarget);
        if (!pPedTarget->IsPedInControl())
            return nullptr;
        if (pPedTarget->m_pFire)
            return nullptr;
        if (pPedTarget->physicalFlags.bFireProof)
            return nullptr;
        break;
    }
    case eEntityType::ENTITY_TYPE_VEHICLE: {
        auto pVehTarget = static_cast<CVehicle*>(pTarget);
        if (pVehTarget->m_pFire)
            return nullptr;
        if (pVehTarget->IsAutomobile()) {
            if (static_cast<CAutomobile*>(pTarget)->m_damageManager.GetEngineStatus() >= 225)
                return nullptr;
        }
        if (pVehTarget->physicalFlags.bFireProof)
            return nullptr;
        break;
    }
    case eEntityType::ENTITY_TYPE_OBJECT: {
        if (static_cast<CObject*>(pTarget)->m_pFire)
            return nullptr;
        break;
    }
    }
    if (auto pFire = GetNextFreeFire(false))
        pFire->Start(pCreator, pTarget, lifetime, std::min<uint8_t>((uint8_t)m_nMaxFireGenerationsAllowed, nGenerations));
    return nullptr;
}

int32_t CFireManager::StartScriptFire(CVector const& pos, CEntity * pTarget, float _fUnused, uint8_t _nUnused, uint8_t nGenerations, int32_t nStrength) {

    if (pTarget) {
        /* Extinguish current fire (if any) of target */
        const auto StopFire = [](CFire* pFire) {
            pFire->Extinguish();
            pFire->m_nFlags.bCreatedByScript = false;
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

    if (auto pFire = GetNextFreeFire(true)) {
        pFire->Start(pos, (float)nStrength, pTarget, std::min<uint8_t>((uint8_t)m_nMaxFireGenerationsAllowed, nGenerations));
        return CTheScripts::GetNewUniqueScriptThingIndex(GetIndexOf(pFire), 5);
    }
    return -1;
}

void CFireManager::Update() {
    return plugin::CallMethod<0x53AF00, CFireManager*>(this);
}
