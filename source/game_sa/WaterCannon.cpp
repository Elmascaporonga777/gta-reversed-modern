#include "StdInc.h"

#include "WaterCannon.h"

RxVertexIndex (&CWaterCannon::m_auRenderIndices)[18] = *(RxVertexIndex (*)[18])0xC80700;

void CWaterCannon::InjectHooks() {
    ReversibleHooks::Install("CWaterCannon", "Constructor", 0x728B10, &CWaterCannon::Constructor);
    ReversibleHooks::Install("CWaterCannon", "Destructor", 0x728B30, &CWaterCannon::Destructor);
    ReversibleHooks::Install("CWaterCannon", "Init", 0x728B40, &CWaterCannon::Init);
    ReversibleHooks::Install("CWaterCannon", "Update_OncePerFrame", 0x72A280, &CWaterCannon::Update_OncePerFrame);
    ReversibleHooks::Install("CWaterCannon", "Update_NewInput", 0x728C20, &CWaterCannon::Update_NewInput);
    ReversibleHooks::Install("CWaterCannon", "PushPeds", 0x7295E0, &CWaterCannon::PushPeds);
    ReversibleHooks::Install("CWaterCannon", "Render", 0x728DA0, &CWaterCannon::Render);
}

// 0x728B10
CWaterCannon::CWaterCannon() {
    // NOP
}

CWaterCannon* CWaterCannon::Constructor() {
    this->CWaterCannon::CWaterCannon();
    return this;
}

// 0x728B30
CWaterCannon::~CWaterCannon() {
    // NOP
}

CWaterCannon* CWaterCannon::Destructor() {
    CWaterCannon::~CWaterCannon();
    return this;
}

// 0x728B40
void CWaterCannon::Init() {
    m_nId = 0;
    m_nSectionsCount = 0;
    m_nCreationTime = CTimer::GetTimeInMS();
    m_anSectionState[0] = '\0';

    m_auRenderIndices[0] = 0;
    m_auRenderIndices[1] = 1;
    m_auRenderIndices[2] = 2;

    m_auRenderIndices[3] = 1;
    m_auRenderIndices[4] = 3;
    m_auRenderIndices[5] = 2;

    m_auRenderIndices[6] = 4;
    m_auRenderIndices[7] = 5;
    m_auRenderIndices[8] = 6;

    m_auRenderIndices[9] = 5;
    m_auRenderIndices[10] = 7;
    m_auRenderIndices[11] = 6;

    m_auRenderIndices[12] = 8;
    m_auRenderIndices[13] = 9;
    m_auRenderIndices[14] = 10;

    m_auRenderIndices[15] = 9;
    m_auRenderIndices[16] = 11;
    m_auRenderIndices[17] = 10;

    m_audio.Initialise(this);
}

bool CWaterCannon::HasActiveSection() const {
    const auto end = std::end(m_anSectionState);
    return std::find(std::begin(m_anSectionState), end, true) != end;
}

// 0x72A280
void CWaterCannon::Update_OncePerFrame(short a1) {
    if (CTimer::GetTimeInMS() > m_nCreationTime + 150) {
        const auto section = (m_nSectionsCount + 1) % SECTIONS_COUNT;
        m_nSectionsCount = section;
        m_anSectionState[section] = false;
    }

    for (int i = 0; i < SECTIONS_COUNT; i++) {
        if (m_anSectionState[i]) {
            CVector& speed = m_sectionMoveSpeed[i];
            speed.z -= CTimer::GetTimeStep() / 250.0f;

            CVector& point = m_sectionPoint[i];
            point += speed * CTimer::GetTimeStep();

            // Originally done in a seprate loop, but we do it here
            gFireManager.ExtinguishPointWithWater(point, 2.0f, 0.5f);
        }
    }

    if ((uint8_t)(CTimer::m_FrameCounter + a1) % 4 == 0) { // Notice cast to byte
        PushPeds();
    }

    if (!HasActiveSection()) {
        m_nId = 0;
    }
}

// 0x728C20
void CWaterCannon::Update_NewInput(CVector* start, CVector* end) {
    m_sectionPoint[m_nSectionsCount]     = *start;
    m_sectionMoveSpeed[m_nSectionsCount] = *end;
    m_anSectionState[m_nSectionsCount]   = 1;
}

// NOTSA
CBoundingBox CWaterCannon::GetSectionsBoundingBox() const {
    // Ik, ik junk code, can't do better

    // R* originally used 10000 here, but thats bug prone (if map size gets increased)
    CVector min{ FLT_MAX, FLT_MAX, FLT_MAX }, max{ FLT_MIN, FLT_MIN, FLT_MIN };  
    for (size_t i = 0; i < SECTIONS_COUNT; i++) {
        if (!IsSectionActive(i))
            continue;

        const auto Do = [posn = GetSectionPosn(i)](CVector& out, auto pr) {
            out = CVector{
                pr(out.x, posn.x),
                pr(out.y, posn.y),
                pr(out.z, posn.z)
            };
        };
        Do(min, [](float a, float b) { return std::min(a, b); });
        Do(max, [](float a, float b) { return std::max(a, b); });
    }
    return { min, max };
}

// 0x7295E0
void CWaterCannon::PushPeds() {
    const auto sectionsBounding = GetSectionsBoundingBox();
    for (int pedIdx = 0; pedIdx < CPools::ms_pPedPool->m_nSize; pedIdx++) {
        CPed* ped = CPools::ms_pPedPool->GetAt(pedIdx);
        if (!ped)
            continue;

        CVector pedPosn = ped->GetPosition();
        if (!sectionsBounding.IsPointWithin(pedPosn))
            continue;

        if (!ped->physicalFlags.bMakeMassTwiceAsBig)
            continue;

        for (int i = 0; i < SECTIONS_COUNT; i++) {
            const CVector secPosn = GetSectionPosn(i);
            if ((pedPosn - secPosn).SquaredMagnitude() >= 5.0f)
                continue;

            const CVector secMoveSpeed = GetSectionMoveSpeed(i);

            {
                CEventHitByWaterCannon event(secPosn, secMoveSpeed);
                ped->GetEventGroup().Add(&event, false);
            }

            ped->bWasStanding = false;
            ped->bIsStanding = false;

            ped->ApplyMoveForce({ 0.0f, 0.0f, CTimer::GetTimeStep() });

            {
                // TODO: Refactor... Ugly code
                const CVector applyableMoveSpeed = (secMoveSpeed / 10.0f - ped->m_vecMoveSpeed) / 10.0f;

                // Check if directions are the same (eg, + / +, - / -),
                // differring sign bits will always yield a negativ result
                /// (unless 0, but that is handled by > (instead of >=))
                if (secMoveSpeed.x * applyableMoveSpeed.x > 0.0f) 
                    ped->m_vecMoveSpeed.x = applyableMoveSpeed.x + secMoveSpeed.x;

                if (secMoveSpeed.y * applyableMoveSpeed.y > 0.0f)
                    ped->m_vecMoveSpeed.y = applyableMoveSpeed.y + secMoveSpeed.y;
            }

            FxPrtMult_c prtInfo{ 1.0f, 1.0f, 1.0f, 0.6f, 0.75f, 0.0f, 0.2f };

            // TODO: Fix fx crap
            CVector prtVel = ped->m_vecMoveSpeed * 0.3f;
            g_fx.m_pPrtSmokeII3expand->AddParticle(&pedPosn, &prtVel, 0.0f, &prtInfo, -1.0f, 1.2f, 0.6f, false);

            CVector prtVel2 = ped->m_vecMoveSpeed * -0.3f;
            prtVel2.z += 0.5f;
            g_fx.m_pPrtSmokeII3expand->AddParticle(&pedPosn, &prtVel2, 0.0f, &prtInfo, -1.0f, 1.2f, 0.6f, false);

            break;
        }
    }
}

// 0x728DA0
void CWaterCannon::Render() {
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,         (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE,          (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,    (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND,             (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,            (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE,            (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER,        (void*)nullptr);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION,    (void*)rwALPHATESTFUNCTIONGREATER);
    RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, (void*)FALSE);

    size_t prevIdx = m_nSectionsCount % SECTIONS_COUNT;
    size_t currIdx = prevIdx == 0 ? SECTIONS_COUNT - 1 : prevIdx - 1;

    bool hasCalculatedMatrix = false;

    CVector right{}, fwd{}, up{};

    m_audio.ClearSplashInfo();

    for (int i = 0; i < SECTIONS_COUNT; i++) {
        if (IsSectionActive(prevIdx) && IsSectionActive(currIdx)) {
            const CVector prevPosn = GetSectionPosn(prevIdx);
            const CVector currPosn = GetSectionPosn(currIdx);

            const CVector currToPrevDir = prevPosn - currPosn;
            if (currToPrevDir.SquaredMagnitude() < 25.0f) {
                if (!hasCalculatedMatrix) {
                    hasCalculatedMatrix = true;

                    up = Normalized(CrossProduct(prevPosn - currPosn, TheCamera.GetForward())) / 20.0f;
                    right = Normalized(CVector::Random(0.0f, 1.0f)) / 20.0f;
                    fwd = Normalized(CVector::Random(0.0f, 1.0f)) / 20.0f;
                }

                const float dist = (float)(i * i) / (float)SECTIONS_COUNT + 3.0f;

                RxObjSpace3DVertex vertices[12];

                // Set alpha depending on current `i`. The higher, the lower the alpha.
                const float progress = (float)i / (float)SECTIONS_COUNT;
                const auto  alpha = (RwUInt8)(64.0f * (1.0f - progress));
                RwRGBA color{ 200, 200, 255, alpha };
                for (auto& v : vertices) {
                    RxObjSpace3DVertexSetPreLitColor(&v, &color);
                }

                const CVector thisUp = up * dist, thisRight = right * dist, thisFwd = fwd * dist;
                const CVector pos[std::size(vertices)] = {
                    currPosn - thisUp,
                    currPosn + thisUp,
                    prevPosn - thisUp,
                    prevPosn + thisUp,

                    currPosn - thisRight,
                    currPosn + thisRight,
                    prevPosn - thisRight,
                    prevPosn + thisRight,

                    currPosn - thisFwd,
                    currPosn + thisFwd,
                    prevPosn - thisFwd,
                    prevPosn + thisFwd,
                };
                for (size_t v = 0; v < std::size(vertices); v++) {
                    RxObjSpace3DVertexSetPos(&vertices[v], &pos[v]);
                }

                CColPoint colPoint{};
                CEntity* hitEntity{};
                const bool hasSectionHit = CWorld::ProcessLineOfSight(prevPosn, currPosn, colPoint, hitEntity, true, true, false, false, false, false, false, false);
                if (hasSectionHit) {
                    FxPrtMult_c prtinfo{ 1.0f, 1.0f, 1.0f, 0.15f, 0.75f, 1.0f, 0.2f };
                    CVector direction = colPoint.m_vecNormal * 3.0f * CVector::Random(0.2f, 1.8f);

                    for (int n = 0; n < 2; n++) {
                        const float unk = n / CTimer::GetTimeStepInMS();

                        g_fx.m_pPrtWatersplash->AddParticle(&colPoint.m_vecPoint, &direction, unk, &prtinfo, -1.0f, 1.2f, 0.6f, 0);

                        CVector retadedSignature = direction * 0.6f;
                        g_fx.m_pPrtWatersplash->AddParticle(&colPoint.m_vecPoint, &retadedSignature, unk, &prtinfo, -1.0f, 1.2f, 0.6f, 0);
                    }

                    m_audio.SetSplashInfo(colPoint.m_vecPoint, direction.Magnitude());

                    break;
                }


                if (RwIm3DTransform(vertices, std::size(vertices), nullptr, rwIM3D_VERTEXRGBA))
                {
                    RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, m_auRenderIndices, std::size(m_auRenderIndices));
                    RwIm3DEnd();
                }

                if (hasSectionHit)
                    break;
            }
        }
        currIdx = prevIdx;
        if (prevIdx == 0)
            prevIdx = SECTIONS_COUNT - 1;
        else
            prevIdx--;
    }

    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,      (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATEFOGENABLE,         (void*)FALSE);
}

// NOTSA
bool CWaterCannon::IsSectionActive(size_t idx) const {
    return m_anSectionState[idx];
}

CVector CWaterCannon::GetSectionPosn(size_t idx) const {
    return m_sectionPoint[idx];
}

CVector CWaterCannon::GetSectionMoveSpeed(size_t idx) const {
    return m_sectionMoveSpeed[idx];
}
